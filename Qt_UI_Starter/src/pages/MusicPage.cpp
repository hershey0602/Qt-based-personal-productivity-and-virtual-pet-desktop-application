#include "MusicPage.h"

#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QSlider>
#include <limits>

namespace fg {
namespace {
QString formatTime(qint64 milliseconds)
{
    const qint64 seconds = qMax<qint64>(0, milliseconds) / 1000;
    return QStringLiteral("%1:%2").arg(seconds / 60, 2, 10, QChar('0'))
        .arg(seconds % 60, 2, 10, QChar('0'));
}
}

MusicPage::MusicPage(AppServices& services, QWidget* parent)
    : LayoutPage(services, QStringLiteral("music"), parent)
{
    connect(this, &LayoutPage::actionRequested, this, [this](const QString& action) {
        if (action == QStringLiteral("music.import")) {
            const QStringList paths = QFileDialog::getOpenFileNames(
                this, QStringLiteral("添加本地音乐"), QString(),
                QStringLiteral("音频 (*.mp3 *.wav *.flac *.ogg *.m4a *.aac *.opus);;所有文件 (*.*)"));
            if (!paths.isEmpty()) {
                showResult(services_.music().importFiles(paths));
            }
        } else if (action == QStringLiteral("music.remove")) {
            auto* list = qobject_cast<QListWidget*>(component(QStringLiteral("music.list")));
            if (list) {
                showResult(services_.music().remove(list->currentRow()));
            }
        } else if (action == QStringLiteral("music.previous")) {
            showResult(services_.music().previous());
        } else if (action == QStringLiteral("music.play")) {
            showResult(services_.music().playPause());
        } else if (action == QStringLiteral("music.stop")) {
            showResult(services_.music().stop());
        } else if (action == QStringLiteral("music.next")) {
            showResult(services_.music().next());
        }
    });

    auto* list = qobject_cast<QListWidget*>(component(QStringLiteral("music.list")));
    connect(list, &QListWidget::currentRowChanged, this, [this](int row) {
        if (row >= 0) {
            showResult(services_.music().select(row));
        }
    });
    connect(list, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
        if (!item) {
            return;
        }
        const auto selected = services_.music().select(item->listWidget()->row(item));
        if (!selected.ok) {
            showResult(selected);
            return;
        }
        showResult(services_.music().playPause());
    });

    auto* seek = qobject_cast<QSlider*>(component(QStringLiteral("music.seek")));
    connect(seek, &QSlider::sliderMoved, this, [this](int position) {
        showResult(services_.music().seek(position));
    });
    auto* volume = qobject_cast<QSlider*>(component(QStringLiteral("music.volumeSlider")));
    connect(volume, &QSlider::valueChanged, this, [this, volume](int value) {
        if (!volume->isSliderDown()) {
            return;
        }
        const auto result = services_.music().setVolume(value);
        if (!result.ok) {
            showResult(result);
            renderMusic(services_.music().snapshot());
        }
    });
    refresh();
}

void MusicPage::refresh()
{
    refreshView();
    renderMusic(services_.music().snapshot());
}

void MusicPage::renderMusic(fg::MusicSnapshot snapshot)
{
    auto* list = qobject_cast<QListWidget*>(component(QStringLiteral("music.list")));
    auto* empty = qobject_cast<QLabel*>(component(QStringLiteral("music.empty")));
    auto* remove = qobject_cast<QPushButton*>(component(QStringLiteral("music.remove")));
    auto* play = qobject_cast<QPushButton*>(component(QStringLiteral("music.play")));
    auto* previous = qobject_cast<QPushButton*>(component(QStringLiteral("music.previous")));
    auto* next = qobject_cast<QPushButton*>(component(QStringLiteral("music.next")));
    auto* stop = qobject_cast<QPushButton*>(component(QStringLiteral("music.stop")));
    auto* seek = qobject_cast<QSlider*>(component(QStringLiteral("music.seek")));
    auto* volume = qobject_cast<QSlider*>(component(QStringLiteral("music.volumeSlider")));
    auto* track = qobject_cast<QLabel*>(component(QStringLiteral("music.track")));
    auto* hint = qobject_cast<QLabel*>(component(QStringLiteral("music.trackHint")));
    auto* position = qobject_cast<QLabel*>(component(QStringLiteral("music.position")));
    auto* duration = qobject_cast<QLabel*>(component(QStringLiteral("music.duration")));
    if (!list || !empty || !remove || !play || !previous || !next || !stop || !seek || !volume
        || !track || !hint || !position || !duration) {
        return;
    }

    const QSignalBlocker listBlocker(list);
    list->clear();
    for (const MusicTrack& item : snapshot.tracks) {
        auto* row = new QListWidgetItem(item.displayName, list);
        row->setToolTip(item.available ? item.absolutePath
                                       : QStringLiteral("文件不可用：%1").arg(item.absolutePath));
        if (!item.available) {
            row->setForeground(QColor(QStringLiteral("#A07878")));
        }
    }
    if (snapshot.currentIndex.has_value() && *snapshot.currentIndex >= 0
        && *snapshot.currentIndex < list->count()) {
        list->setCurrentRow(*snapshot.currentIndex);
    }

    const bool hasTracks = !snapshot.tracks.isEmpty();
    const bool hasSelection = snapshot.currentIndex.has_value();
    empty->setVisible(!hasTracks);
    list->setEnabled(hasTracks);
    remove->setEnabled(hasSelection);
    previous->setEnabled(hasTracks);
    next->setEnabled(hasTracks);
    stop->setEnabled(hasSelection);
    play->setEnabled(hasTracks);
    play->setText(snapshot.state == PlaybackState::Playing ? QStringLiteral("暂停") : QStringLiteral("播放"));
    track->setText(hasSelection ? snapshot.tracks.at(*snapshot.currentIndex).displayName
                                : QStringLiteral("尚未选择音频"));
    hint->setText(hasSelection && !snapshot.tracks.at(*snapshot.currentIndex).available
                      ? QStringLiteral("该文件已失效，请移除后重新导入")
                      : QStringLiteral("添加音乐后可以在这里播放"));
    position->setText(formatTime(snapshot.positionMs));
    duration->setText(formatTime(snapshot.durationMs));
    const int sliderMaximum = int(qBound<qint64>(qint64(1), snapshot.durationMs,
                                                  qint64(std::numeric_limits<int>::max())));
    seek->setMaximum(sliderMaximum);
    seek->setValue(int(qBound<qint64>(qint64(0), snapshot.positionMs,
                                      qint64(sliderMaximum))));
    seek->setEnabled(snapshot.seekable && hasSelection);
    {
        const QSignalBlocker volumeBlocker(volume);
        volume->setValue(snapshot.volume);
    }
}
}

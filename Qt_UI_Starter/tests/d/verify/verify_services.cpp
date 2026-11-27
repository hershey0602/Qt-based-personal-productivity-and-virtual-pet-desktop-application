#include <QtTest>

#include <QAudioOutput>
#include <QFile>
#include <QMediaPlayer>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QSoundEffect>
#include <memory>

#include "services/CompletionSound.h"
#include "services/MusicController.h"
#include "services/SettingsService.h"

namespace fg {
class DServiceVerify : public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void d_t01_loadAndPlayWav();
    void d_t02_pauseAndStop();
    void d_t03_previousNextWrap();
    void d_t04_seekAndVolume();
    void d_t05_badPathSingleError();
    void d_t06_rebuildRestoresWithoutAutoplay();
    void d_t07_preferencesPersist();
    void d_t09_completionSoundSwitchAndDedup();

private:
    QTemporaryDir temp_;
    QString audioPath1_;
    QString audioPath2_;

    SettingsService* makeSettings();
    MusicController* makeController(SettingsService& settings);
    bool waitForPlaying(MusicController& controller, int timeoutMs = 5000);
    bool waitForPosition(MusicController& controller, qint64 minimumMs, int timeoutMs = 5000);
};

void DServiceVerify::initTestCase()
{
    QVERIFY(temp_.isValid());
    const QString source = QStringLiteral(FG_AUDIO_PATH);
    QVERIFY2(QFile::exists(source), qPrintable(source));
    audioPath1_ = temp_.filePath(QStringLiteral("verify-1.wav"));
    audioPath2_ = temp_.filePath(QStringLiteral("verify-2.wav"));
    QVERIFY(QFile::copy(source, audioPath1_));
    QVERIFY(QFile::copy(source, audioPath2_));
}

SettingsService* DServiceVerify::makeSettings()
{
    auto* settings = new SettingsService(temp_.filePath(QStringLiteral("service.ini")));
    Q_UNUSED(settings->load());
    return settings;
}

MusicController* DServiceVerify::makeController(SettingsService& settings)
{
    auto* controller = new MusicController(settings);
    if (!controller->loadPlaylist().ok) {
        delete controller;
        return nullptr;
    }
    return controller;
}

bool DServiceVerify::waitForPlaying(MusicController& controller, int timeoutMs)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < timeoutMs) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        if (controller.snapshot().state == PlaybackState::Playing) {
            return true;
        }
        QTest::qSleep(20);
    }
    return false;
}

bool DServiceVerify::waitForPosition(MusicController& controller, qint64 minimumMs, int timeoutMs)
{
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < timeoutMs) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        if (controller.snapshot().positionMs >= minimumMs) {
            return true;
        }
        QTest::qSleep(20);
    }
    return false;
}

void DServiceVerify::d_t01_loadAndPlayWav()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    MusicPreferences music;
    music.localPaths = {audioPath1_, audioPath2_};
    QVERIFY(settings->saveMusicPreferences(music).ok);
    std::unique_ptr<MusicController> controller(makeController(*settings));
    QVERIFY(controller);
    QCOMPARE(controller->snapshot().tracks.size(), 2);
    QVERIFY(!controller->snapshot().currentIndex.has_value());
    QVERIFY(controller->select(0).ok);
    QVERIFY(controller->playPause().ok);
    QVERIFY2(waitForPlaying(*controller), "QMediaPlayer never entered PlayingState");
    QVERIFY2(waitForPosition(*controller, 0), "QMediaPlayer position never advanced");
    QVERIFY(controller->snapshot().durationMs > 0);
}

void DServiceVerify::d_t02_pauseAndStop()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    MusicPreferences music;
    music.localPaths = {audioPath1_, audioPath2_};
    QVERIFY(settings->saveMusicPreferences(music).ok);
    std::unique_ptr<MusicController> controller(makeController(*settings));
    QVERIFY(controller);
    QVERIFY(controller->select(0).ok);
    QVERIFY(controller->playPause().ok);
    QVERIFY(waitForPlaying(*controller));
    QVERIFY(waitForPosition(*controller, 1));
    QVERIFY(controller->playPause().ok);
    QTest::qWait(200);
    const qint64 pausedPosition = controller->snapshot().positionMs;
    QTest::qWait(200);
    QVERIFY(qAbs(controller->snapshot().positionMs - pausedPosition) <= 30);
    QVERIFY(controller->stop().ok);
    QCOMPARE(controller->snapshot().state, PlaybackState::Stopped);
    QCOMPARE(controller->snapshot().positionMs, qint64(0));
}

void DServiceVerify::d_t03_previousNextWrap()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    MusicPreferences music;
    music.localPaths = {audioPath1_, audioPath2_};
    QVERIFY(settings->saveMusicPreferences(music).ok);
    std::unique_ptr<MusicController> controller(makeController(*settings));
    QVERIFY(controller);
    QVERIFY(controller->select(0).ok);
    QVERIFY(controller->next().ok);
    QCOMPARE(controller->snapshot().currentIndex.value_or(-1), 1);
    QVERIFY(controller->next().ok);
    QCOMPARE(controller->snapshot().currentIndex.value_or(-1), 0);
    QVERIFY(controller->previous().ok);
    QCOMPARE(controller->snapshot().currentIndex.value_or(-1), 1);
}

void DServiceVerify::d_t04_seekAndVolume()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    MusicPreferences music;
    music.localPaths = {audioPath1_, audioPath2_};
    QVERIFY(settings->saveMusicPreferences(music).ok);
    std::unique_ptr<MusicController> controller(makeController(*settings));
    QVERIFY(controller);
    QVERIFY(controller->select(0).ok);
    QVERIFY(controller->playPause().ok);
    QVERIFY(waitForPlaying(*controller));
    QTRY_VERIFY_WITH_TIMEOUT(controller->snapshot().seekable, 5000);
    const qint64 target = qMax<qint64>(1, controller->snapshot().durationMs / 2);
    QVERIFY(controller->seek(target).ok);
    QTRY_VERIFY_WITH_TIMEOUT(qAbs(controller->snapshot().positionMs - target) < 150, 3000);
    QVERIFY(controller->setVolume(0).ok);
    QCOMPARE(controller->snapshot().volume, 0);
    QVERIFY(controller->setVolume(100).ok);
    QCOMPARE(controller->snapshot().volume, 100);
}

void DServiceVerify::d_t05_badPathSingleError()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    const QString badPath = temp_.filePath(QStringLiteral("broken.wav"));
    QFile broken(badPath);
    QVERIFY(broken.open(QIODevice::WriteOnly));
    QVERIFY(broken.write("not a wav") > 0);
    broken.close();
    MusicPreferences music;
    music.localPaths = {badPath};
    QVERIFY(settings->saveMusicPreferences(music).ok);
    std::unique_ptr<MusicController> controller(makeController(*settings));
    QVERIFY(controller);
    QSignalSpy errors(controller.get(), &IMusicController::errorOccurred);
    QVERIFY(controller->select(0).ok);
    QVERIFY(controller->playPause().ok);
    QTRY_VERIFY_WITH_TIMEOUT(errors.count() >= 1, 5000);
    QTest::qWait(500);
    QCOMPARE(errors.count(), 1);
}

void DServiceVerify::d_t06_rebuildRestoresWithoutAutoplay()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    MusicPreferences music;
    music.localPaths = {audioPath1_, audioPath2_};
    music.volume = 37;
    QVERIFY(settings->saveMusicPreferences(music).ok);
    {
        std::unique_ptr<MusicController> controller(makeController(*settings));
        QVERIFY(controller);
        QCOMPARE(controller->snapshot().tracks.size(), 2);
        QVERIFY(controller->setVolume(37).ok);
    }
    std::unique_ptr<SettingsService> reopened(makeSettings());
    std::unique_ptr<MusicController> restored(makeController(*reopened));
    QVERIFY(restored);
    QCOMPARE(restored->snapshot().tracks.size(), 2);
    QCOMPARE(restored->snapshot().volume, 37);
    QCOMPARE(restored->snapshot().state, PlaybackState::Stopped);
    QVERIFY(!restored->snapshot().currentIndex.has_value());
}

void DServiceVerify::d_t07_preferencesPersist()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    Preferences preferences;
    preferences.defaultFocusMinutes = 50;
    preferences.defaultRestMinutes = 10;
    preferences.completionSoundEnabled = false;
    QVERIFY(settings->savePreferences(preferences).ok);
    QSettings ini(temp_.filePath(QStringLiteral("service.ini")), QSettings::IniFormat);
    QCOMPARE(ini.value(QStringLiteral("preferences/defaultFocusMinutes")).toInt(), 50);
    QCOMPARE(ini.value(QStringLiteral("preferences/defaultRestMinutes")).toInt(), 10);
    QVERIFY(!ini.value(QStringLiteral("preferences/completionSoundEnabled")).toBool());
    std::unique_ptr<SettingsService> reopened(makeSettings());
    QCOMPARE(reopened->preferences().defaultFocusMinutes, 50);
    QCOMPARE(reopened->preferences().defaultRestMinutes, 10);
    QVERIFY(!reopened->preferences().completionSoundEnabled);
}

void DServiceVerify::d_t09_completionSoundSwitchAndDedup()
{
    std::unique_ptr<SettingsService> settings(makeSettings());
    Preferences enabled = settings->preferences();
    enabled.completionSoundEnabled = true;
    QVERIFY(settings->savePreferences(enabled).ok);
    CompletionSound sound(*settings);
    auto* effect = sound.findChild<QSoundEffect*>();
    QVERIFY(effect);
    QSignalSpy playingChanged(effect, &QSoundEffect::playingChanged);
    sound.playForCompletedSession(QStringLiteral("session-verify"));
    QTRY_VERIFY_WITH_TIMEOUT(effect->isPlaying() || effect->status() == QSoundEffect::Ready, 3000);
    const int beforeDuplicate = playingChanged.count();
    sound.playForCompletedSession(QStringLiteral("session-verify"));
    QTest::qWait(100);
    QCOMPARE(playingChanged.count(), beforeDuplicate);

    Preferences disabled = enabled;
    disabled.completionSoundEnabled = false;
    QVERIFY(settings->savePreferences(disabled).ok);
    CompletionSound disabledSound(*settings);
    auto* disabledEffect = disabledSound.findChild<QSoundEffect*>();
    QVERIFY(disabledEffect);
    disabledSound.playForCompletedSession(QStringLiteral("session-disabled"));
    QTest::qWait(300);
    QVERIFY(!disabledEffect->isPlaying());
}
}

QTEST_GUILESS_MAIN(fg::DServiceVerify)
#include "verify_services.moc"

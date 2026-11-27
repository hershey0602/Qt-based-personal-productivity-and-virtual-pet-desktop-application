#include "CompletionSound.h"

#include <QUrl>

namespace fg {
CompletionSound::CompletionSound(ISettingsService& settings, QObject* parent)
    : ICompletionSound(parent), settings_(settings), effect_(this)
{
    effect_.setSource(QUrl(QStringLiteral("qrc:/fg/UI_ASSETS/audio/sfx_focus_complete.wav")));
    effect_.setLoopCount(1);
    effect_.setVolume(1.0);
    connect(&effect_, &QSoundEffect::statusChanged, this, [this] {
        if (effect_.status() == QSoundEffect::Error) {
            emit errorOccurred({AppErrorCode::MediaUnsupported,
                                 QStringLiteral("完成提示音加载失败"),
                                 QStringLiteral("QSoundEffect could not load the bundled completion sound")});
        }
    });
}

void CompletionSound::playForCompletedSession(const SessionId& sessionId)
{
    if (sessionId.isEmpty() || playedSessions_.contains(sessionId)) {
        return;
    }
    if (!settings_.preferences().completionSoundEnabled) {
        return;
    }
    if (effect_.status() == QSoundEffect::Error) {
        emit errorOccurred({AppErrorCode::MediaUnsupported,
                             QStringLiteral("完成提示音暂不可用"),
                             QStringLiteral("Completion sound status is Error")});
        return;
    }
    playedSessions_.insert(sessionId);
    effect_.play();
}
}

#include <QtTest>

#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>

#include "services/CompletionSound.h"
#include "services/MusicController.h"
#include "services/SettingsService.h"

namespace fg {
class DServicesTest : public QObject {
    Q_OBJECT
private slots:
    void settingsRoundTrip();
    void invalidPreferencesAreRejected();
    void musicStartsEmptyAndKeepsFiles();
    void completionSoundHonoursDisabledSetting();
};

void DServicesTest::settingsRoundTrip()
{
    QTemporaryDir temp;
    QVERIFY(temp.isValid());
    SettingsService service(temp.path());
    QVERIFY(service.load().ok);
    QCOMPARE(service.preferences().defaultFocusMinutes, 25);
    QCOMPARE(service.preferences().defaultRestMinutes, 5);
    QVERIFY(service.preferences().completionSoundEnabled);

    Preferences changed;
    changed.defaultFocusMinutes = 50;
    changed.defaultRestMinutes = 10;
    changed.completionSoundEnabled = false;
    QVERIFY(service.savePreferences(changed).ok);

    SettingsService reopened(temp.path());
    QVERIFY(reopened.load().ok);
    QCOMPARE(reopened.preferences().defaultFocusMinutes, 50);
    QCOMPARE(reopened.preferences().defaultRestMinutes, 10);
    QVERIFY(!reopened.preferences().completionSoundEnabled);
}

void DServicesTest::invalidPreferencesAreRejected()
{
    QTemporaryDir temp;
    QVERIFY(temp.isValid());
    SettingsService service(temp.path());
    QVERIFY(service.load().ok);
    Preferences invalid = service.preferences();
    invalid.defaultFocusMinutes = 30;
    const OperationResult result = service.savePreferences(invalid);
    QVERIFY(!result.ok);
    QCOMPARE(result.error.code, AppErrorCode::Validation);
    QCOMPARE(service.preferences().defaultFocusMinutes, 25);
}

void DServicesTest::musicStartsEmptyAndKeepsFiles()
{
    QTemporaryDir temp;
    QVERIFY(temp.isValid());
    SettingsService settings(temp.path());
    QVERIFY(settings.load().ok);
    MusicController controller(settings);
    QVERIFY(controller.loadPlaylist().ok);
    QVERIFY(controller.snapshot().tracks.isEmpty());
    QCOMPARE(controller.snapshot().state, PlaybackState::Stopped);
    QVERIFY(!controller.playPause().ok);

    const QString audioPath = temp.filePath(QStringLiteral("sample.wav"));
    QFile audio(audioPath);
    QVERIFY(audio.open(QIODevice::WriteOnly));
    QVERIFY(audio.write("not-a-real-media-file") > 0);
    audio.close();
    QVERIFY(controller.importFiles({audioPath}).ok);
    QCOMPARE(controller.snapshot().tracks.size(), 1);
    QVERIFY(!controller.snapshot().currentIndex.has_value());
    QVERIFY(controller.select(0).ok);
    QCOMPARE(controller.snapshot().state, PlaybackState::Stopped);
    QVERIFY(controller.remove(0).ok);
    QVERIFY(QFileInfo::exists(audioPath));
    QVERIFY(controller.snapshot().tracks.isEmpty());
}

void DServicesTest::completionSoundHonoursDisabledSetting()
{
    QTemporaryDir temp;
    QVERIFY(temp.isValid());
    SettingsService settings(temp.path());
    QVERIFY(settings.load().ok);
    Preferences preferences = settings.preferences();
    preferences.completionSoundEnabled = false;
    QVERIFY(settings.savePreferences(preferences).ok);
    CompletionSound sound(settings);
    sound.playForCompletedSession(QStringLiteral("session-1"));
    sound.playForCompletedSession(QStringLiteral("session-1"));
}
}

QTEST_MAIN(fg::DServicesTest)
#include "test_d_services.moc"

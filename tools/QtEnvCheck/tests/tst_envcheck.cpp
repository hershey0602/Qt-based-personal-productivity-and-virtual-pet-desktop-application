#include "checks.h"
#include <QSqlDatabase>
#include <QTest>
#include <QWidget>

class EnvCheckTest : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() { initializeResources(); }
    void exactQt() {
        QCOMPARE(QString::fromLatin1(QT_VERSION_STR), QStringLiteral("6.11.2"));
        QCOMPARE(QString::fromLatin1(qVersion()), QStringLiteral("6.11.2"));
        QCOMPARE(sizeof(void *), size_t(8));
    }
    void widgetLifecycle() {
        QWidget window;
        QVERIFY(!checkWidgets(window).passed);
        window.show();
        QVERIFY(QTest::qWaitForWindowExposed(&window));
        const auto result = checkWidgets(window);
        QVERIFY2(result.passed, qPrintable(result.detail));
    }
    void svgPixels() {
        QImage image;
        const auto result = checkSvg(&image);
        QVERIFY2(result.passed, qPrintable(result.detail));
        QCOMPARE(image.size(), QSize(128, 128));
        QVERIFY(image.pixelColor(64, 30).green() > 100);
    }
    void missingSvgFails() {
        const auto result = checkSvg(nullptr, QStringLiteral(":/env/missing.svg"));
        QVERIFY(!result.passed);
        QVERIFY(result.detail.contains(QStringLiteral("missing.svg")));
    }
    void sqliteRoundTripAndCleanup() {
        const auto before = QSqlDatabase::connectionNames();
        for (int i = 0; i < 2; ++i) {
            const auto result = checkSql();
            QVERIFY2(result.passed, qPrintable(result.detail));
        }
        QCOMPARE(QSqlDatabase::connectionNames(), before);
    }
    void missingDriverFails() {
        const auto result = checkSql(QStringLiteral("ENV_NONEXISTENT_DRIVER"));
        QVERIFY(!result.passed);
        QVERIFY(result.detail.contains(QStringLiteral("ENV_NONEXISTENT_DRIVER")));
    }
    void multimediaAssociation() {
        const auto result = checkMultimedia();
        QVERIFY2(result.passed, qPrintable(result.detail));
    }
};
QTEST_MAIN(EnvCheckTest)
#include "tst_envcheck.moc"

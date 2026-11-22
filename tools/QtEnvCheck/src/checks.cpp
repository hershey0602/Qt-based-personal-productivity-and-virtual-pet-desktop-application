#include "checks.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QLabel>
#include <QMediaPlayer>
#include <QPainter>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSignalSpy>
#include <QSvgRenderer>
#include <QTest>
#include <QUuid>
#include <QVBoxLayout>

void initializeResources() { Q_INIT_RESOURCE(resources); }

CheckResult checkWidgets(QWidget &window)
{
    const bool versionOk = QString::fromLatin1(qVersion()) == QStringLiteral("6.11.2")
        && QString::fromLatin1(QT_VERSION_STR) == QStringLiteral("6.11.2");
    const bool visible = window.isVisible() && window.windowHandle();
    return {QStringLiteral("Widgets"), versionOk && visible,
            !versionOk ? QStringLiteral("Qt 版本不符合 6.11.2") :
            visible ? QStringLiteral("桌面窗口已创建并显示") : QStringLiteral("窗口未显示或缺少原生窗口句柄")};
}

CheckResult checkSvg(QImage *rendered, const QString &path)
{
    QSvgRenderer renderer(path);
    if (!renderer.isValid())
        return {QStringLiteral("Svg"), false, QStringLiteral("SVG 加载失败：") + path};
    QImage image(128, 128, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    { QPainter painter(&image); renderer.render(&painter); }
    // Known sample: transparent corner and green disc away from the white tick.
    const QColor green = image.pixelColor(64, 30);
    const bool ok = green.alpha() > 240 && green.green() > green.red()
        && image.pixelColor(0, 0).alpha() == 0;
    if (rendered) *rendered = image;
    return {QStringLiteral("Svg"), ok, ok ? QStringLiteral("内置 SVG 加载、渲染及像素检查成功；左侧显示渲染结果")
                                          : QStringLiteral("SVG 渲染像素与绿色圆形样本不符")};
}

CheckResult checkSql(const QString &driver)
{
    if (!QSqlDatabase::isDriverAvailable(driver))
        return {QStringLiteral("Sql / SQLite"), false, QStringLiteral("数据库驱动不可用：") + driver};
    const QString connection = QUuid::createUuid().toString(QUuid::WithoutBraces);
    CheckResult result{QStringLiteral("Sql / SQLite"), false, {}};
    {
        auto db = QSqlDatabase::addDatabase(driver, connection);
        db.setDatabaseName(QStringLiteral(":memory:"));
        if (!db.open()) {
            result.detail = QStringLiteral("打开内存数据库失败：") + db.lastError().text();
        } else {
            QSqlQuery query(db);
            const auto execute = [&](const QString &sql, const QString &step) {
                if (query.exec(sql)) return true;
                result.detail = step + QStringLiteral("失败：") + query.lastError().text();
                return false;
            };
            if (execute(QStringLiteral("CREATE TABLE env_probe(id INTEGER PRIMARY KEY, name TEXT NOT NULL)"), QStringLiteral("建表"))) {
                query.prepare(QStringLiteral("INSERT INTO env_probe(id,name) VALUES(?,?)"));
                query.addBindValue(1);
                query.addBindValue(QStringLiteral("环境测试"));
                if (!query.exec()) {
                    result.detail = QStringLiteral("写入失败：") + query.lastError().text();
                } else if (execute(QStringLiteral("SELECT id,name FROM env_probe WHERE id=1"), QStringLiteral("查询"))) {
                    result.passed = query.next() && query.value(0).toInt() == 1
                        && query.value(1).toString() == QStringLiteral("环境测试") && !query.next();
                    result.detail = result.passed ? QStringLiteral("内存数据库建表、参数写入、条件查询及中文回读成功")
                                                  : QStringLiteral("查询结果、行数或中文值不匹配");
                }
            }
            query.finish();
            db.close();
        }
    } // Destroy queries and database handles before removing the connection.
    QSqlDatabase::removeDatabase(connection);
    return result;
}

CheckResult checkMultimedia()
{
    QAudioOutput output;
    QMediaPlayer player;
    if (!player.isAvailable())
        return {QStringLiteral("Multimedia"), false, QStringLiteral("QMediaPlayer 后端不可用：") + player.errorString()};
    player.setAudioOutput(&output);
    const bool attached = player.audioOutput() == &output;
    player.setAudioOutput(nullptr);
    const bool ok = attached && player.audioOutput() == nullptr;
    return {QStringLiteral("Multimedia"), ok, ok ? QStringLiteral("QMediaPlayer / QAudioOutput 创建、关联及解除关联成功（未执行音频播放）")
                                                  : QStringLiteral("音频输出关联或解除关联失败")};
}

class ModuleSmokeTest : public QObject {
    Q_OBJECT
private slots:
    void widgetAndSignal() {
        QLabel label;
        label.setText(QStringLiteral("Qt Test 实测"));
        QCOMPARE(label.text(), QStringLiteral("Qt Test 实测"));
        QSignalSpy spy(&label, &QLabel::linkActivated);
        QVERIFY(spy.isValid());
        QVERIFY(QMetaObject::invokeMethod(&label, "linkActivated", Q_ARG(QString, QStringLiteral("env"))));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("env"));
    }
};

CheckResult checkQtTest()
{
    ModuleSmokeTest test;
    char name[] = "QtEnvCheck.ModuleSmokeTest";
    char option[] = "-o";
    char destination[] = "-,txt";
    char *argv[] = {name, option, destination, nullptr};
    const int code = QTest::qExec(&test, 3, argv);
    return {QStringLiteral("Test"), code == 0, code == 0 ? QStringLiteral("Qt Test 真实断言与信号捕获测试通过")
        : QStringLiteral("Qt Test 执行失败，退出码 %1；查看测试输出").arg(code)};
}

#include "checks.moc"

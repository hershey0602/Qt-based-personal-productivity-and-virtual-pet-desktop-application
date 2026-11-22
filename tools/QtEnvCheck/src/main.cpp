#include "checks.h"
#include <QApplication>
#include <QDateTime>
#include <QLabel>
#include <QPixmap>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QTest>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    initializeResources();
    const bool selfTest = app.arguments().contains(QStringLiteral("--self-test"));
    QWidget window;
    window.setWindowTitle(QStringLiteral("Focus Garden · Qt 环境统一测试"));
    window.resize(1000, 540);
    auto *layout = new QVBoxLayout(&window);
    auto *version = new QLabel(QStringLiteral("编译 Qt：%1    运行 Qt：%2\n编译器：GCC %3    C++：%4    架构：%5 位")
        .arg(QString::fromLatin1(QT_VERSION_STR), QString::fromLatin1(qVersion()), QStringLiteral(__VERSION__))
        .arg(__cplusplus).arg(sizeof(void *) * 8));
    layout->addWidget(version);
    QImage svg;
    const auto svgResult = checkSvg(&svg);
    auto *preview = new QLabel;
    preview->setPixmap(QPixmap::fromImage(svg));
    layout->addWidget(preview);
    auto *table = new QTableWidget(5, 3);
    table->setHorizontalHeaderLabels({QStringLiteral("检查项"), QStringLiteral("结果"), QStringLiteral("实测说明 / 失败原因")});
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setWordWrap(true);
    layout->addWidget(table);
    auto *summary = new QLabel(QStringLiteral("正在检查……"));
    layout->addWidget(summary);
    window.show();
    QTimer::singleShot(0, &window, [&] {
        const bool exposed = QTest::qWaitForWindowExposed(&window, 5000);
        auto widgets = checkWidgets(window);
        if (!exposed) { widgets.passed = false; widgets.detail = QStringLiteral("窗口未在 5 秒内暴露到桌面"); }
        const QList<CheckResult> results{widgets, svgResult, checkSql(), checkMultimedia(), checkQtTest()};
        bool allPassed = true;
        QTextStream out(stdout);
        out << "Compile Qt=" << QT_VERSION_STR << " Runtime Qt=" << qVersion() << '\n';
        for (int row = 0; row < results.size(); ++row) {
            const auto &r = results.at(row);
            const QString status = r.passed ? QStringLiteral("PASS") : QStringLiteral("FAIL");
            table->setItem(row, 0, new QTableWidgetItem(r.name));
            auto *item = new QTableWidgetItem(status);
            item->setForeground(r.passed ? QColor(0, 120, 55) : QColor(190, 0, 0));
            table->setItem(row, 1, item);
            table->setItem(row, 2, new QTableWidgetItem(r.detail));
            out << status << " | " << r.name << " | " << r.detail << '\n';
            allPassed = allPassed && r.passed;
        }
        table->resizeRowsToContents();
        summary->setText(QStringLiteral("%1 · %2 · Multimedia 仅验证对象与关联，未测试实际播放。")
            .arg(allPassed ? QStringLiteral("PASS：5/5") : QStringLiteral("FAIL：请检查失败行"), QDateTime::currentDateTime().toString(Qt::ISODate)));
        out << "OVERALL " << (allPassed ? "PASS" : "FAIL") << Qt::endl;
        if (selfTest) QTimer::singleShot(300, &app, [&app, allPassed] { app.exit(allPassed ? 0 : 1); });
    });
    return app.exec();
}

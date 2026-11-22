#pragma once
#include <QImage>
#include <QString>

class QWidget;
struct CheckResult {
    QString name;
    bool passed;
    QString detail;
};
void initializeResources();
CheckResult checkWidgets(QWidget &window);
CheckResult checkSvg(QImage *rendered = nullptr, const QString &path = QStringLiteral(":/env/sample.svg"));
CheckResult checkSql(const QString &driver = QStringLiteral("QSQLITE"));
CheckResult checkMultimedia();
CheckResult checkQtTest();

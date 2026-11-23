#pragma once

#include <QSqlDatabase>
#include <QString>

namespace fg {

class Migrations
{
public:
    static bool apply(QSqlDatabase& database, QString* diagnostic = nullptr);
};

} // namespace fg

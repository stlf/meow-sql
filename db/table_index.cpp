#include "table_index.h"
#include <QMap>
#include <QDebug>
#include "table_column.h"
#include "entity/table_entity.h"

namespace meow {
namespace db {

TableIndexClass strToTableIndexClass(const QString & str)
{
    static QMap<QString, TableIndexClass> map = {
        { "PRIMARY",  TableIndexClass::PrimaryKey },
        { "KEY",      TableIndexClass::Key },
        { "UNIQUE",   TableIndexClass::Unique },
        { "FULLTEXT", TableIndexClass::FullText },
        { "SPATIAL",  TableIndexClass::Spatial },
    };

    QString strUp = str.toUpper();
    return map.value(strUp, TableIndexClass::None);
}

QString tableIndexClassToStr(TableIndexClass cls)
{
    switch (cls) {
    case TableIndexClass::PrimaryKey:
        return "PRIMARY";
    case TableIndexClass::Key:
        return "KEY";
    case TableIndexClass::Unique:
        return "UNIQUE";
    case TableIndexClass::FullText:
        return "FULLTEXT";
    case TableIndexClass::Spatial:
        return "SPATIAL";
    default:
        return "NONE";
    }
}

const QStringList tableIndexClassNames()
{
    static QStringList list = {
        "PRIMARY",
        "KEY",
        "UNIQUE",
        "FULLTEXT",
        "SPATIAL"
    };
    return list;
}

TableIndexType strToTableIndexType(const QString & str)
{
    static QMap<QString, TableIndexType> map = {
        { "BTREE",  TableIndexType::BTREE },
        { "HASH",   TableIndexType::HASH },
        { "RTREE",  TableIndexType::RTREE }
    };

    QString strUp = str.toUpper();
    return map.value(strUp, TableIndexType::None);
}

QString tableIndexTypeToStr(TableIndexType type)
{
    switch (type) {
    case TableIndexType::BTREE:
        return "BTREE";
    case TableIndexType::HASH:
        return "HASH";
    case TableIndexType::RTREE:
        return "RTREE";
    default:
        return "NONE";
    }
}

const QStringList tableIndexTypeNames()
{
    static QStringList list = {
        "BTREE",
        "HASH",
        "RTREE"
    };
    return list;
}

QString TableIndex::Column::name() const {
    TableColumn * column = _index->table()->structure()->columnById(_columnId);
    return column ? column->name() : QString();
}

TableIndex::TableIndex(TableEntity * table)
    :_table(table),
     _class(TableIndexClass::None),
     _type(TableIndexType::None)
{

}

TableIndex::operator QString() const
{
    QString str = QString("name:%1 class:%2")
            .arg(_name)
            .arg(tableIndexClassToStr(_class));

    if (_type != TableIndexType::None) {
        str += " type:" + tableIndexTypeToStr(_type);
    }

    if (!_columns.isEmpty()) {
        str += " columns:" + columnNames().join(',');
    }

    return str;
}

QString TableIndex::name() const
{
    if (_class == TableIndexClass::PrimaryKey) {
        return "PRIMARY KEY";
    }
    return _name;
}

int TableIndex::addColumn(const QString & name)
{
    TableColumn * column = _table->structure()->columnByName(name);
    if (column) {
        _columns.append(Column(this, column->id()));
        return _columns.size();
    }
    return -1;
}

void TableIndex::setTable(TableEntity * table)
{
    _table = table;
}

TableIndex * TableIndex::deepCopy(TableEntity * table)
{
    TableIndex * copy = new TableIndex(*this);
    copy->_table = table;
    copy->_columns.clear();

    for (auto & column : _columns) {
        copy->_columns.append(Column(copy, column.id()));
    }

    return copy;
}

} // namespace db
} // namespace meow

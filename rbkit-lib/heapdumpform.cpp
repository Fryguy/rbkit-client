#include "heapdumpform.h"
#include "ui_heapdumpform.h"
#include "dbobjstore.h"
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>

HeapDumpForm::HeapDumpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeapDumpForm)
{
    ui->setupUi(this);
}

HeapDumpForm::~HeapDumpForm()
{
    delete ui;
}
RBKit::ObjectStore HeapDumpForm::getObjectStore() const
{
    return objectStore;
}

void HeapDumpForm::setObjectStore(const RBKit::ObjectStore &value)
{
    RBKit::DbObjStore dbstore;
    dbstore.persistToDb(value);

    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("object_counts");
    model->select();
    //model->setRelation(2, QSqlRelation("objects", "id", "id"));
    //model->setRelation(3, QSqlRelation("objects", "id", "id"));

    ui->tableView->setModel(model);
}

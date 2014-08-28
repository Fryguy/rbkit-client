#include "heapdumpform.h"
#include "ui_heapdumpform.h"
#include "dbobjstore.h"

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

    // objectStore = value;
    // HeapTable *heapTable = new HeapTable(0, objectStore);
    // ui->tableView->setModel(heapTable);
}



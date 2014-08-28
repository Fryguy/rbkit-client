#include <QSqlDatabase>
#include <QTreeView>
#include "objectstore.h"
#include "rbevents.h"

namespace RBKit
{
    class DbObjStore : public QObject
    {
        Q_OBJECT

    public:
        DbObjStore(QObject *parent = 0);
        ~DbObjStore();

    public:
        void persistToDb(const RBKit::ObjectStore&);
    };
}

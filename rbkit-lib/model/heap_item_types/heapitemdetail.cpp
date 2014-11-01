#include "heapitemdetail.h"

namespace RBKit {

HeapItemDetail::HeapItemDetail(const QString _className, const QString _fileName)
    : className(_className), fileName(_fileName)
{
}

void HeapItemDetail::addParent(BaseHeapItem *parent)
{
    parents.append(parent);
}

void HeapItemDetail::populateDetails(BaseHeapItem *rootObject, BaseHeapItem *child)
{

}

} // namespace RBKit

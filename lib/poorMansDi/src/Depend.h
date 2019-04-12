#ifndef BETSHEDOES_DEPEND_H
#define BETSHEDOES_DEPEND_H

#include "Dependency.h"
#include "DependencyContainer.h"

template <typename T>
class DependentOnA : public DependencySpecificIndex<T>
{

public:
    DependentOnA ( T *explicitInstance = nullptr ) : instanceToProvide ( explicitInstance ) {}
    virtual ~DependentOnA() {}

    operator T *() {
        return instanceToProvide == nullptr ? ( T * ) DependencyContainer::dependOn ( this ) : instanceToProvide;
    }
protected:
    T *instanceToProvide;
};

template <typename T>
class DependentOn : public DependentOnA<T>
{
public:
    DependentOn ( T *explicitInstance = nullptr ) : DependentOnA<T> ( explicitInstance ) {}
    virtual ~DependentOn() {}

    operator T &() {
        return DependentOnA<T>::instanceToProvide == nullptr ? * ( T * ) DependencyContainer::dependOn ( this ) : *DependentOnA<T>::instanceToProvide;
    }
};
#endif //BETSHEDOES_DEPEND_H

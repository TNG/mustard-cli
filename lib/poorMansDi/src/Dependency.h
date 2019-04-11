#ifndef BETSHEDOES_DEPENDENCY_H
#define BETSHEDOES_DEPENDENCY_H

#include <typeinfo>
#include <typeindex>
#include <functional>

class DependencyIndex
{
public:
    virtual ~DependencyIndex() {}

    virtual std::type_index getTypeIndex() = 0;
};

template <typename T>
class DependencySpecificIndex : public DependencyIndex
{
public:
    virtual ~DependencySpecificIndex() {}

    virtual std::type_index getTypeIndex() override {
        return std::type_index ( typeid ( T ) );
    }
};

class Dependency
{
public:
    virtual ~Dependency() {}

    virtual void *create() = 0;
    virtual std::function<void ( void * ) > destroyFunction() = 0;
    virtual std::type_index getTypeIndex() = 0;
};


#endif //BETSHEDOES_DEPENDENCY_H

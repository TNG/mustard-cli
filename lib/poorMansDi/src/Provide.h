#ifndef BETSHEDOES_PROVIDE_H
#define BETSHEDOES_PROVIDE_H

#include "DependencyContainer.h"

template<typename ImplementingClass, typename Interface>
class ProvideImplementationForDependency : public Dependency
{
public:
    ProvideImplementationForDependency ( bool deleteOnExit = true ) :
        deleteOnExit ( deleteOnExit ),
        oldDependency ( DependencyContainer::possiblyGetDependencyFor ( &interfaceDependencyIndex ) ),
        oldInstance ( oldDependency == nullptr ? nullptr :
                      ( Interface * ) DependencyContainer::possiblyGetInstanceFor ( &interfaceDependencyIndex ) ) {
        DependencyContainer::provideDependency ( this );
    }


    virtual ~ProvideImplementationForDependency() {
        DependencyContainer::unregisterDependency ( this );
        if ( oldDependency != nullptr ) {
            DependencyContainer::restoreOldDependency ( oldDependency, oldInstance );
        }
    }

    virtual void *create() override {
        return dynamic_cast<Interface *> ( new ImplementingClass() );
    }

    virtual std::function<void ( void * ) > destroyFunction() override {
        if ( deleteOnExit ) {
            return [] ( void *ptr ) {
                delete ( ( ImplementingClass * ) ptr );
            };
        } else {
            return [] ( void *ptr ) {
            };
        }
    }

    virtual std::type_index getTypeIndex() override {
        return interfaceDependencyIndex.getTypeIndex();
    }

private:
    DependencySpecificIndex<Interface> interfaceDependencyIndex;
    bool deleteOnExit;
    Dependency *oldDependency;
    Interface *oldInstance;
};

template<typename T>
class ProvideDependency : public ProvideImplementationForDependency<T, T>
{
public:
    ProvideDependency ( bool deleteOnExit = true ) : ProvideImplementationForDependency<T, T> ( deleteOnExit ) { }

    virtual ~ProvideDependency() { }
};

#endif //BETSHEDOES_PROVIDE_H

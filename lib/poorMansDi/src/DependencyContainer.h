#ifndef BETSHEDOES_DEPENDENCYCONTAINER_H
#define BETSHEDOES_DEPENDENCYCONTAINER_H

#include <typeinfo>
#include <map>
#include <vector>
#include "Dependency.h"

class DependencyContainer
{
public:
    static void provideDependency ( Dependency *dependency );

    static void *dependOn ( DependencyIndex *dependency );

    static bool dependencyExists ( DependencyIndex *pDependency );

    ~DependencyContainer();

    static Dependency *possiblyGetDependencyFor ( DependencyIndex *dependencyIndex );

    static void *possiblyGetInstanceFor ( DependencyIndex *dependency );

    static void unregisterDependency ( Dependency *dependency );

    static void restoreOldDependency ( Dependency *dependency, void *instance );

private:
    class InstanceInformation
    {
    public:
        InstanceInformation ( Dependency *dependency, void *instancePtr ) :
            instancePtr ( instancePtr ),
            destructionInstruction ( dependency->destroyFunction() ) { }

        void *instancePtr;
        std::function<void ( void * ) > destructionInstruction;
    };

    inline static void storeDependencyInformation ( Dependency *dependency );

    inline static void removePossiblyExistingInstanceFromLookup ( Dependency *dependency );

    inline static void *createDependencyInstance ( const std::type_index &typeIndex );

    inline static void *getDependencyInstance ( const std::type_index &dependencyIndex );

    static std::map<std::type_index, Dependency *> dependencyInformation;
    static std::map<std::type_index, void *> quickInstanceLookup;
    static std::vector<InstanceInformation> instanceDestructionOrder;

    static void provideInstance ( Dependency *dependency, void *instance );

    void destroyInstance ( const InstanceInformation &instanceInformation ) const;
};

#endif //BETSHEDOES_DEPENDENCYCONTAINER_H

#include <stdio.h>
#include "DependencyContainer.h"
#include "Common.h"

using namespace std;

map<type_index, Dependency *> DependencyContainer::dependencyInformation;
map<type_index, void *> DependencyContainer::quickInstanceLookup;
vector<DependencyContainer::InstanceInformation> DependencyContainer::instanceDestructionOrder;

DependencyContainer containerInstance;

void DependencyContainer::provideDependency ( Dependency *dependency )
{
    storeDependencyInformation ( dependency );
    removePossiblyExistingInstanceFromLookup ( dependency );
}

void DependencyContainer::storeDependencyInformation ( Dependency *dependency )
{
    dependencyInformation[dependency->getTypeIndex()] = dependency;
}

void DependencyContainer::removePossiblyExistingInstanceFromLookup ( Dependency *dependency )
{
    auto instance = quickInstanceLookup.find ( dependency->getTypeIndex() );
    if ( instance != quickInstanceLookup.end() ) {
        quickInstanceLookup.erase ( instance );
    }
}

void *DependencyContainer::dependOn ( DependencyIndex *dependency )
{
    type_index dependencyType = dependency->getTypeIndex();
    map<type_index, Dependency *>::iterator iterator = dependencyInformation.find ( dependencyType );

    if ( iterator == dependencyInformation.end() ) {
        printf ( "Error: The requested class '%s' is not registered!\n", dependency->getTypeIndex().name() );
        return nullptr;
    }

    return getDependencyInstance ( dependencyType );
}

void *DependencyContainer::getDependencyInstance ( const type_index &typeIndex )
{
    auto dependencyInstance = quickInstanceLookup.find ( typeIndex );
    if ( dependencyInstance != quickInstanceLookup.end() ) {
        return dependencyInstance->second;
    }
    return createDependencyInstance ( typeIndex );
}

void *DependencyContainer::createDependencyInstance ( const type_index &typeIndex )
{
    Dependency *dependency = dependencyInformation[typeIndex];
    void *createdInstance = dependency->create();
    provideInstance ( dependency, createdInstance );
    instanceDestructionOrder.push_back ( InstanceInformation ( dependency, createdInstance ) );
    return createdInstance;
}

bool DependencyContainer::dependencyExists ( DependencyIndex *dependency )
{
    return dependencyInformation.find ( dependency->getTypeIndex() ) != dependencyInformation.end();
}

Dependency *DependencyContainer::possiblyGetDependencyFor ( DependencyIndex *dependencyIndex )
{
    if ( !dependencyExists ( dependencyIndex ) ) {
        return nullptr;
    }
    return dependencyInformation[dependencyIndex->getTypeIndex()];
}

void *DependencyContainer::possiblyGetInstanceFor ( DependencyIndex *dependencyIndex )
{
    auto dependency = quickInstanceLookup.find ( dependencyIndex->getTypeIndex() );
    return dependency == quickInstanceLookup.end() ? nullptr : dependency->second;
}

void DependencyContainer::restoreOldDependency ( Dependency *dependency, void *instance )
{
    if ( dependency == nullptr ) {
        return;
    }
    provideDependency ( dependency );
    provideInstance ( dependency, instance );
}

void DependencyContainer::provideInstance ( Dependency *dependency, void *instance )
{
    if ( instance != nullptr ) {
        quickInstanceLookup[dependency->getTypeIndex()] = instance;
    }
}

void DependencyContainer::unregisterDependency ( Dependency *dependency )
{
    if ( dependency == nullptr ) {
        return;
    }
    dependencyInformation.erase ( dependency->getTypeIndex() );
    quickInstanceLookup.erase ( dependency->getTypeIndex() );
}

DependencyContainer::~DependencyContainer()
{
    while ( instanceDestructionOrder.size() != 0 ) {
        destroyInstance ( instanceDestructionOrder.back() );
        instanceDestructionOrder.pop_back();
    }
}

void DependencyContainer::destroyInstance ( const DependencyContainer::InstanceInformation &instanceInformation ) const
{
    instanceInformation.destructionInstruction ( instanceInformation.instancePtr );
}
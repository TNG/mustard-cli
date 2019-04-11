//
// Created by mimgrund on 23.09.16.
//

#ifndef DEPENDENCYINJECTIONDEMO_TECHDAYCONFIGURATION_H
#define DEPENDENCYINJECTIONDEMO_TECHDAYCONFIGURATION_H


#include "CoffeeMachines/CoffeeMaker.h"
#include "CoffeeMachines/Barista.h"
#include "CoffeeMachines/KlixMachine.h"
#include "../src/Provide.h"

class TechdayConfiguration
{
public:
private:
    //Barista coffeeMaker;
    ProvideImplementationForDependency<Barista, CoffeeMaker> baristaDependency;
};


#endif //DEPENDENCYINJECTIONDEMO_TECHDAYCONFIGURATION_H

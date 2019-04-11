#ifndef DEPENDENCYINJECTIONDEMO_TECHDAY_H
#define DEPENDENCYINJECTIONDEMO_TECHDAY_H

#include "CoffeeMachines/CoffeeMaker.h"

class Techday
{
public:
    Techday ( CoffeeMaker *coffeeMaker = nullptr );

    void drinkCoffee();

private:
    CoffeeMaker *coffeeMaker;
};

#endif //DEPENDENCYINJECTIONDEMO_TECHDAY_H

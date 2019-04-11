//
// Created by mimgrund on 23.09.16.
//

#ifndef DEPENDENCYINJECTIONDEMO_COFFEEMASCHINE_H
#define DEPENDENCYINJECTIONDEMO_COFFEEMASCHINE_H
#include "Coffee/Coffee.h"

class CoffeeMaker
{
public:
    virtual Coffee *getCoffee() const = 0;
};

#endif //DEPENDENCYINJECTIONDEMO_COFFEMASCHINE_H

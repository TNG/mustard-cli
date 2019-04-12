//
// Created by mimgrund on 23.09.16.
//

#ifndef DEPENDENCYINJECTIONDEMO_BARISTA_H
#define DEPENDENCYINJECTIONDEMO_BARISTA_H

#include <iostream>
#include "CoffeeMaker.h"

class Barista : public CoffeeMaker
{

public:
    Barista() {
        std::cout << "We just created a coffeeMaker!" << std::endl;
    }
    virtual Coffee *getCoffee() const;
};


#endif //DEPENDENCYINJECTIONDEMO_BARISTA_H

#ifndef DEPENDENCYINJECTIONDEMO_KLIXMACHINE_H
#define DEPENDENCYINJECTIONDEMO_KLIXMACHINE_H

#include <iostream>
#include "CoffeeMaker.h"

class KlixMachine : public CoffeeMaker
{

public:
    KlixMachine() {
        std::cout << "We just created a KlixMachine!" << std::endl;
    }
    virtual Coffee *getCoffee() const override;
};


#endif //DEPENDENCYINJECTIONDEMO_KLIXMACHINE_H

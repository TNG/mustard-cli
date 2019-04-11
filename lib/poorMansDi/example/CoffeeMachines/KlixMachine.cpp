#include "KlixMachine.h"

Coffee *KlixMachine::getCoffee() const
{
    return new BrownWater();
}



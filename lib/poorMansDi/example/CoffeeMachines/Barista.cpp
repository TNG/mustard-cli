//
// Created by mimgrund on 23.09.16.
//

#include "Barista.h"

Coffee *Barista::getCoffee() const
{
    return new PremiumCoffee();
}


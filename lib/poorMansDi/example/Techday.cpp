//
// Created by mimgrund on 23.09.16.
//

#include <iostream>
#include "Techday.h"
#include "../src/Depend.h"

Techday::Techday ( CoffeeMaker *coffeeMaker ) :
    coffeeMaker ( DependentOnA<CoffeeMaker> ( coffeeMaker ) )
{

}

void Techday::drinkCoffee()
{
    Coffee *coffee = coffeeMaker->getCoffee();
    std::cout << coffee->getTaste() << endl;
    delete coffee;
}




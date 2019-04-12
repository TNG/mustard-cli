#include <iostream>
#include "Techday.h"
#include "CoffeeMachines/Barista.h"
#include "CoffeeMachines/KlixMachine.h"
#include "TechdayConfiguration.h"

Techday createTechday ( CoffeeMaker *coffeeMaker );

void showThatDiWorks();

void showOverrideOfConfiguration();

using namespace std;

int main()
{
    //Which coffee maschine to use is defined in techdayConfiguration:
    TechdayConfiguration techdayConfiguration;
    cout << "We are done setting up the configuration" << endl << endl;

    cout << "Drink coffee according to configuration" << endl;
    showThatDiWorks();

    cout << endl << "Drink coffee with an override" << endl;
    showOverrideOfConfiguration();

    cout << endl << "Override will be gone, existing singleton will be provided." << endl;
    showThatDiWorks();

    return 0;
}

void showThatDiWorks()
{
    Techday techday;
    cout << "Let's drink a coffee" << endl;
    techday.drinkCoffee();
}

void showOverrideOfConfiguration()
{
    ProvideImplementationForDependency<KlixMachine, CoffeeMaker> klickMaschineDependency;
    cout << "We are done setting up the configuration" << endl;
    Techday techday;
    cout << "Let's drink a coffee" << endl;
    techday.drinkCoffee();

}


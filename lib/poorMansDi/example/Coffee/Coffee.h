//
// Created by mimgrund on 23.09.16.
//

#ifndef DEPENDENCYINJECTIONDEMO_COFFEE_H
#define DEPENDENCYINJECTIONDEMO_COFFEE_H

#include <string>
using namespace std;
class Coffee
{
public:
    virtual string getTaste() const = 0;
};


class PremiumCoffee : public Coffee
{
public:
    virtual string getTaste() const {
        return "Aaah, barista Quality!";
    }
};

class BrownWater : public Coffee
{
public:
    virtual string getTaste() const {
        return "uuurgh! brown water!";
    }
};
#endif //DEPENDENCYINJECTIONDEMO_COFFEE_H

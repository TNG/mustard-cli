#ifndef MUSTARD_USERCONFIRMATION_H
#define MUSTARD_USERCONFIRMATION_H

#include <string>
#include "UserChoice.h"

using namespace std;

enum Confirmed {YES, NO};

class UserConfirmation : public UserChoice<Confirmed>
{
public:
    UserConfirmation ( const string &message ) :
        UserChoice<Confirmed> (
            message, {
        {'y', YES},
        {'n', NO}
    }
    ) {}
};

#endif //MUSTARD_USERCONFIRMATION_H

#ifndef MUSTARD_USER_H
#define MUSTARD_USER_H

#include <string>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;

struct User {
    string displayName;
    string eMail;
    static User from ( const Document::ValueType &userObject ) {
        const string displayName ( userObject["displayName"].GetString() );
        const string email ( userObject["emailAddress"].GetString() );
        return {displayName, email};
    }
};

#endif //MUSTARD_USER_H

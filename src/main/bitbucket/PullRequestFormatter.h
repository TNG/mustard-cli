#ifndef MUSTARD_PULLREQUESTFORMATTER_H
#define MUSTARD_PULLREQUESTFORMATTER_H

#include <string>
#include "model/PullRequest.h"

using namespace std;
class PullRequestFormatter
{
public:
    static string format ( const PullRequest &pullRequest );

private:
    static ostream &bold_on ( ostream &os );

    static ostream &bold_off ( ostream &os );
    static string formatUser ( const User &user );
    static string symbol ( ReviewStatus status );
};


#endif //MUSTARD_PULLREQUESTFORMATTER_H

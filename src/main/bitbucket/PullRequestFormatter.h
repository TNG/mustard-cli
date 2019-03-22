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
    static ostream &formatBold ( ostream &stream );

    static ostream &formatNormal ( ostream &stream );
    static string formatUser ( const User &user );
    static string symbol ( ReviewStatus status );
};


#endif //MUSTARD_PULLREQUESTFORMATTER_H

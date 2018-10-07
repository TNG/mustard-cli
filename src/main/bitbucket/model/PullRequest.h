#ifndef MUSTARD_PULLREQUEST_H
#define MUSTARD_PULLREQUEST_H

#include <string>
#include <vector>
#include "User.h"
#include "Reviewer.h"

using namespace std;

struct PullRequest {
    string url;
    unsigned int id;
    string title;
    string description;
    User author;
    vector<Reviewer> reviewers;
};

#endif //MUSTARD_PULLREQUEST_H

#ifndef MUSTARD_REVIEWER_H
#define MUSTARD_REVIEWER_H

#include "User.h"
#include "ReviewStatus.h"

struct Reviewer {
    User user;
    ReviewStatus status;

    static Reviewer from ( const User &user, const string &status ) {
        ReviewStatus reviewStatus =
            status == "APPROVED"
            ? APPROVED
            : status == "NEEDS_WORK"
            ? NEEDS_WORK
            : UNAPPROVED;
        return {user, reviewStatus};
    }
};

#endif //MUSTARD_REVIEWER_H

#include <sstream>
#include "PullRequestFormatter.h"

string PullRequestFormatter::format ( const PullRequest &pullRequest )
{
    stringstream ss;
    ss << " *** " << bold_on << pullRequest.title << bold_off << " *** " << endl;
    ss << pullRequest.description << endl << endl;
    ss << "author:        " << formatUser ( pullRequest.author ) << endl;
    ss << "reviewers: ";
    bool hasReviewers = false;
    for ( const auto &reviewer : pullRequest.reviewers ) {
        if ( !hasReviewers ) {
            hasReviewers = true;
        } else {
            ss << "           ";
        }
        ss  << symbol ( reviewer.status ) << " " << formatUser ( reviewer.user ) << endl;
    }
    if ( !hasReviewers ) {
        ss << "    <none>";
    }
    ss << endl << endl;
    ss << "View in BitBucket: " << pullRequest.url << endl << endl;
    return ss.str();
}

std::ostream &PullRequestFormatter::bold_on ( std::ostream &os )
{
    return os << "\e[1m";
}

std::ostream &PullRequestFormatter::bold_off ( std::ostream &os )
{
    return os << "\e[0m";
}

string PullRequestFormatter::symbol ( ReviewStatus status )
{
    switch ( status ) {
    case UNAPPROVED:
        return "[ ]";
    case APPROVED:
        return "[\033[0;32m✓\033[0m]";
    case NEEDS_WORK:
        return "[\033[0;31m✗\033[0m]";
    }
}

string PullRequestFormatter::formatUser ( const User &user )
{
    stringstream ss;
    ss << bold_on << user.displayName << bold_off << " <" <<  user.eMail << ">";
    return ss.str();
}

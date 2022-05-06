#include <sstream>
#include "PullRequestFormatter.h"
#include "../system/TextTable.h"

string PullRequestFormatter::format ( const PullRequest &pullRequest )
{
    stringstream ss;
    ss << pullRequest.fromBranch << " → " << pullRequest.toBranch << " *** "
       << formatBold << pullRequest.title << formatNormal << " *** " << endl;
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

std::ostream &PullRequestFormatter::formatBold ( std::ostream &stream )
{
    return stream << "\e[1m";
}

std::ostream &PullRequestFormatter::formatNormal ( std::ostream &stream )
{
    return stream << "\e[0m";
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
    return "";
}

string PullRequestFormatter::formatUser ( const User &user )
{
    stringstream ss;
    ss << formatBold << user.displayName << formatNormal << " <" <<  user.eMail << ">";
    return ss.str();
}

string PullRequestFormatter::shortFormat ( const vector<PullRequest> &pullRequests, function<bool ( const PullRequest & ) > highlight )
{
    TextTable textTable ( 5 );
    int applicablePr = 1;
    for ( const auto &pullRequest : pullRequests ) {
        stringstream hooks, project, title, author, fromTo;
        for ( const auto &reviewer : pullRequest.reviewers ) {
            hooks  << symbol ( reviewer.status );
        }
        if (highlight(pullRequest)) {
            project << formatBold << "[" << applicablePr++ << "] ";
        } else {
            project << formatNormal;
        }

        project << pullRequest.project << "/" << pullRequest.repoSlug << formatNormal;
        fromTo << pullRequest.fromBranch << " → " << pullRequest.toBranch;
        title <<  pullRequest.title;
        author << formatUser ( pullRequest.author );
        textTable.addRow ( {hooks.str(), project.str(), fromTo.str(), title.str(), author.str() } );
    }
    return textTable.getTable();
}

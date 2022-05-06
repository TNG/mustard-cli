#include <Depend.h>
#include <iostream>
#include "InboxWorkflow.h"
#include "../git/GitClient.h"
#include "../bitbucket/PullRequestFormatter.h"
#include "../system/UserChoice.h"
#include "../system/UserConfirmation.h"
#include "StartReviewWorkflow.h"

InboxWorkflow::InboxWorkflow ( BitBucketClient *bitBucketClient, GitClient *gitClient ) :
    bitbucketClient ( DependentOn<BitBucketClient> ( bitBucketClient ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

int InboxWorkflow::run ( int argc, const char **argv )
{
    const string repositorySlug = gitClient->getConfigValue ( "mustard.repositorySlug" );
    const string projectKey = gitClient->getConfigValue ( "mustard.projectKey" );
    auto pullRequests = bitbucketClient->getPullRequests();
    auto highlight = [repositorySlug, projectKey, *this] ( auto p ) {
        return equalsCaseInsensitive ( p.repoSlug, repositorySlug ) &&
               equalsCaseInsensitive ( p.project, projectKey );
    };

    std::map<string,PullRequest*> pullrequestsInThisProject;
    pullrequestsInThisProject["0"] = nullptr;

    std::cout << PullRequestFormatter::shortFormat ( pullRequests, highlight);

    int checkoutablePullRequestsFound = 0;
    std::for_each(pullRequests.begin(), pullRequests.end(), [highlight,&pullrequestsInThisProject,&checkoutablePullRequestsFound](auto &pr) {
        if (highlight(pr)){
            pullrequestsInThisProject[to_string(++checkoutablePullRequestsFound)] = &pr;
        }
    });
    if(pullrequestsInThisProject.size() > 0){
        PullRequest *prToCheckout = UserChoice("Type number of pr to git-checkout, 0 to quit.",
                                               pullrequestsInThisProject).askUser();
        if (prToCheckout != nullptr){
            gitClient->fetchAndCheckout(prToCheckout->fromBranch);
            if (UserConfirmation("Do you want to start reviewing?").askUser() == YES) {
                StartReviewWorkflow startReviewWorkflow;
                startReviewWorkflow.run(argc, argv);
            }
        }
    }

    return 0;
}

bool InboxWorkflow::equalsCaseInsensitive ( const string &a, const string &b )
{
    if ( a.length() != b.length() ) {
        return false;
    }
    return equal ( a.begin(), a.end(), b.begin(), [] ( auto a, auto b ) {
        return tolower ( a ) == tolower ( b );
    } );
}

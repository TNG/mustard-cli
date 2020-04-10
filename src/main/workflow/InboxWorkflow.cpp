#include <Depend.h>
#include <iostream>
#include "InboxWorkflow.h"
#include "../git/GitClient.h"
#include "../bitbucket/PullRequestFormatter.h"

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
    std::cout << PullRequestFormatter::shortFormat ( pullRequests, highlight );
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

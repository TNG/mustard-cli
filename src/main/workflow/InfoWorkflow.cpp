#include <Depend.h>
#include "InfoWorkflow.h"
#include "../bitbucket/PullRequestFormatter.h"

InfoWorkflow::InfoWorkflow ( BitBucketClient *bitBucketClient, GitClient *gitClient ) :
    bitBucketClient ( DependentOn<BitBucketClient> ( bitBucketClient ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

int InfoWorkflow::run ( int argc, const char **argv )
{
    Commitish featureCommit = gitClient->getFeatureBranchOnOrigin();
    PullRequest pullRequest = bitBucketClient->getPullRequestFor ( featureCommit );
    cout << PullRequestFormatter::format ( pullRequest );
    return 0;
}

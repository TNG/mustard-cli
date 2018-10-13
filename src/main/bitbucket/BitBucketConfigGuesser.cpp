#include <Depend.h>
#include <regex>
#include <Provide.h>
#include "BitBucketConfigGuesser.h"

ProvideDependency<BitBucketConfigGuesser> BitBucketConfigGuesserDependency;

BitBucketConfigGuesser::BitBucketConfigGuesser ( GitClient *gitClient ) :
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

void BitBucketConfigGuesser::guess()
{
    string cloneUrl = gitClient->getConfigValue ( "remote.origin.url" );
    static regex server ( "^.*(?:://|@)([^/@:]*)(?::\\d*)?/.*$" );
    static regex repoSlug ( "^.*/([^/]*)/[^/]*\\.git$" );
    static regex projectKey ( "^.*/([^/]*)\\.git$" );
    this->server = getSingleCaptureIn ( cloneUrl, server );
    this->repositorySlug = getSingleCaptureIn ( cloneUrl, repoSlug );
    this->projectKey = getSingleCaptureIn ( cloneUrl, projectKey );
}
string BitBucketConfigGuesser::getSingleCaptureIn ( const string &text, const regex &extractor ) const
{
    smatch matches;
    regex_match ( text, matches, extractor );
    if ( matches.size() != 2 ) {
        return "";
    }
    return matches[1];
}

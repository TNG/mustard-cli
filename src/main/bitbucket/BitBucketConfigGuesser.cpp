#include <Depend.h>
#include <regex>
#include <Provide.h>
#include "BitBucketConfigGuesser.h"
#include "../comments/commentState/RegexMatcher.h"

ProvideDependency<BitBucketConfigGuesser> BitBucketConfigGuesserDependency;

BitBucketConfigGuesser::BitBucketConfigGuesser ( GitClient *gitClient ) :
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

void BitBucketConfigGuesser::guess()
{
    string cloneUrl = gitClient->getConfigValue ( "remote.origin.url" );
    RegexMatcher server ( "^(?:https?|ssh)://(?:[^@]*@)?(.*)(?::\\d+|/scm)(?:/[^/]*){2}$" );
    static regex projectKey ( "^.*/([^/]*)/[^/]*\\.git$" );
    static regex repoSlug ( "^.*/([^/]*)\\.git$" );
    this->server = server.getSingleCaptureIn ( cloneUrl ).value();
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

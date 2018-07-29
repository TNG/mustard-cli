#include <cpr/cpr.h>
#include <Depend.h>
#include "BitBucketClientImpl.h"

BitBucketClientImpl::BitBucketClientImpl ( HttpClient *httpClient ) :
    httpClient ( DependentOn<HttpClient> ( httpClient ) )
{
}

Commitish BitBucketClientImpl::getPullRequestTargetFor ( const Commitish &commit )
{
    const HttpResponse pullRequest = httpClient->get ( "https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests" );
    if ( !pullRequest.successful ) {
        printf ( "Could not determine pull requests\n" );
    }
    printf ( "%d - '%s'\n",  pullRequest.httpStatus, pullRequest.body.c_str() );
    return Commitish();
}

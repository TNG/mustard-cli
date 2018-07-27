#include "BitBucketClientImpl.h"
using  namespace Pistache;

BitBucketClientImpl::BitBucketClientImpl()
{
    auto opts = Http::Client::options()
                .threads ( 1 )
                .maxConnectionsPerHost ( 8 );
    client.init ( opts );
}

Commitish BitBucketClientImpl::getPullRequestTargetFor ( const Commitish &commit )
{
    auto response = client.get ( "http://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests" )
                    .send();
    response.then ( [&] ( Http::Response response ) {
        std::cout << "Response code = " << response.code() << std::endl;
        auto body = response.body();
        if ( !body.empty() ) {
            std::cout << "Response body = " << body << std::endl;
        }
    }, Async::IgnoreException );
    Async::Barrier<Http::Response> ( response ).wait();
    return Commitish();
}

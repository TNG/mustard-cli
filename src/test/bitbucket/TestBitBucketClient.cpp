#include <gtest/gtest.h>
#include "../../main/bitbucket/BitBucketClientImpl.h"
#include "../system/HttpClientForTest.h"

using namespace testing;

class TestBitBucketClientImpl : public Test
{
public:
    TestBitBucketClientImpl() :
        client ( &httpClient ) {}

    HttpClientForTest httpClient;
    BitBucketClientImpl client;
};

TEST_F ( TestBitBucketClientImpl, Unit_ViewPullRequests )
{
    const Commitish commit ( "egal" );
    HttpResponse response = {"{}", 200, true};
    EXPECT_CALL ( httpClient, get ( StrNe ( "" ) ) ).WillOnce ( Return ( response ) );
    client.getPullRequestTargetFor ( commit );
}
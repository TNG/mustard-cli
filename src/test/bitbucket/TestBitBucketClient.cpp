#include <gtest/gtest.h>
#include "../../main/bitbucket/BitBucketClientImpl.h"
#include "../system/HttpClientForTest.h"

using namespace testing;

class BitBucketConfigurationForTest : public BitBucketConfiguration
{
public:
    const string getBitBucketEndpoint() override {
        return "testUrl";
    }
    virtual ~BitBucketConfigurationForTest() {}
};

class TestBitBucketClientImpl : public Test
{
public:
    TestBitBucketClientImpl() :
        client ( &httpClient, &config ) {}

    HttpClientForTest httpClient;
    BitBucketConfigurationForTest config;
    BitBucketClientImpl client;
};

TEST_F ( TestBitBucketClientImpl, Unit_ViewPullRequests )
{
    const Commitish commit ( "egal" );
    HttpResponse response = {"{}", 200, true};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    client.getPullRequestTargetFor ( commit );
}
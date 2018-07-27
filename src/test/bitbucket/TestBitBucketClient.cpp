#include <gtest/gtest.h>
#include "../../main/bitbucket/BitBucketClientImpl.h"

using  namespace testing;

class TestBitBucketClientImpl : public Test
{
public:
    BitBucketClientImpl client;
};

TEST_F ( TestBitBucketClientImpl, Unit_ViewPullRequests )
{
    const Commitish commit ( "egal" );
    client.getPullRequestTargetFor ( commit );
}
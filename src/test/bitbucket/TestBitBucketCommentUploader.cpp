#include <gtest/gtest.h>
#include "../system/HttpClientForTest.h"
#include "../../main/bitbucket/BitBucketCommentUploader.h"
#include "../../main/bitbucket/PullRequest.h"
#include "../../main/comments/Comments.h"


using namespace testing;

class BitBucketConfigurationForTest : public BitBucketConfiguration
{
public:
    const string getPullRequestEndpoint() override {
        return "testUrl";
    }

    virtual ~BitBucketConfigurationForTest() {}
};

class BitBucketCommentUpdaterTest : public Test
{
public:
    BitBucketCommentUpdaterTest() :
        pullRequest ( {
        "url", 1337
    } ),
    commentUploader ( pullRequest, &httpClient, &config ) {}

    PullRequest pullRequest;
    HttpClientForTest httpClient;
    BitBucketConfigurationForTest config;
    BitBucketCommentUploader commentUploader;
};

TEST_F ( BitBucketCommentUpdaterTest, Unit_uploadsJson )
{
    Comments comments ( {FileComments ( {"file", {LineComment ( 42, "the answer to everything" ) }} ) } );
    const char *expectedPostJson = "{\"text\":\"the answer to everything\",\"anchor\":{\"line\":42,\"lineType\":\"ADDED\",\"fileType\":\"TO\",\"path\":\"file\",\"srcPath\":\"file\"}}";
    EXPECT_CALL ( httpClient, post ( _, StrEq ( expectedPostJson ) ) ).WillOnce ( Return ( HttpResponse() ) );
    comments.accept ( commentUploader );
}

TEST_F ( BitBucketCommentUpdaterTest, Unit_postsToCorrectUrl )
{
    Comments comments ( {FileComments ( {"file", {LineComment ( 42, "the answer to everything" ) }} ) } );
    const char *expectedUrl = "testUrl/1337/comments";
    EXPECT_CALL ( httpClient, post ( StrEq ( expectedUrl ), _ ) ).WillOnce ( Return ( HttpResponse() ) );
    comments.accept ( commentUploader );
}

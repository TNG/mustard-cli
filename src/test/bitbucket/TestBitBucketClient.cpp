#include <gtest/gtest.h>
#include "../../main/bitbucket/BitBucketClientImpl.h"
#include "../system/HttpClientForTest.h"
#include "../../main/bitbucket/BitBucketClientException.h"
#include "../../main/bitbucket/PullRequest.h"

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
    const string responseJson = "{\"size\":1,\"limit\":25,\"isLastPage\":true,\"values\":[{\"id\":1,\"version\":0,\"title\":\"some change\",\"description\":\"Ignore this, its only testing\",\"state\":\"OPEN\",\"open\":true,\"closed\":false,\"createdDate\":1532016958766,\"updatedDate\":1532016958766,\"fromRef\":{\"id\":\"refs/heads/testPullRequest\",\"displayId\":\"testPullRequest\",\"latestCommit\":\"latestCommitId\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"toRef\":{\"id\":\"refs/heads/master\",\"displayId\":\"master\",\"latestCommit\":\"targetBranchCommit\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"locked\":false,\"author\":{\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"role\":\"AUTHOR\",\"approved\":false,\"status\":\"UNAPPROVED\"},\"reviewers\":[],\"participants\":[],\"properties\":{\"mergeResult\":{\"outcome\":\"CLEAN\",\"current\":true},\"resolvedTaskCount\":0,\"openTaskCount\":0},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests/1\"}]}}],\"start\":0}";
    const Commitish latestCommit ( "latestCommitId" ), targetBranchCommit ( "targetBranchCommit" );
    HttpResponse response = {responseJson, 200, true};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    Commitish extractedCommittish = client.getPullRequestTargetFor ( latestCommit );
    EXPECT_EQ ( targetBranchCommit, extractedCommittish );
}

TEST_F ( TestBitBucketClientImpl, Unit_getPullRequest )
{
    const string responseJson = "{\"size\":1,\"limit\":25,\"isLastPage\":true,\"values\":[{\"id\":1,\"version\":0,\"title\":\"some change\",\"description\":\"Ignore this, its only testing\",\"state\":\"OPEN\",\"open\":true,\"closed\":false,\"createdDate\":1532016958766,\"updatedDate\":1532016958766,\"fromRef\":{\"id\":\"refs/heads/testPullRequest\",\"displayId\":\"testPullRequest\",\"latestCommit\":\"latestCommitId\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"toRef\":{\"id\":\"refs/heads/master\",\"displayId\":\"master\",\"latestCommit\":\"targetBranchCommit\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"locked\":false,\"author\":{\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"role\":\"AUTHOR\",\"approved\":false,\"status\":\"UNAPPROVED\"},\"reviewers\":[],\"participants\":[],\"properties\":{\"mergeResult\":{\"outcome\":\"CLEAN\",\"current\":true},\"resolvedTaskCount\":0,\"openTaskCount\":0},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests/1\"}]}}],\"start\":0}";
    const Commitish latestCommit ( "latestCommitId" ), targetBranchCommit ( "targetBranchCommit" );
    HttpResponse response = {responseJson, 200, true};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    PullRequest pullRequest = client.getPullRequestFor ( latestCommit );
    EXPECT_STREQ ( "https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests/1", pullRequest.url.c_str() );
}

TEST_F ( TestBitBucketClientImpl, Unit_ViewPullRequests_InvalidJson_Throws )
{
    const string responseJson = "{\"size\":1,\"limit\":25,\"isLastPage\":true,\"values\":[{\"i";
    const Commitish latestCommit ( "latestCommitId" ), targetBranchCommit ( "targetBranchCommit" );
    HttpResponse response = {responseJson, 200, true};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    EXPECT_THROW ( client.getPullRequestTargetFor ( latestCommit ), BitBucketClientException );
}

TEST_F ( TestBitBucketClientImpl, Unit_ViewPullRequests_MissingField_Throws )
{
    const string responseJson = "{\"size\":1,\"limit\":25,\"isLastPage\":true,\"values\":[{\"id\":1,\"version\":0,\"title\":\"some change\",\"description\":\"Ignore this, its only testing\",\"state\":\"OPEN\",\"open\":true,\"closed\":false,\"createdDate\":1532016958766,\"updatedDate\":1532016958766,\"fromRef\":{\"id\":\"refs/heads/testPullRequest\",\"displayId\":\"testPullRequest\",\"latestCommit\":\"latestCommitId\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"missing\":{\"id\":\"refs/heads/master\",\"displayId\":\"master\",\"latestCommit\":\"targetBranchCommit\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"locked\":false,\"author\":{\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"role\":\"AUTHOR\",\"approved\":false,\"status\":\"UNAPPROVED\"},\"reviewers\":[],\"participants\":[],\"properties\":{\"mergeResult\":{\"outcome\":\"CLEAN\",\"current\":true},\"resolvedTaskCount\":0,\"openTaskCount\":0},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests/1\"}]}}],\"start\":0}";
    const Commitish latestCommit ( "latestCommitId" ), targetBranchCommit ( "targetBranchCommit" );
    HttpResponse response = {responseJson, 200, true};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    EXPECT_THROW ( client.getPullRequestTargetFor ( latestCommit ), BitBucketClientException );
}

TEST_F ( TestBitBucketClientImpl, Unit_ViewPullRequests_HttpFailure_Throws )
{
    HttpResponse response = {"", 500, false};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    EXPECT_THROW ( client.getPullRequestTargetFor ( Commitish() ), BitBucketClientException );
}

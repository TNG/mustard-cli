#include <gtest/gtest.h>
#include "../../main/bitbucket/BitBucketClientImpl.h"
#include "../system/HttpClientForTest.h"
#include "../../main/bitbucket/BitBucketClientException.h"
#include "../../main/bitbucket/model/PullRequest.h"
#include "../../main/bitbucket/PullRequestFormatter.h"
#include "../comments/CommentMatcher.h"

using namespace testing;

class BitBucketConfigurationForTest : public BitBucketConfiguration
{
public:
    Credentials getCredentials() override {
        return {"username", "pass"};
    }

    const string getPullRequestEndpoint() override {
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
    EXPECT_STREQ ( "https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests/1",
                   pullRequest.url.c_str() );
}

TEST_F ( TestBitBucketClientImpl, Unit_getPullRequest_FurtherInformationCorrect )
{
    const string responseJson = "{\"size\":1,\"limit\":25,\"isLastPage\":true,\"values\":[{\"id\":1,\"version\":0,\"title\":\"some change\",\"description\":\"Ignore this, its only testing\",\"state\":\"OPEN\",\"open\":true,\"closed\":false,\"createdDate\":1532016958766,\"updatedDate\":1532016958766,\"fromRef\":{\"id\":\"refs/heads/testPullRequest\",\"displayId\":\"testPullRequest\",\"latestCommit\":\"latestCommitId\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"toRef\":{\"id\":\"refs/heads/master\",\"displayId\":\"master\",\"latestCommit\":\"targetBranchCommit\",\"repository\":{\"slug\":\"poormansdi\",\"id\":1267,\"name\":\"poorMansDi\",\"scmId\":\"git\",\"state\":\"AVAILABLE\",\"statusMessage\":\"Available\",\"forkable\":true,\"project\":{\"key\":\"~IMGRUNDM\",\"id\":869,\"name\":\"Maximilian Imgrund\",\"type\":\"PERSONAL\",\"owner\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"public\":true,\"links\":{\"clone\":[{\"href\":\"ssh://git@bitbucket.int.tngtech.com:122/~imgrundm/poormansdi.git\",\"name\":\"ssh\"},{\"href\":\"https://imgrundm@bitbucket.int.tngtech.com/scm/~imgrundm/poormansdi.git\",\"name\":\"http\"}],\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/browse\"}]}}},\"locked\":false,\"author\":{\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"role\":\"AUTHOR\",\"approved\":false,\"status\":\"UNAPPROVED\"},\"reviewers\":[{\"user\": {\"name\": \"jcitizen\",\"emailAddress\": \"jane@example.com\",\"id\": 101,\"displayName\": \"Jane Citizen\",\"active\": true,\"slug\": \"jcitizen\",\"type\": \"NORMAL\"},\"lastReviewedCommit\": \"7549846524f8aed2bd1c0249993ae1bf9d3c9998\",\"role\": \"REVIEWER\",\"approved\": true,\"status\": \"APPROVED\"} ],\"participants\":[],\"properties\":{\"mergeResult\":{\"outcome\":\"CLEAN\",\"current\":true},\"resolvedTaskCount\":0,\"openTaskCount\":0},\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests/1\"}]}}],\"start\":0}";
    const Commitish latestCommit ( "latestCommitId" ), targetBranchCommit ( "targetBranchCommit" );
    HttpResponse response = {responseJson, 200, true};
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl" ) ) ).WillOnce ( Return ( response ) );
    PullRequest pullRequest = client.getPullRequestFor ( latestCommit );
    EXPECT_STREQ ( "some change", pullRequest.title.c_str() );
    EXPECT_STREQ ( "Ignore this, its only testing", pullRequest.description.c_str() );
    EXPECT_STREQ ( "Maximilian Imgrund", pullRequest.author.displayName.c_str() );
    EXPECT_STREQ ( "maximilian.imgrund@tngtech.com", pullRequest.author.eMail.c_str() );

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

TEST_F ( TestBitBucketClientImpl, Unit_ExtractComments )
{
    HttpResponse response = {"{\n"
                             "    \"size\": 2,\n"
                             "    \"limit\": 25,\n"
                             "    \"isLastPage\": true,\n"
                             "    \"values\": [\n"
                             "        {\n"
                             "            \"id\": 164514,\n"
                             "            \"createdDate\": 1538897102243,\n"
                             "            \"user\": {\n"
                             "                \"name\": \"imgrundm\",\n"
                             "                \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                \"id\": 1913,\n"
                             "                \"displayName\": \"Maximilian Imgrund\",\n"
                             "                \"active\": true,\n"
                             "                \"slug\": \"imgrundm\",\n"
                             "                \"type\": \"NORMAL\",\n"
                             "                \"links\": {\n"
                             "                    \"self\": [\n"
                             "                        {\n"
                             "                            \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                        }\n"
                             "                    ]\n"
                             "                }\n"
                             "            },\n"
                             "            \"action\": \"COMMENTED\",\n"
                             "            \"commentAction\": \"ADDED\",\n"
                             "            \"comment\": {\n"
                             "                \"properties\": {\n"
                             "                    \"repositoryId\": 1267\n"
                             "                },\n"
                             "                \"id\": 16388,\n"
                             "                \"version\": 2,\n"
                             "                \"text\": \"This comment neither.\",\n"
                             "                \"author\": {\n"
                             "                    \"name\": \"imgrundm\",\n"
                             "                    \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                    \"id\": 1913,\n"
                             "                    \"displayName\": \"Maximilian Imgrund\",\n"
                             "                    \"active\": true,\n"
                             "                    \"slug\": \"imgrundm\",\n"
                             "                    \"type\": \"NORMAL\",\n"
                             "                    \"links\": {\n"
                             "                        \"self\": [\n"
                             "                            {\n"
                             "                                \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                            }\n"
                             "                        ]\n"
                             "                    }\n"
                             "                },\n"
                             "                \"createdDate\": 1538897102243,\n"
                             "                \"updatedDate\": 1540543026906,\n"
                             "                \"comments\": [],\n"
                             "                \"tasks\": [],\n"
                             "                \"permittedOperations\": {\n"
                             "                    \"editable\": true,\n"
                             "                    \"deletable\": true\n"
                             "                }\n"
                             "            },\n"
                             "            \"commentAnchor\": {\n"
                             "                \"fromHash\": \"3fec5abb7b2f1d03f9a947ea71429b6bd151485a\",\n"
                             "                \"toHash\": \"25fc8f32b14d0e6d22aa702a1b08435d84e0e51e\",\n"
                             "                \"line\": 2,\n"
                             "                \"lineType\": \"ADDED\",\n"
                             "                \"fileType\": \"TO\",\n"
                             "                \"path\": \"CMakeLists.txt\",\n"
                             "                \"srcPath\": \"CMakeLists.txt\",\n"
                             "                \"diffType\": \"EFFECTIVE\",\n"
                             "                \"orphaned\": false\n"
                             "            },\n"
                             "            \"diff\": {\n"
                             "                \"source\": {\n"
                             "                    \"components\": [\n"
                             "                        \"CMakeLists.txt\"\n"
                             "                    ],\n"
                             "                    \"parent\": \"\",\n"
                             "                    \"name\": \"CMakeLists.txt\",\n"
                             "                    \"extension\": \"txt\",\n"
                             "                    \"toString\": \"CMakeLists.txt\"\n"
                             "                },\n"
                             "                \"destination\": {\n"
                             "                    \"components\": [\n"
                             "                        \"CMakeLists.txt\"\n"
                             "                    ],\n"
                             "                    \"parent\": \"\",\n"
                             "                    \"name\": \"CMakeLists.txt\",\n"
                             "                    \"extension\": \"txt\",\n"
                             "                    \"toString\": \"CMakeLists.txt\"\n"
                             "                },\n"
                             "                \"hunks\": [\n"
                             "                    {\n"
                             "                        \"sourceLine\": 1,\n"
                             "                        \"sourceSpan\": 12,\n"
                             "                        \"destinationLine\": 1,\n"
                             "                        \"destinationSpan\": 12,\n"
                             "                        \"segments\": [\n"
                             "                            {\n"
                             "                                \"type\": \"CONTEXT\",\n"
                             "                                \"lines\": [\n"
                             "                                    {\n"
                             "                                        \"destination\": 1,\n"
                             "                                        \"source\": 1,\n"
                             "                                        \"line\": \"cmake_minimum_required (VERSION 2.8.11)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    }\n"
                             "                                ],\n"
                             "                                \"truncated\": false\n"
                             "                            },\n"
                             "                            {\n"
                             "                                \"type\": \"REMOVED\",\n"
                             "                                \"lines\": [\n"
                             "                                    {\n"
                             "                                        \"destination\": 2,\n"
                             "                                        \"source\": 2,\n"
                             "                                        \"line\": \"\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    }\n"
                             "                                ],\n"
                             "                                \"truncated\": false\n"
                             "                            },\n"
                             "                            {\n"
                             "                                \"type\": \"ADDED\",\n"
                             "                                \"lines\": [\n"
                             "                                    {\n"
                             "                                        \"destination\": 2,\n"
                             "                                        \"source\": 3,\n"
                             "                                        \"line\": \"# this change makes no sense at all\",\n"
                             "                                        \"truncated\": false,\n"
                             "                                        \"commentIds\": [\n"
                             "                                            16388\n"
                             "                                        ]\n"
                             "                                    }\n"
                             "                                ],\n"
                             "                                \"truncated\": false\n"
                             "                            },\n"
                             "                            {\n"
                             "                                \"type\": \"CONTEXT\",\n"
                             "                                \"lines\": [\n"
                             "                                    {\n"
                             "                                        \"destination\": 3,\n"
                             "                                        \"source\": 3,\n"
                             "                                        \"line\": \"project (poorMansDi)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 4,\n"
                             "                                        \"source\": 4,\n"
                             "                                        \"line\": \"\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 5,\n"
                             "                                        \"source\": 5,\n"
                             "                                        \"line\": \"option (BUILD_TESTS \\\"Build tests\\\" ON)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 6,\n"
                             "                                        \"source\": 6,\n"
                             "                                        \"line\": \"option (BUILD_EXAMPLE \\\"Build example\\\" ON)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 7,\n"
                             "                                        \"source\": 7,\n"
                             "                                        \"line\": \"option (OPTIMISE_COMPILATION \\\"Enable O3 optimisation instead O0\\\" OFF)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 8,\n"
                             "                                        \"source\": 8,\n"
                             "                                        \"line\": \"option (CODE_COVERAGE \\\"Run code coverage on tests\\\" ON)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 9,\n"
                             "                                        \"source\": 9,\n"
                             "                                        \"line\": \"\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 10,\n"
                             "                                        \"source\": 10,\n"
                             "                                        \"line\": \"\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 11,\n"
                             "                                        \"source\": 11,\n"
                             "                                        \"line\": \"if(OPTIMISE_COMPILATION)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    },\n"
                             "                                    {\n"
                             "                                        \"destination\": 12,\n"
                             "                                        \"source\": 12,\n"
                             "                                        \"line\": \"    set(OPTIMISATION -O3)\",\n"
                             "                                        \"truncated\": false\n"
                             "                                    }\n"
                             "                                ],\n"
                             "                                \"truncated\": false\n"
                             "                            }\n"
                             "                        ],\n"
                             "                        \"truncated\": false\n"
                             "                    }\n"
                             "                ],\n"
                             "                \"truncated\": false,\n"
                             "                \"properties\": {\n"
                             "                    \"toHash\": \"25fc8f32b14d0e6d22aa702a1b08435d84e0e51e\",\n"
                             "                    \"current\": true,\n"
                             "                    \"fromHash\": \"3fec5abb7b2f1d03f9a947ea71429b6bd151485a\"\n"
                             "                }\n"
                             "            }\n"
                             "        },\n"
                             "        {\n"
                             "            \"id\": 153423,\n"
                             "            \"createdDate\": 1532016958806,\n"
                             "            \"user\": {\n"
                             "                \"name\": \"imgrundm\",\n"
                             "                \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                \"id\": 1913,\n"
                             "                \"displayName\": \"Maximilian Imgrund\",\n"
                             "                \"active\": true,\n"
                             "                \"slug\": \"imgrundm\",\n"
                             "                \"type\": \"NORMAL\",\n"
                             "                \"links\": {\n"
                             "                    \"self\": [\n"
                             "                        {\n"
                             "                            \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                        }\n"
                             "                    ]\n"
                             "                }\n"
                             "            },\n"
                             "            \"action\": \"OPENED\"\n"
                             "        }\n"
                             "    ],\n"
                             "    \"start\": 0\n"
                             "}", 200, true
                            };
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl/42/activities?limit=1000" ) ) ).WillOnce ( Return ( response ) );
    PullRequest pullRequest = {"", 42, "", "", {"", ""}, {}};

    Comments comments ( client.getCommentsFor ( pullRequest ) );

    CommentMatcher commentFromBitBucket ( [] ( const string & file, const LineComment & comment ) {
        return ( comment.getComment() == "This comment neither." )
               && ( comment.getLine() == 2 )
               && ( file == "CMakeLists.txt" )
               && ( comment.getAuthor() == "imgrundm" );
    } );
    comments.accept ( commentFromBitBucket );
    EXPECT_TRUE ( commentFromBitBucket.isMatching() );
}

TEST_F ( TestBitBucketClientImpl, Unit_ExtractComments_DoNotFailOnGeneralRemarks )
{
    HttpResponse response = {"{\n"
                             "    \"size\": 2,\n"
                             "    \"limit\": 25,\n"
                             "    \"isLastPage\": true,\n"
                             "    \"values\": [\n"
                             "        {\n"
                             "            \"id\": 164514,\n"
                             "            \"createdDate\": 1538897102243,\n"
                             "            \"user\": {\n"
                             "                \"name\": \"imgrundm\",\n"
                             "                \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                \"id\": 1913,\n"
                             "                \"displayName\": \"Maximilian Imgrund\",\n"
                             "                \"active\": true,\n"
                             "                \"slug\": \"imgrundm\",\n"
                             "                \"type\": \"NORMAL\",\n"
                             "                \"links\": {\n"
                             "                    \"self\": [\n"
                             "                        {\n"
                             "                            \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                        }\n"
                             "                    ]\n"
                             "                }\n"
                             "            },\n"
                             "            \"action\": \"COMMENTED\",\n"
                             "            \"commentAction\": \"ADDED\",\n"
                             "            \"comment\": {\n"
                             "                \"properties\": {\n"
                             "                    \"repositoryId\": 1267\n"
                             "                },\n"
                             "                \"id\": 16388,\n"
                             "                \"version\": 2,\n"
                             "                \"text\": \"This comment neither.\",\n"
                             "                \"author\": {\n"
                             "                    \"name\": \"imgrundm\",\n"
                             "                    \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                    \"id\": 1913,\n"
                             "                    \"displayName\": \"Maximilian Imgrund\",\n"
                             "                    \"active\": true,\n"
                             "                    \"slug\": \"imgrundm\",\n"
                             "                    \"type\": \"NORMAL\",\n"
                             "                    \"links\": {\n"
                             "                        \"self\": [\n"
                             "                            {\n"
                             "                                \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                            }\n"
                             "                        ]\n"
                             "                    }\n"
                             "                },\n"
                             "                \"createdDate\": 1538897102243,\n"
                             "                \"updatedDate\": 1540543026906,\n"
                             "                \"comments\": [],\n"
                             "                \"tasks\": [],\n"
                             "                \"permittedOperations\": {\n"
                             "                    \"editable\": true,\n"
                             "                    \"deletable\": true\n"
                             "                }\n"
                             "            }\n"
                             "        }\n"
                             "    ],\n"
                             "    \"start\": 0\n"
                             "}", 200, true
                            };
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl/42/activities?limit=1000" ) ) ).WillOnce ( Return ( response ) );
    PullRequest pullRequest = {"", 42, "", "", {"", ""}, {}};

    Comments comments ( client.getCommentsFor ( pullRequest ) );

    CommentMatcher commentFromBitBucket ( [] ( const string & file, const LineComment & comment ) {
        return true;
    } );
    comments.accept ( commentFromBitBucket );
    EXPECT_TRUE ( commentFromBitBucket.isNotMatching() );
}

TEST_F ( TestBitBucketClientImpl, Unit_ExtractComments_DoNotExtractOrphanedComments )
{
    HttpResponse response = {"{\n"
                             "    \"size\": 2,\n"
                             "    \"limit\": 25,\n"
                             "    \"isLastPage\": true,\n"
                             "    \"values\": [\n"
                             "        {\n"
                             "            \"id\": 164514,\n"
                             "            \"createdDate\": 1538897102243,\n"
                             "            \"user\": {\n"
                             "                \"name\": \"imgrundm\",\n"
                             "                \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                \"id\": 1913,\n"
                             "                \"displayName\": \"Maximilian Imgrund\",\n"
                             "                \"active\": true,\n"
                             "                \"slug\": \"imgrundm\",\n"
                             "                \"type\": \"NORMAL\",\n"
                             "                \"links\": {\n"
                             "                    \"self\": [\n"
                             "                        {\n"
                             "                            \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                        }\n"
                             "                    ]\n"
                             "                }\n"
                             "            },\n"
                             "            \"action\": \"COMMENTED\",\n"
                             "            \"commentAction\": \"ADDED\",\n"
                             "            \"comment\": {\n"
                             "                \"properties\": {\n"
                             "                    \"repositoryId\": 1267\n"
                             "                },\n"
                             "                \"id\": 16388,\n"
                             "                \"version\": 2,\n"
                             "                \"text\": \"This comment neither.\",\n"
                             "                \"author\": {\n"
                             "                    \"name\": \"imgrundm\",\n"
                             "                    \"emailAddress\": \"maximilian.imgrund@tngtech.com\",\n"
                             "                    \"id\": 1913,\n"
                             "                    \"displayName\": \"Maximilian Imgrund\",\n"
                             "                    \"active\": true,\n"
                             "                    \"slug\": \"imgrundm\",\n"
                             "                    \"type\": \"NORMAL\",\n"
                             "                    \"links\": {\n"
                             "                        \"self\": [\n"
                             "                            {\n"
                             "                                \"href\": \"https://bitbucket.int.tngtech.com/users/imgrundm\"\n"
                             "                            }\n"
                             "                        ]\n"
                             "                    }\n"
                             "                },\n"
                             "                \"createdDate\": 1538897102243,\n"
                             "                \"updatedDate\": 1540543026906,\n"
                             "                \"comments\": [],\n"
                             "                \"tasks\": [],\n"
                             "                \"permittedOperations\": {\n"
                             "                    \"editable\": true,\n"
                             "                    \"deletable\": true\n"
                             "                }\n"
                             "            },\n"
                             "            \"commentAnchor\": {\n"
                             "                \"fromHash\": \"3fec5abb7b2f1d03f9a947ea71429b6bd151485a\",\n"
                             "                \"toHash\": \"25fc8f32b14d0e6d22aa702a1b08435d84e0e51e\",\n"
                             "                \"line\": 2,\n"
                             "                \"lineType\": \"ADDED\",\n"
                             "                \"fileType\": \"TO\",\n"
                             "                \"path\": \"CMakeLists.txt\",\n"
                             "                \"srcPath\": \"CMakeLists.txt\",\n"
                             "                \"diffType\": \"EFFECTIVE\",\n"
                             "                \"orphaned\": true\n"
                             "            }\n"
                             "        }\n"
                             "    ],\n"
                             "    \"start\": 0\n"
                             "}", 200, true
                            };
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl/42/activities?limit=1000" ) ) ).WillOnce ( Return ( response ) );
    PullRequest pullRequest = {"", 42, "", "", {"", ""}, {}};

    Comments comments ( client.getCommentsFor ( pullRequest ) );

    CommentMatcher commentFromBitBucket ( [] ( const string & file, const LineComment & comment ) {
        return true;
    } );
    comments.accept ( commentFromBitBucket );
    EXPECT_TRUE ( commentFromBitBucket.isNotMatching() );
}

TEST_F ( TestBitBucketClientImpl, Unit_canProcessNestedComments )
{
    const string commentWithReply = "{\"size\":5,\"limit\":500,\"isLastPage\":true,\"values\":[{\"id\":178600,\"createdDate\":1547308569065,\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"action\":\"REVIEWED\"},{\"id\":167734,\"createdDate\":1540563061056,\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"action\":\"COMMENTED\",\"commentAction\":\"ADDED\",\"comment\":{\"properties\":{\"repositoryId\":2376},\"id\":16633,\"version\":0,\"text\":\"Fürwahr!\",\"author\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"createdDate\":1540563061056,\"updatedDate\":1540563061056,\"comments\":[{\"properties\":{\"repositoryId\":2376},\"id\":17330,\"version\":0,\"text\":\"Das glauben ja auch nur die Hühner \",\"author\":{\"name\":\"replyMan\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"createdDate\":1547308538694,\"updatedDate\":1547308538694,\"comments\":[],\"tasks\":[],\"permittedOperations\":{\"editable\":true,\"deletable\":true}}],\"tasks\":[],\"permittedOperations\":{\"editable\":true,\"deletable\":true}},\"commentAnchor\":{\"fromHash\":\"252c0d65b7ddf63c6984a72af8dea0ae05291c8e\",\"toHash\":\"6264307ceca75b19d22ed891f9e1bb23508496d3\",\"line\":3,\"lineType\":\"ADDED\",\"fileType\":\"TO\",\"path\":\"unsinn.md\",\"srcPath\":\"unsinn.md\",\"diffType\":\"EFFECTIVE\",\"orphaned\":false},\"diff\":{\"source\":{\"components\":[\"unsinn.md\"],\"parent\":\"\",\"name\":\"unsinn.md\",\"extension\":\"md\",\"toString\":\"unsinn.md\"},\"destination\":{\"components\":[\"unsinn.md\"],\"parent\":\"\",\"name\":\"unsinn.md\",\"extension\":\"md\",\"toString\":\"unsinn.md\"},\"hunks\":[{\"sourceLine\":1,\"sourceSpan\":3,\"destinationLine\":1,\"destinationSpan\":3,\"segments\":[{\"type\":\"CONTEXT\",\"lines\":[{\"destination\":1,\"source\":1,\"line\":\"#So viel Unsinn\",\"truncated\":false},{\"destination\":2,\"source\":2,\"line\":\"\",\"truncated\":false}],\"truncated\":false},{\"type\":\"REMOVED\",\"lines\":[{\"destination\":3,\"source\":3,\"line\":\"Und as nur um einen PR yu machen...\",\"truncated\":false}],\"truncated\":false},{\"type\":\"ADDED\",\"lines\":[{\"destination\":3,\"source\":4,\"line\":\"Und das nur um einen PR zu machen...\",\"truncated\":false,\"commentIds\":[16633]}],\"truncated\":false}],\"truncated\":false}],\"truncated\":false,\"properties\":{\"toHash\":\"6264307ceca75b19d22ed891f9e1bb23508496d3\",\"current\":true,\"fromHash\":\"252c0d65b7ddf63c6984a72af8dea0ae05291c8e\"}}},{\"id\":168555,\"createdDate\":1541174899515,\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"action\":\"APPROVED\"},{\"id\":168553,\"createdDate\":1541174852106,\"user\":{\"name\":\"imgrundm\",\"emailAddress\":\"maximilian.imgrund@tngtech.com\",\"id\":1913,\"displayName\":\"Maximilian Imgrund\",\"active\":true,\"slug\":\"imgrundm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/imgrundm\"}]}},\"action\":\"REVIEWED\"},{\"id\":167730,\"createdDate\":1540562816314,\"user\":{\"name\":\"bruecknm\",\"emailAddress\":\"matthias.brueckner@tngtech.com\",\"id\":2029,\"displayName\":\"Matthias Brückner\",\"active\":true,\"slug\":\"bruecknm\",\"type\":\"NORMAL\",\"links\":{\"self\":[{\"href\":\"https://bitbucket.int.tngtech.com/users/bruecknm\"}]}},\"action\":\"OPENED\"}],\"start\":0}";
    HttpResponse response = {commentWithReply, 200, true};
    cout << commentWithReply << endl;
    EXPECT_CALL ( httpClient, get ( StrEq ( "testUrl/42/activities?limit=1000" ) ) ).WillOnce ( Return ( response ) );
    PullRequest pullRequest = {"", 42, "", "", {"", ""}, {}};

    Comments comments ( client.getCommentsFor ( pullRequest ) );

    CommentMatcher commentFromBitBucket;
    commentFromBitBucket.check (
    "has reply", [] ( auto file, auto lineComment ) {
        return lineComment.getReplies().size() == 1;
    }
    );
    commentFromBitBucket.check ( "reply author", [] ( auto file, auto lineComment ) {
        return lineComment.getReplies() [0].getAuthor() == "replyMan";
    } );
    comments.accept ( commentFromBitBucket );
    EXPECT_TRUE ( commentFromBitBucket.isMatching() );
}

cmake_minimum_required(VERSION 3.10)
project(mustard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG release-1.11.0)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.7.2)
FetchContent_MakeAvailable(cpr)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
find_package(LibSecret)
if (LIBSECRET_FOUND)
    message("libsecret has been found and will persist credentials")
    add_definitions(-DWITH_LIBSECRET)
else ()
    message("libsecret not found - will not persist credentials")
endif ()

include_directories(${CPR_INCLUDE_DIRS})
include_directories(${LIBSECRET_INCLUDE_DIRS})
include_directories(lib/poorMansDi/src)

set(SOURCES
        src/main/authentication/AuthenticationProvider.h
        src/main/authentication/AuthenticationProviderImpl.cpp
        src/main/authentication/AuthenticationProviderImpl.h
        src/main/bitbucket/BitBucketClient.h
        src/main/bitbucket/BitBucketClientException.h
        src/main/bitbucket/BitBucketClientImpl.cpp
        src/main/bitbucket/BitBucketClientImpl.h
        src/main/bitbucket/BitBucketCommentUploader.cpp
        src/main/bitbucket/BitBucketCommentUploader.h
        src/main/bitbucket/BitBucketConfigGuesser.cpp
        src/main/bitbucket/BitBucketConfigGuesser.h
        src/main/bitbucket/BitBucketConfiguration.cpp
        src/main/bitbucket/BitBucketConfiguration.h
        src/main/bitbucket/PullRequestFormatter.cpp
        src/main/bitbucket/PullRequestFormatter.h
        src/main/bitbucket/model/PullRequest.h
        src/main/bitbucket/model/ReviewStatus.h
        src/main/bitbucket/model/Reviewer.h
        src/main/bitbucket/model/User.h
        src/main/comments/CommenExtractor.h
        src/main/comments/CommentAppender.cpp
        src/main/comments/CommentAppender.h
        src/main/comments/CommentConsumer.h
        src/main/comments/CommentExtractorImpl.cpp
        src/main/comments/CommentExtractorImpl.h
        src/main/comments/Comments.cpp
        src/main/comments/Comments.h
        src/main/comments/FileComments.cpp
        src/main/comments/FileComments.h
        src/main/comments/LineComment.cpp
        src/main/comments/LineComment.h
        src/main/comments/Todo.h
        src/main/comments/Tag.h
        src/main/comments/TagExtractor.cpp
        src/main/comments/TagExtractor.h
        src/main/comments/commentState/CommentState.h
        src/main/comments/commentState/CommentStateListener.h
        src/main/comments/commentState/LineClassifier.cpp
        src/main/comments/commentState/LineClassifier.h
        src/main/comments/commentState/LineConsumer.cpp
        src/main/comments/commentState/LineConsumer.h
        src/main/comments/commentState/RegexMatcher.h
        src/main/comments/commentState/lineConsumers/ContextDefinitionConsumer.cpp
        src/main/comments/commentState/lineConsumers/ContextDefinitionConsumer.h
        src/main/comments/commentState/lineConsumers/FileDefinitionConsumer.cpp
        src/main/comments/commentState/lineConsumers/FileDefinitionConsumer.h
        src/main/comments/commentState/lineConsumers/FileDiffLineConsumer.cpp
        src/main/comments/commentState/lineConsumers/FileDiffLineConsumer.h
        src/main/comments/commentState/lineConsumers/MultiLineCommentConsumer.cpp
        src/main/comments/commentState/lineConsumers/MultiLineCommentConsumer.h
        src/main/comments/commentState/states/ContextDefinitionState.cpp
        src/main/comments/commentState/states/ContextDefinitionState.h
        src/main/comments/commentState/states/DiffHeaderState.cpp
        src/main/comments/commentState/states/DiffHeaderState.h
        src/main/comments/commentState/states/FileDefinitionState.cpp
        src/main/comments/commentState/states/FileDefinitionState.h
        src/main/comments/commentState/states/FileDiffState.cpp
        src/main/comments/commentState/states/FileDiffState.h
        src/main/comments/commentState/states/MultiLineCommentState.cpp
        src/main/comments/commentState/states/MultiLineCommentState.h
        src/main/credentials/CredentialProvider.h
        src/main/credentials/LibSecretCredentialProvider.cpp
        src/main/credentials/LibSecretCredentialProvider.h
        src/main/error/MustardException.h
        src/main/git/Commitish.h
        src/main/git/GitClient.h
        src/main/git/GitClientException.h
        src/main/git/GitClientImpl.cpp
        src/main/git/GitClientImpl.h
        src/main/system/CommandResult.cpp
        src/main/system/CommandResult.h
        src/main/system/CommandRunner.h
        src/main/system/HttpClient.cpp
        src/main/system/HttpClient.h
        src/main/system/HttpResponse.h
        src/main/system/LinuxCommandRunner.cpp
        src/main/system/LinuxCommandRunner.h
        src/main/system/TextTable.cpp
        src/main/system/TextTable.h
        src/main/system/UserChoice.h
        src/main/system/UserConfirmation.h
        src/main/workflow/AutoconfWorkflow.cpp
        src/main/workflow/AutoconfWorkflow.h
        src/main/workflow/CommandlineConfiguration.cpp
        src/main/workflow/CommandlineConfiguration.h
        src/main/workflow/InboxWorkflow.cpp
        src/main/workflow/InboxWorkflow.h
        src/main/workflow/InfoWorkflow.cpp
        src/main/workflow/InfoWorkflow.h
        src/main/workflow/PasswdWorkflow.cpp
        src/main/workflow/PasswdWorkflow.h
        src/main/workflow/StartReviewWorkflow.cpp
        src/main/workflow/StartReviewWorkflow.h
        src/main/workflow/StopReviewWorkflow.cpp
        src/main/workflow/StopReviewWorkflow.h
        src/main/workflow/Workflow.h
        src/main/workflow/WorkflowException.h
        src/main/workflow/WorkflowFactory.cpp
        src/main/workflow/WorkflowFactory.h
        )

add_executable(mustard lib/poorMansDi/src/DependencyContainer.cpp ${SOURCES} src/main/main.cpp)
target_link_libraries(mustard cpr::cpr ${LIBSECRET_LIBRARIES})
target_compile_features(mustard PUBLIC cxx_std_17)

set(TEST_SOURCES
        ${SOURCES}
        src/test/bitbucket/TestBitBucketClient.cpp
        src/test/bitbucket/TestBitBucketCommentUploader.cpp
        src/test/bitbucket/TestBitbucketConfigGuesser.cpp
        src/test/comments/CommentMatcher.cpp
        src/test/comments/CommentMatcher.h
        src/test/comments/GitClientForTest.h
        src/test/comments/TestCommentAppender.cpp
        src/test/comments/TestCommentExtractor.cpp
        src/test/comments/TestTagExtractor.cpp
        src/test/comments/commentState/lineConsumers/testMultiLineCommentConsumer.cpp
        src/test/comments/commentState/TestLineClassifier.cpp
        src/test/git/GitTestEnvironment.cpp
        src/test/git/GitTestEnvironment.h
        src/test/git/TestGitClientImpl.cpp
        src/test/system/HttpClientForTest.h
        src/test/system/TestLinuxCommandRunner.cpp
        src/main/workflow/InboxWorkflow.cpp src/main/workflow/InboxWorkflow.h src/main/system/TextTable.cpp src/main/system/TextTable.h)

add_executable(mustard_tests lib/poorMansDi/src/DependencyContainer.cpp ${TEST_SOURCES} src/test/main.cpp)
target_link_libraries(mustard_tests cpr::cpr gtest gmock ${LIBSECRET_LIBRARIES})
target_compile_features(mustard_tests PUBLIC cxx_std_17)

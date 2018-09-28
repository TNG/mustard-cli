cmake_minimum_required(VERSION 3.10)
project(mustard)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/cpr)
add_subdirectory(lib/gmock-1.7.0)

include_directories(${CPR_INCLUDE_DIRS})
include_directories(lib/poorMansDi/src)

set(SOURCES
        src/main/authentication/AuthenticationProvider.h
        src/main/authentication/AuthenticationProviderImpl.cpp
        src/main/authentication/AuthenticationProviderImpl.h
        src/main/bitbucket/BitBucketClientException.h
        src/main/bitbucket/BitBucketClient.h
        src/main/bitbucket/BitBucketClientImpl.cpp
        src/main/bitbucket/BitBucketClientImpl.h
        src/main/bitbucket/BitBucketConfiguration.cpp
        src/main/bitbucket/BitBucketConfiguration.h
        src/main/comments/CommenExtractor.h
        src/main/comments/CommentConsumer.h
        src/main/comments/CommentExtractorImpl.cpp
        src/main/comments/CommentExtractorImpl.h
        src/main/comments/Comments.cpp
        src/main/comments/Comments.h
        src/main/comments/FileComments.cpp
        src/main/comments/FileComments.h
        src/main/comments/LineComment.cpp
        src/main/comments/LineComment.h
        src/main/error/MustardException.h
        src/main/git/Commitish.h
        src/main/git/GitClientException.h
        src/main/git/GitClient.h
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
        src/main/workflow/StartReviewWorkflow.cpp
        src/main/workflow/StartReviewWorkflow.h
        src/main/workflow/StopReviewWorkflow.cpp
        src/main/workflow/StopReviewWorkflow.h
        src/main/workflow/WorkflowException.h
        src/main/workflow/WorkflowFactory.cpp
        src/main/workflow/WorkflowFactory.h
        src/main/workflow/Workflow.h
        )

add_executable(mustard lib/poorMansDi/src/DependencyContainer.cpp ${SOURCES} src/main/main.cpp)
target_link_libraries(mustard cpr)

set(TEST_SOURCES
        ${SOURCES}
        src/test/bitbucket/TestBitBucketClient.cpp
        src/test/comments/GitClientForTest.h
        src/test/comments/TestCommentExtractor.cpp
        src/test/git/GitTestEnvironment.cpp
        src/test/git/GitTestEnvironment.h
        src/test/git/TestGitClientImpl.cpp
        src/test/system/HttpClientForTest.h
        src/test/system/TestLinuxCommandRunner.cpp
        )

add_executable(mustard_tests lib/poorMansDi/src/DependencyContainer.cpp ${TEST_SOURCES} src/test/main.cpp)
target_link_libraries(mustard_tests ${CPR_LIBRARIES} gtest gmock)


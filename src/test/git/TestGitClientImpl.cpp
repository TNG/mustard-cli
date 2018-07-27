#include <gtest/gtest.h>
#include "../../main/git/GitClientImpl.h"

using  namespace testing;

class TestGitClientImpl: public Test
{
public:
    GitClientImpl gitClient;
};

TEST_F ( TestGitClientImpl, Unit_GetHeadCommit )
{
    const Commitish commitish = gitClient.getHeadCommit();
    cout << commitish << endl;
}
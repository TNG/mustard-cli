#include <gtest/gtest.h>
#include "../../../../main/comments/commentState/lineConsumers/MultiLineCommentConsumer.h"

using namespace testing;
class TestMultiLineCommentConsumer: public Test
{
public:
    MultiLineCommentConsumer multiLineCommentConsumer = MultiLineCommentConsumer ( &testListener );
private:
    class TestCommentListener : public CommentStateListener
    {
    public:
        void newFile ( const string &fileName ) override {}
        void newLine() override {}
        void newPhantomLine() override {}
        void newComment ( const string &author, const string &comment, optional<unsigned long> id, optional<unsigned long> replyToId ) override {}
        void withTodos ( std::vector<Todo> &todos ) {}
        void setLine ( int i ) override {}
    };
    TestCommentListener testListener;

};

TEST_F ( TestMultiLineCommentConsumer, Unit_DoesNotCrashOnNoId )
{
    multiLineCommentConsumer.consume ( "+/*~author~ blubb" );
    ASSERT_FALSE ( multiLineCommentConsumer.getId().has_value() );
}

TEST_F ( TestMultiLineCommentConsumer, Unit_DoesNotCrashOnMalformedId )
{
    multiLineCommentConsumer.consume ( "+/*~author@asdoinoinas~ blubb" );
    ASSERT_FALSE ( multiLineCommentConsumer.getId().has_value() );
}

TEST_F ( TestMultiLineCommentConsumer, Unit_extractsId )
{
    multiLineCommentConsumer.consume ( "+/*~@author(author) @id(1234321)~ blubb" );
    ASSERT_TRUE ( multiLineCommentConsumer.getId().has_value() );
    ASSERT_EQ ( 1234321L, multiLineCommentConsumer.getId().value() );
}

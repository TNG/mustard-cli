#include <gtest/gtest.h>
#include <utility>
#include <functional>
#include "GitClientForTest.h"
#include "../../main/comments/CommentExtractorImpl.h"
#include "../../main/comments/CommentConsumer.h"
#include "../../main/comments/Comments.h"

using namespace testing;

class CommentExtractorTest : public Test
{
public:
    CommentExtractorTest() : commentExtractor ( &gitClient ) {}

    GitClientForTest gitClient;
    CommentExtractorImpl commentExtractor;
};

class CommentMatcher : public CommentConsumer
{
public:
    CommentMatcher ( std::function<bool ( const string &file, unsigned int line, const string &comment ) > predicate ) :
        predicate ( std::move ( predicate ) ) {};

    void consume ( const string &file, unsigned int line, const string &comment ) override {
        matched |= predicate ( file, line, comment );
    }

    bool isMatching() {
        return matched ;
    };

private:
    std::function<bool ( const string &file, unsigned int line, const string &comment ) > predicate;
    bool matched = false;
};

TEST_F ( CommentExtractorTest, Unit_TestSimpleExtraction )
{
    const string diff = "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
                        "index 7f8b793..0101eb2 100644\n"
                        "--- a/subdir/subsubdir/file.txt\n"
                        "+++ b/subdir/subsubdir/file.txt\n"
                        "@@ -4 +4 @@ File with line 3\n"
                        "-File with line 4\n"
                        "+File with line 4 //Nice line\n"
                        "@@ -18 +18 @@ File with line 17\n"
                        "-File with line 18\n"
                        "+File with line 18 //Not so good";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diff ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineFour ( [] ( const string & file, unsigned int line, const string & comment ) {
        return ( comment == "Nice line" )
               && ( line == 4 )
               && ( file == "subdir/subsubdir/file.txt" );
    } );
    CommentMatcher commentInLineEightteen ( [] ( const string & file, unsigned int line, const string & comment ) {
        return ( comment == "Not so good" )
               && ( line == 18 )
               && ( file == "subdir/subsubdir/file.txt" );
    } );
    resultingComments.accept ( commentInLineFour );
    resultingComments.accept ( commentInLineEightteen );
    EXPECT_TRUE ( commentInLineFour.isMatching() );
    EXPECT_TRUE ( commentInLineEightteen.isMatching() );
}
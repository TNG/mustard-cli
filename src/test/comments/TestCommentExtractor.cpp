#include <gtest/gtest.h>
#include <utility>
#include <functional>
#include "GitClientForTest.h"
#include "../../main/comments/CommentExtractorImpl.h"
#include "../../main/comments/CommentConsumer.h"
#include "../../main/comments/Comments.h"
#include "CommentMatcher.h"

using namespace testing;

class CommentExtractorTest : public Test
{
public:
    CommentExtractorTest() : commentExtractor ( &gitClient ) {}

    GitClientForTest gitClient;
    CommentExtractorImpl commentExtractor;
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

    CommentMatcher commentInLineFour ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getComment() == "Nice line" )
               && ( lineComment.getLine() == 4 )
               && ( file == "subdir/subsubdir/file.txt" );
    } );
    CommentMatcher commentInLineEightteen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getComment() == "Not so good" )
               && ( lineComment.getLine() == 18 )
               && ( file == "subdir/subsubdir/file.txt" );
    } );
    resultingComments.accept ( commentInLineFour );
    resultingComments.accept ( commentInLineEightteen );
    EXPECT_TRUE ( commentInLineFour.isMatching() );
    EXPECT_TRUE ( commentInLineEightteen.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_DoNotCaptureAutoInsertedComments )
{
    const string diff = "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
                        "index 7f8b793..0101eb2 100644\n"
                        "--- a/subdir/subsubdir/file.txt\n"
                        "+++ b/subdir/subsubdir/file.txt\n"
                        "@@ -4 +4 @@ File with line 3\n"
                        "-File with line 4\n"
                        "+File with line 4 //~imgrundm - Nice line\n";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diff ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineFour ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 4 )
               && ( file == "subdir/subsubdir/file.txt" );
    } );
    resultingComments.accept ( commentInLineFour );
    EXPECT_FALSE ( commentInLineFour.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_DoCaptureCommentsAfterAutoinsertedComments )
{
    const string diff = "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
                        "index 7f8b793..0101eb2 100644\n"
                        "--- a/subdir/subsubdir/file.txt\n"
                        "+++ b/subdir/subsubdir/file.txt\n"
                        "@@ -4 +4 @@ File with line 3\n"
                        "-File with line 4\n"
                        "+File with line 4 //~imgrundm - Nice line//indeed!\n";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diff ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineFour ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 4 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "indeed!" );
    } );
    resultingComments.accept ( commentInLineFour );
    EXPECT_TRUE ( commentInLineFour.isMatching() );
}

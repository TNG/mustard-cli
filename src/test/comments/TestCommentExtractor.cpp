#include <gtest/gtest.h>
#include <gmock/gmock.h>
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
                        "+File with line 4 //~Nice line\n"
                        "@@ -18 +18 @@ File with line 17\n"
                        "-File with line 18\n"
                        "+File with line 18 //~ Not so good";
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
                        "+File with line 4 //~@author(imgrundm)~ - Nice line\n";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diff ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineFour ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 4 )
               && ( file == "subdir/subsubdir/file.txt" );
    } );
    resultingComments.accept ( commentInLineFour );
    EXPECT_TRUE ( commentInLineFour.isNotMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_DoCaptureCommentsAfterAutoinsertedComments )
{
    const string diff = "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
                        "index 7f8b793..0101eb2 100644\n"
                        "--- a/subdir/subsubdir/file.txt\n"
                        "+++ b/subdir/subsubdir/file.txt\n"
                        "@@ -4 +4 @@ File with line 3\n"
                        "-File with line 4\n"
                        "+File with line 4 //~@author(imgrundm)~ Nice line//~indeed!\n";
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

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanCopeWithMultiLineComments )
{
    const string diffWithMultiLineComment = "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
                                            "index 7f8b793..280de3e 100644\n"
                                            "--- a/subdir/subsubdir/file.txt\n"
                                            "+++ b/subdir/subsubdir/file.txt\n"
                                            "@@ -8,6 +8,9 @@ File with line 7\n"
                                            " File with line 8\n"
                                            " File with line 9\n"
                                            " File with line 10\n"
                                            "+/*~ This line ten is wrong for two reasons:\n"
                                            "+ * it should have been 9\n"
                                            "+ * it should not have been 11-1*/\n"
                                            " File with line 11\n"
                                            " File with line 12\n"
                                            " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineFour ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 10 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "This line ten is wrong for two reasons:\n"
                    "it should have been 9\n"
                    "it should not have been 11-1" );
    } );
    resultingComments.accept ( commentInLineFour );
    EXPECT_TRUE ( commentInLineFour.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanCopeWithIndentedMultiLineComments )
{
    const string diffWithMultiLineComment = "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
                                            "index 7f8b793..280de3e 100644\n"
                                            "--- a/subdir/subsubdir/file.txt\n"
                                            "+++ b/subdir/subsubdir/file.txt\n"
                                            "@@ -8,6 +8,9 @@ File with line 7\n"
                                            " File with line 8\n"
                                            " File with line 9\n"
                                            " File with line 10\n"
                                            "+    /*~ This line ten is wrong for two reasons:*/\n"
                                            " File with line 11\n"
                                            " File with line 12\n"
                                            " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineFour ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 10 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "This line ten is wrong for two reasons:" );
    } );
    resultingComments.accept ( commentInLineFour );
    EXPECT_TRUE ( commentInLineFour.isMatching() );
}


TEST_F ( CommentExtractorTest, Unit_TestExtraction_MultiLineCommentsMixedWithNormalCommentsAreFine )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..ecdfbad 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -8,11 +8,18 @@ File with line 7\n"
        " File with line 8\n"
        " File with line 9\n"
        " File with line 10\n"
        "+      /*~ This line ten is wrong for two reasons:\n"
        "+    * it should have been 9\n"
        "+    * it should not have been 11-1*/\n"
        " File with line 11\n"
        " File with line 12\n"
        "-File with line 13\n"
        "+File with line 13 //~Comment on line 13\n"
        " File with line 14\n"
        " File with line 15\n"
        "+/*~ line 15 is\n"
        "+completely\n"
        "+dull\n"
        "+ * mistake*/\n"
        " File with line 16\n"
        " File with line 17\n"
        " File with line 18";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineTen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 10 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "This line ten is wrong for two reasons:\n"
                    "it should have been 9\n"
                    "it should not have been 11-1" );
    } );

    CommentMatcher commentInLineThirteen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 13 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "Comment on line 13" );
    } );

    CommentMatcher commentInLineFiveteen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 15 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "line 15 is\n"
                    "completely\n"
                    "dull\n"
                    "mistake" );
    } );
    resultingComments.accept ( commentInLineTen );
    resultingComments.accept ( commentInLineThirteen );
    resultingComments.accept ( commentInLineFiveteen );
    EXPECT_TRUE ( commentInLineTen.isMatching() );
    EXPECT_TRUE ( commentInLineThirteen.isMatching() );
    EXPECT_TRUE ( commentInLineFiveteen.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_MultiLineCommentsSplitOverContexts )
{
    const string diffWithMultiLineComment = R"(diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt
index 7f8b793..c85a0c3 100644
--- a/subdir/subsubdir/file.txt
+++ b/subdir/subsubdir/file.txt
@@ -1,5 +1,6 @@
 File with line 1
 File with line 2
+/*~ comment on 2*/
 File with line 3
 File with line 4
 File with line 5
@@ -10,6 +11,8 @@ File with line 9
 File with line 10
 File with line 11
 File with line 12
+/*~
+ * comment on 12*/
 File with line 13
 File with line 14
 File with line 15
@@ -17,4 +20,5 @@ File with line 16
 File with line 17
 File with line 18
 File with line 19
+/*~ comment on 19*/
)";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLine2 ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 2 )
               && ( lineComment.getComment() == "comment on 2" );
    } );

    CommentMatcher commentInLine12 ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 12 )
               && ( lineComment.getComment() == "comment on 12" );
    } );

    CommentMatcher commentInLine19 ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 19 )
               && ( lineComment.getComment() == "comment on 19" );
    } );

    resultingComments.accept ( commentInLine2 );
    resultingComments.accept ( commentInLine12 );
    resultingComments.accept ( commentInLine19 );
    EXPECT_TRUE ( commentInLine2.isMatching() );
    EXPECT_TRUE ( commentInLine12.isMatching() );
    EXPECT_TRUE ( commentInLine19.isMatching() );
}


TEST_F ( CommentExtractorTest, Unit_TestExtraction_TwoMultinlineCommentsOnSameLineAreFine )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..64adcc1 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -8,6 +8,11 @@ File with line 7\n"
        " File with line 8\n"
        " File with line 9\n"
        " File with line 10\n"
        "+/*~ This line ten is wrong for two reasons:\n"
        "+ * it should have been 9\n"
        "+ * it should not have been 11-1*/\n"
        "+/*~ Additionally,\n"
        "+ * we do not like comments.*/\n"
        " File with line 11\n"
        " File with line 12\n"
        " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineTen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 10 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "This line ten is wrong for two reasons:\n"
                    "it should have been 9\n"
                    "it should not have been 11-1" );
    } );
    CommentMatcher anotherCommentInLineTen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 10 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "Additionally,\n"
                    "we do not like comments." );
    } );
    resultingComments.accept ( commentInLineTen );
    resultingComments.accept ( anotherCommentInLineTen );
    EXPECT_TRUE ( commentInLineTen.isMatching() );
    EXPECT_TRUE ( anotherCommentInLineTen.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CommentsAtEndOfDiffFileWithoutContext )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..64adcc1 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -10,0 +11,5 @@ File with line 10\n"
        "+/*~ This line ten is wrong for two reasons:\n"
        "+ * it should have been 9\n"
        "+ * it should not have been 11-1*/\n"
        "+/*~ Additionally,\n"
        "+ * we do not like Comments.*/";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineTen;
    commentInLineTen.check ( "line number is 10",
    [] ( const auto & file, const auto & lineComment ) {
        return lineComment.getLine() == 10;
    } );
    commentInLineTen.check ( "file is correct",
    [] ( const auto & file, const auto & lineComment ) {
        return file == "subdir/subsubdir/file.txt";
    } );
    commentInLineTen.check ( "comment is correct",
    [] ( const auto & file, const auto & lineComment ) {
        return lineComment.getComment() == "This line ten is wrong for two reasons:\n"
               "it should have been 9\n"
               "it should not have been 11-1";
    } );


    CommentMatcher anotherCommentInLineTen;
    anotherCommentInLineTen.check ( "line number is 10",
    [] ( const auto & file, const auto & lineComment ) {
        return lineComment.getLine() == 10;
    } );
    anotherCommentInLineTen.check ( "file is correct",
    [] ( const auto & file, const auto & lineComment ) {
        return file == "subdir/subsubdir/file.txt";
    } );
    anotherCommentInLineTen.check ( "comment is correct",
    [] ( const auto & file, const auto & lineComment ) {
        return lineComment.getComment() == "Additionally,\n"
               "we do not like Comments.";
    } );
    resultingComments.accept ( commentInLineTen );
    resultingComments.accept ( anotherCommentInLineTen );
    EXPECT_TRUE ( commentInLineTen.isMatching() );
    EXPECT_TRUE ( anotherCommentInLineTen.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_DoesTakeForeignMultilineComments )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..8fff734 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -10,0 +11,2 @@ File with line 10\n"
        "+/*~author~"
        "+ * This comment\n"
        "+ * has been made by someone else */";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    EXPECT_FALSE ( resultingComments.isEmpty() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_MultilineComments_EmptyLineIsIgnored )
{
    const string diffWithMultiLineComment =
            "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
            "index 7f8b793..8fff734 100644\n"
            "--- a/subdir/subsubdir/file.txt\n"
            "+++ b/subdir/subsubdir/file.txt\n"
            "@@ -10,0 +11,2 @@ File with line 10\n"
            "+/*~ This comment\n"
            "+ *\n"
            "+ * has been made by someone\n"
            "+ *\n"
            "+ */\n"
            "context line after comment\n";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    CommentMatcher matcher;
    matcher.check ( "comment content",
                    [] ( auto file, auto lineComment ) {
                        return lineComment.getComment() == "This comment\nhas been made by someone";
                    } );
    resultingComments.accept(matcher);
    EXPECT_TRUE (matcher.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_DoesNotTakeForeignSinglelineComments )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..194ef41 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -10 +10 @@ File with line 9\n"
        "-File with line 10\n"
        "+File with line 10//~author~ foreign comment";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    EXPECT_TRUE ( resultingComments.isEmpty() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanExtractReplies )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..cdc23e0 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -8,6 +8,9 @@ File with line 7\n"
        " File with line 8\n"
        " File with line 9\n"
        " File with line 10\n"
        "+/*~@author(author) @id(42)~\n"
        "+ * foreign comment\n"
        "+ * @reply very interesting comment*/\n"
        " File with line 11\n"
        " File with line 12\n"
        " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    CommentMatcher matcher;
    matcher.check ( "replyId", [] ( auto file, auto lineComment ) {
        return lineComment.getId() == 42;
    } );
    matcher.check ( "comment content",
    [] ( auto file, auto lineComment ) {
        return lineComment.getReplies()[0].getComment() == "very interesting comment";
    } );
    resultingComments.accept ( matcher );
    EXPECT_TRUE ( matcher.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanExtractRepliesMultiLined )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..cdc23e0 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -8,6 +8,9 @@ File with line 7\n"
        " File with line 8\n"
        " File with line 9\n"
        " File with line 10\n"
        "+/*~@author(author) @id(42)~\n"
        "+ * foreign comment\n"
        "+ * @reply\n"
        "+ * and so trivial indeed.*/\n"
        " File with line 11\n"
        " File with line 12\n"
        " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    CommentMatcher matcher;
    matcher.check ( "replyId", [] ( auto file, auto lineComment ) {
        return lineComment.getId() == 42;
    } );
    matcher.check ( "comment content",
    [] ( auto file, auto lineComment ) {
        return lineComment.getReplies()[0].getComment() == "and so trivial indeed.";
    } );
    resultingComments.accept ( matcher );
    EXPECT_TRUE ( matcher.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanAnswerToReplies )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..cdc23e0 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -8,6 +8,9 @@ File with line 7\n"
        " File with line 8\n"
        " File with line 9\n"
        " File with line 10\n"
        "+/*~@author(imgrundm) @id(17365)~\n"
        "+ * der folgende Kommentar ist viel zu lang!\n"
        "+ *        ~@author(imgrundm) @id(17366) @inReplyTo(17365)~\n"
        "+ *         finde ich nicht!\n"
        "+ * @reply doch, ist so. */\n"
        " File with line 11\n"
        " File with line 12\n"
        " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    CommentMatcher matcher;
    matcher.check ( "reply content",
    [] ( auto file, auto lineComment ) {
        return lineComment.getReplies()[0].getReplies()[0].getComment() == "doch, ist so. ";
    } );
    matcher.check ( "reply id",
    [] ( auto file, auto lineComment ) {
        return lineComment.getReplies()[0].getId().value() == 17366;
    } );
    resultingComments.accept ( matcher );
    EXPECT_TRUE ( matcher.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanAnswerToRepliesInMiddleOfSection )
{
    const string diffWithMultiLineComment =
        "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
        "index 7f8b793..cdc23e0 100644\n"
        "--- a/subdir/subsubdir/file.txt\n"
        "+++ b/subdir/subsubdir/file.txt\n"
        "@@ -8,6 +8,9 @@ File with line 7\n"
        " File with line 8\n"
        " File with line 9\n"
        " File with line 10\n"
        "+/*~author(imgrundm) @id(17365)~\n"
        "+ * der folgende Kommentar ist viel zu lang!\n"
        "+ *        ~@author(imgrundm) @id(17366) @inReplyTo(17365)~\n"
        "+ *         finde ich nicht!\n"
        "+ *         @reply und wie!\n"
        "+ *        ~@author(imgrundm) @id(17367) @inReplyTo(17365)~\n"
        "+ *         doch ist er!\n"
        "+ *         Du kannst ja auch nochmal im style nachschlagen. */\n"
        " File with line 11\n"
        " File with line 12\n"
        " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    CommentMatcher matcher;
    matcher.check ( "reply content",
    [] ( auto file, auto lineComment ) {
        return lineComment.getReplies().size() == 2 &&
        lineComment.getReplies()[0].getReplies()[0].getComment() == "und wie!";
    } );
    matcher.check ( "id",
    [] ( auto file, auto lineComment ) {
        return lineComment.getId().value() == 17365;
    } );
    resultingComments.accept ( matcher );
    EXPECT_TRUE ( matcher.isMatching() );
}

TEST_F(CommentExtractorTest, Unit_FullRepliesWithNonAlphabetUserNames){
    const string diffWithMultiLineComment =
            "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
            "index 7f8b793..cdc23e0 100644\n"
            "--- a/subdir/subsubdir/file.txt\n"
            "+++ b/subdir/subsubdir/file.txt\n"
            "@@ -8,6 +8,9 @@ File with line 7\n"
            " File with line 8\n"
            " File with line 9\n"
            " File with line 10\n"
            "+/*~@author(Hans.Eimer) @id(55181)~\n"
            "+ * Wenn Du das machst, dann...\n"
            "+ *        ~@author(Bans.Zweimer) @id(55185) @inReplyTo(55181)~\n"
            "+ *         Erledigt. */\n"
            " File with line 11\n"
            " File with line 12\n"
            " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );
    auto extractedComments = commentExtractor.extract(); //without reference error...
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_ConsecutiveSimpleLineComments )
{
    const string diffWithMultiLineComment =
            "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
            "index 7f8b793..64adcc1 100644\n"
            "--- a/subdir/subsubdir/file.txt\n"
            "+++ b/subdir/subsubdir/file.txt\n"
            "@@ -8,6 +8,11 @@ File with line 7\n"
            " File with line 8\n"
            " File with line 9\n"
            "-File with line 10\n"
            "+File with line 10//~ comment 1\n"
            "-File with line 11\n"
            "+File with line 11//~ comment 2\n"
            " File with line 12\n"
            " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();

    CommentMatcher commentInLineTen ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 10 )
               && ( file == "subdir/subsubdir/file.txt" )
               && ( lineComment.getComment() == "comment 1" );
    } );
    CommentMatcher commentInLineEleven ( [] ( const string & file, const LineComment & lineComment ) {
        return ( lineComment.getLine() == 11 )
               && ( file == "subdir/subsubdir/file.txt" )
                  && ( lineComment.getComment() == "comment 2" );
    } );
    resultingComments.accept ( commentInLineTen );
    resultingComments.accept ( commentInLineEleven );
    EXPECT_TRUE ( commentInLineTen.isMatching() );
    EXPECT_TRUE ( commentInLineEleven.isMatching() );
}

TEST_F ( CommentExtractorTest, Unit_TestExtraction_CanExtractTodos )
{
    const string diffWithMultiLineComment =
            "diff --git a/subdir/subsubdir/file.txt b/subdir/subsubdir/file.txt\n"
            "index 7f8b793..cdc23e0 100644\n"
            "--- a/subdir/subsubdir/file.txt\n"
            "+++ b/subdir/subsubdir/file.txt\n"
            "@@ -8,6 +8,9 @@ File with line 7\n"
            " File with line 8\n"
            " File with line 9\n"
            " File with line 10\n"
            "+/*~@author(author) @id(42)~\n"
            "+ * foreign comment\n"
            "+ * @todo(remove this thing)*/\n"
            " File with line 11\n"
            " File with line 12\n"
            " File with line 13";
    EXPECT_CALL ( gitClient, getDiff() ).WillOnce ( Return ( diffWithMultiLineComment ) );

    Comments resultingComments = commentExtractor.extract();
    CommentMatcher matcher;
    matcher.check ( "replyId", [] ( auto file, auto lineComment ) {
        return lineComment.getId() == 42;
    } );
    matcher.check ( "todo content",
                    [] ( auto file, auto lineComment ) {
                        return lineComment.getTodos()[0].text == "remove this thing";
                    } );
    resultingComments.accept ( matcher );
    EXPECT_TRUE ( matcher.isMatching() );
}

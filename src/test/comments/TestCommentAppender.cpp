#include <gtest/gtest.h>
#include "../../main/comments/CommentAppender.h"
#include "../git/GitTestEnvironment.h"
#include "../../main/comments/Comments.h"

using namespace testing;

class CommentAppenderTest : public Test
{
public:
    GitTestEnvironment testEnv;
    CommentAppender commentAppender;
};

TEST_F ( CommentAppenderTest, Unit_AppendsCommentToFile )
{
    testEnv.run ( "echo Zeile 1 >> datei" );
    testEnv.run ( "echo Zeile 2 >> datei" );
    testEnv.run ( "echo Zeile 3 >> datei" );
    testEnv.run ( "echo Zeile 4 >> datei" );
    testEnv.run ( "echo Zeile 5 >> datei" );
    const string datei (
        R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein Kommentar */
Zeile 5
)");
    Comments comments({{"datei", {{4, "Dies ist ein Kommentar", "imgrundm"}}}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F ( CommentAppenderTest, Unit_AppendsCommentWithIdToFile )
{
    testEnv.run ( "echo Zeile 1 >> datei" );
    testEnv.run ( "echo Zeile 2 >> datei" );
    testEnv.run ( "echo Zeile 3 >> datei" );
    testEnv.run ( "echo Zeile 4 >> datei" );
    testEnv.run ( "echo Zeile 5 >> datei" );
    const string datei (
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm) @id(1234321)~
 * Dies ist ein Kommentar */
Zeile 5
)");
    Comments comments({{"datei", {{4,"Dies ist ein Kommentar","imgrundm",1234321,{}}}}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsMultipleCommentsToFile) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
/*~@author(Hans Wurst)~
 * Dies ist auch ein Kommentar */
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein Kommentar */
Zeile 5
)");
    Comments comments({{"datei", {
                                         {2, "Dies ist auch ein Kommentar", "Hans Wurst"},
                                         {4, "Dies ist ein Kommentar", "imgrundm"}}}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsMultipleCommentsToSingleLine) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein Kommentar */
/*~@author(Hans Wurst)~
 * Dies ist auch ein Kommentar */
Zeile 5
)");
    Comments comments({{"datei", {
                                         {4, "Dies ist ein Kommentar", "imgrundm"},
                                         {4, "Dies ist auch ein Kommentar", "Hans Wurst"}
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsCommentsToSeveralFiles) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    testEnv.run("echo Zeile 1 >> datei2");
    testEnv.run("echo Zeile 2 >> datei2");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein Kommentar */
Zeile 5
)");
    const string datei2(
            R"(Zeile 1
Zeile 2
/*~@author(oink)~
 * Ein weiterer Kommentar */
)");
    Comments comments({
                              {"datei",  {{4, "Dies ist ein Kommentar", "imgrundm"}}},
                              {"datei2", {{2, "Ein weiterer Kommentar", "oink"}}}
                      });
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
    EXPECT_STREQ(datei2.c_str(), testEnv.run("cat datei2").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsLongCommentMultilined) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein langer Kommentar mit mehr als 30 Zeichen */
Zeile 5
)");
    Comments comments({{"datei", {
                                         {4, "Dies ist ein langer Kommentar mit mehr als 30 Zeichen", "imgrundm"}
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsCommentsWithNewlineMultilined) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein
 * doofer Reim. */
Zeile 5
)");
    Comments comments({{"datei", {
                                         {4, "Dies ist ein\ndoofer Reim.", "imgrundm"}
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsRepliesIndented) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Dies ist ein doofer Reim.
 *        ~@author(replyMan)~
 *         allerdings */
Zeile 5
)");
    Comments comments({{"datei",
                               {LineComment(4, "Dies ist ein doofer Reim.", "imgrundm",{},{
                                       ((LineComment) {0, "allerdings", "replyMan", {}})
                               })}
    }});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsRepliesIndentedWithId) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm) @id(1)~
 * Dies ist ein doofer Reim.
 *        ~@author(replyMan) @id(2) @inReplyTo(1)~
 *         allerdings */
Zeile 5
)");
    Comments comments({{"datei",
                               {LineComment(4, "Dies ist ein doofer Reim.", "imgrundm",1,{
                                       ((LineComment) {0, "allerdings", "replyMan", 2,{}})
                               })}
                       }});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsRepliesIndented_MoreComplexCase) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(A)~
 * CommentA
 *        ~@author(B)~
 *         CommentB
 *                ~@author(C)~
 *                 CommentC
 *        ~@author(C)~
 *         CommentC2 */
Zeile 5
)");
    Comments comments({{"datei",
                               {LineComment(4, "CommentA", "A", {},{
                                                                      (LineComment) {0, "CommentB", "B",{},
                                                                                     {((LineComment) {0, "CommentC", "C", {}})}},

                                                                      (LineComment) {0, "CommentC2", "C",}
                               })}}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsCommentsBreaksLongLines) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    const string datei(
            "Zeile 1\n"
            "/*~@author(imgrundm)~\n"
            " * 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789\n"
            " * 123456789 123456789 123456789 123456789 123456789 123456789 */\n"
            "Zeile 2\n");
    Comments comments({{"datei", {{1,
                                          "123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789", "imgrundm"}
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsTodos) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Der Kommentar
 * @todo(noch zu tun) */
Zeile 5
)");
    LineComment lineComment = {4, "Der Kommentar", "imgrundm"};
    lineComment.addTodo({"noch zu tun", Todo::TodoStatus::TODO});
    Comments comments({{"datei", {
                                        lineComment
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsDones) {
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4
/*~@author(imgrundm)~
 * Der Kommentar
 * @done(Aufgabe) */
Zeile 5
)");
    LineComment lineComment = {4, "Der Kommentar", "imgrundm"};
    lineComment.addTodo({"Aufgabe", Todo::TodoStatus::DONE});
    Comments comments({{"datei", {
                                         lineComment
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(), testEnv.run("cat datei").getOutput().c_str());
}

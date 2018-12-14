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
Zeile 4//~imgrundm~ Dies ist ein Kommentar
Zeile 5
)");
    Comments comments({{"datei", {{4,"Dies ist ein Kommentar","imgrundm"}}}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsMultipleCommentsToFile){
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2//~Hans Wurst~ Dies ist auch ein Kommentar
Zeile 3
Zeile 4//~imgrundm~ Dies ist ein Kommentar
Zeile 5
)");
    Comments comments({{"datei", {
        {2,"Dies ist auch ein Kommentar","Hans Wurst"},
        {4,"Dies ist ein Kommentar","imgrundm"}}}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsMultipleCommentsToSingleLine){
    testEnv.run("echo Zeile 1 >> datei");
    testEnv.run("echo Zeile 2 >> datei");
    testEnv.run("echo Zeile 3 >> datei");
    testEnv.run("echo Zeile 4 >> datei");
    testEnv.run("echo Zeile 5 >> datei");
    const string datei(
            R"(Zeile 1
Zeile 2
Zeile 3
Zeile 4//~imgrundm~ Dies ist ein Kommentar//~Hans Wurst~ Dies ist auch ein Kommentar
Zeile 5
)");
    Comments comments({{"datei", {
                                         {4,"Dies ist ein Kommentar","imgrundm"},
                                         {4,"Dies ist auch ein Kommentar","Hans Wurst"}
    }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsCommentsToSeveralFiles){
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
Zeile 4//~imgrundm~ Dies ist ein Kommentar
Zeile 5
)");
    const string datei2(
            R"(Zeile 1
Zeile 2//~oink~ Ein weiterer Kommentar
)");
    Comments comments({
                              {"datei",  {{4, "Dies ist ein Kommentar", "imgrundm"}}},
                              {"datei2", {{2, "Ein weiterer Kommentar", "oink"}}}
                      });
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
    EXPECT_STREQ(datei2.c_str(),testEnv.run("cat datei2").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsLongCommentMultilined){
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
/*~imgrundm~ Dies ist ein langer Kommentar mit mehr als 30 Zeichen */
Zeile 5
)");
    Comments comments({{"datei", {
                                         {4,"Dies ist ein langer Kommentar mit mehr als 30 Zeichen","imgrundm"}
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
}

TEST_F(CommentAppenderTest, Unit_AppendsCommentsWithNewlineMultilined){
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
/*~imgrundm~ Dies ist ein
 * doofer Reim. */
Zeile 5
)");
    Comments comments({{"datei", {
                                         {4,"Dies ist ein\ndoofer Reim.","imgrundm"}
                                 }}});
    comments.accept(commentAppender);
    commentAppender.finish();
    EXPECT_STREQ(datei.c_str(),testEnv.run("cat datei").getOutput().c_str());
}

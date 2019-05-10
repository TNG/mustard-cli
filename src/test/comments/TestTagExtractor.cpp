#include <gtest/gtest.h>
#include "../../main/comments/TagExtractor.h"

using namespace testing;

class TestTagExtractor : public Test
{
public:
    TagExtractor tagExtractor;
};

TEST_F ( TestTagExtractor, Unit_empty_string_yields_empty_list )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "" );
    ASSERT_TRUE ( tagList.empty() );
}

TEST_F ( TestTagExtractor, Unit_no_tag_in_string_yields_empty_list )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "Gibberisch barber laber" );
    ASSERT_TRUE ( tagList.empty() );
}

TEST_F ( TestTagExtractor, Unit_alone_tagmarker_yields_empty_list )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "Gibberisch barber @ laber" );
    ASSERT_TRUE ( tagList.empty() );
}

TEST_F ( TestTagExtractor, Unit_tag_without_value_is_parsed )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "//~ @reply" );
    ASSERT_EQ ( 1, tagList.size() );
    EXPECT_STREQ ( "reply", tagList[0].name.c_str() );
    EXPECT_FALSE ( tagList[0].value.has_value() );
}

TEST_F ( TestTagExtractor, Unit_tag_with_value_is_parsed )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "//~ @author(imgrund)" );
    ASSERT_EQ ( 1, tagList.size() );
    EXPECT_STREQ ( "author", tagList[0].name.c_str() );
    EXPECT_STREQ ( "imgrund", tagList[0].value.value().c_str() );
}

TEST_F ( TestTagExtractor, Unit_several_tags_with_value_are_parsed )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "//~@author(imgrund) @replyTo(4)" );
    ASSERT_EQ ( 2, tagList.size() );
    EXPECT_STREQ ( "author", tagList[0].name.c_str() );
    EXPECT_STREQ ( "imgrund", tagList[0].value.value().c_str() );
    EXPECT_STREQ ( "replyTo", tagList[1].name.c_str() );
    EXPECT_STREQ ( "4", tagList[1].value.value().c_str() );
}

TEST_F ( TestTagExtractor, Unit_several_tags_with_and_without_value_are_parsed )
{
    const auto &tagList = tagExtractor.extractTagsIn ( "//~ @author(imgrund) @intermediate @replyTo(4)" );
    ASSERT_EQ ( 3, tagList.size() );
    EXPECT_STREQ ( "author", tagList[0].name.c_str() );
    EXPECT_STREQ ( "imgrund", tagList[0].value.value().c_str() );
    EXPECT_STREQ ( "intermediate", tagList[1].name.c_str() );
    EXPECT_FALSE ( tagList[1].value.has_value() );
    EXPECT_STREQ ( "replyTo", tagList[2].name.c_str() );
    EXPECT_STREQ ( "4", tagList[2].value.value().c_str() );
}

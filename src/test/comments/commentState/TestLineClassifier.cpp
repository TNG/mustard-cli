#include <gtest/gtest.h>
#include "../../../main/comments/commentState/LineClassifier.h"

using namespace testing;

class LineClassifierTest : public Test
{
public:
    ;
    LineClassifier lineClassifier;
};


TEST_F ( LineClassifierTest, Unit_CanCopeWithVeryLongLines )
{
    stringstream ss;
    ss << "- ";
    for ( int i = 0; i < 10000; ++i ) {
        ss << "12876zaiufzdb8azsbzsa8zb28736876z87g ";
    }


    string line ( ss.str() );
    lineClassifier.classifyLine ( line );
}
#ifndef MUSTARD_TAGEXTRACTOR_H
#define MUSTARD_TAGEXTRACTOR_H
#include <string>
#include <vector>
#include <regex>
#include "Tag.h"

using namespace std;

class TagExtractor
{
public:
    TagExtractor();
    vector<Tag> extractTagsIn ( const string &line );

    string removeTagsFrom ( const string &line );

private:
    const regex tagMatcher;
};


#endif //MUSTARD_TAGEXTRACTOR_H

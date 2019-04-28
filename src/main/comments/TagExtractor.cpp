#include <iostream>
#include <regex>
#include <Provide.h>
#include "TagExtractor.h"

ProvideDependency<TagExtractor> tagExtractorDependency;

TagExtractor::TagExtractor() :
    tagMatcher ( R"(@(\w+)(?:\(([^\)]+)\))?)" )
{}

vector <Tag> TagExtractor::extractTagsIn ( const string &line )
{
    vector<Tag> results;
    for ( auto it = sregex_iterator ( line.begin(), line.end(), tagMatcher ); it != sregex_iterator(); ++it ) {
        const auto &submatch = *it;
        if ( submatch.size() != 3 ) {
            continue;
        }
        if ( 0 == submatch[2].length() ) {
            results.emplace_back ( Tag ( submatch[1], {} ) );
            continue;
        }
        results.emplace_back ( Tag ( submatch[1], submatch[2] ) );
    }
    return results;
}

string TagExtractor::removeTagsFrom ( const string &line )
{
    return regex_replace ( line, tagMatcher, "" );
}

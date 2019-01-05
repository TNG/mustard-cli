#ifndef MUSTARD_REGEXMATCHER_H
#define MUSTARD_REGEXMATCHER_H

#include <regex>

using namespace std;

class RegexMatcher
{
public:
    RegexMatcher ( const string &regex )
        : extractor ( regex ) {}

    string getSingleCaptureIn ( const string &text ) const {
        smatch matches;
        regex_match ( text, matches, extractor );
        if ( matches.size() != 2 ) {
            return "";
        }
        return matches[1];
    }

    bool isMatching ( const string &text ) const {
        return regex_match ( text, extractor );
    }

private:
    regex extractor;
};

#endif //MUSTARD_REGEXMATCHER_H

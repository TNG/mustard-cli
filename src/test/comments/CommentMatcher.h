#ifndef MUSTARD_COMMENTMATCHER_H
#define MUSTARD_COMMENTMATCHER_H

#include <functional>
#include <vector>
#include "../../main/comments/CommentConsumer.h"

using namespace std;

class CommentMatcher : public CommentConsumer
{
public:
    CommentMatcher() = default;
    CommentMatcher ( std::function<bool ( const string &file, const LineComment & ) > predicate );

    void check ( const string &explanation, function<bool ( const string &file, const LineComment & ) > predicate );
    void consume ( const string &file, const LineComment &lineComment ) override;

    bool isMatching();
    bool isNotMatching();

private:
    struct predicateMatchedExplanation {
        bool matched;
        string explanation;
        function<bool ( const string &, const LineComment & ) > predicate;
    };

    vector<predicateMatchedExplanation> predicates;
    bool matched = false;
};

#endif //MUSTARD_COMMENTMATCHER_H

#include <functional>
#include <iostream>
#include <gtest/gtest.h>
#include "../../main/comments/CommentConsumer.h"
#include "CommentMatcher.h"

CommentMatcher::CommentMatcher ( std::function<bool ( const string &file, const LineComment & ) > predicate )
{
    predicates.push_back ( {false, "constructor predicate", predicate} );
}


void CommentMatcher::consume ( const string &file, const LineComment &lineComment )
{
    bool allMatch = true;
    for ( auto &predicate : predicates ) {
        bool isMatching = predicate.predicate ( file, lineComment );
        allMatch = allMatch && isMatching;
        predicate.matched = isMatching || predicate.matched;
    }
    matched |= allMatch;
}

bool CommentMatcher::isMatching()
{
    if ( !matched ) {
        for ( auto predicate : predicates ) {
            if ( !predicate.matched ) {
                std::cout << "Did not match: " << predicate.explanation << endl;
            }
        }
    }
    return matched;
}

void CommentMatcher::check ( const string &explanation, function<bool ( const string &, const LineComment & ) > predicate )
{
    predicates.push_back ( {false, explanation, predicate} );
}

bool  CommentMatcher::isNotMatching()
{
    return !matched ;
}

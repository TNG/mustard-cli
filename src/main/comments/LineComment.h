#ifndef MUSTARD_LINECOMMENT_H
#define MUSTARD_LINECOMMENT_H

#include <string>

using namespace std;

class LineComment
{
public:
    LineComment ( const unsigned int line, const string &comment, const string &author = "" )
        : line ( line ), comment ( comment ), author ( author ) {}

    unsigned int getLine() const {
        return line;
    };

    const string &getComment() const {
        return comment;
    }

    const string &getAuthor() const  {
        return author;
    }

private:
    const unsigned int line;
    const string comment;
    const string author;
};


#endif //MUSTARD_LINECOMMENT_H

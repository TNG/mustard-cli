#ifndef MUSTARD_LINECOMMENT_H
#define MUSTARD_LINECOMMENT_H

#include <string>

using namespace std;

class LineComment
{
public:
    LineComment ( const unsigned int line, const string &comment )
        : line ( line ), comment ( comment ) {}

    unsigned int getLine() const {
        return line;
    };

    const string &getComment() const {
        return comment;
    }

private:
    const unsigned int line;
    const string comment;
};


#endif //MUSTARD_LINECOMMENT_H

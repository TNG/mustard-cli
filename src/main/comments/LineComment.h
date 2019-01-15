#ifndef MUSTARD_LINECOMMENT_H
#define MUSTARD_LINECOMMENT_H

#include <string>
#include <vector>

using namespace std;

class LineComment
{
public:
    LineComment ( const unsigned int line,
                  const string &comment,
                  const string &author = "",
                  const vector<LineComment> replies = {} )
        : line ( line ),
          comment ( comment ),
          author ( author ),
          replies ( replies ) {}

    unsigned int getLine() const {
        return line;
    };

    const string &getComment() const {
        return comment;
    }

    const string &getAuthor() const  {
        return author;
    }

    const vector<LineComment> &getReplies() const {
        return replies;
    }

private:
    const unsigned int line;
    const string comment;
    const string author;
    const vector<LineComment> replies;
};


#endif //MUSTARD_LINECOMMENT_H

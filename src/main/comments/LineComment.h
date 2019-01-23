#include <utility>

#ifndef MUSTARD_LINECOMMENT_H
#define MUSTARD_LINECOMMENT_H

#include <string>
#include <vector>
#include <sstream>
#include <optional>

using namespace std;

class LineComment
{
public:
    LineComment ( const unsigned int line,
                  const string &comment,
                  const string &author = "",
                  const vector<LineComment> &replies = {},
                  optional<unsigned long> id = {} )
        : line ( line ),
          comment ( comment ),
          author ( author ),
          replies ( replies ),
          id ( move ( id ) ) {}

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

    const optional<unsigned long> &getId() const {
        return id;
    }

    const string getAuthorAndId() const {
        if ( !id.has_value() ) {
            return getAuthor();
        }
        stringstream ss;
        ss << getAuthor() << "@" << getId().value();
        return ss.str();
    }

private:
    const unsigned int line;
    const string comment;
    const string author;
    const vector<LineComment> replies;
    const optional<unsigned long> id;
};


#endif //MUSTARD_LINECOMMENT_H

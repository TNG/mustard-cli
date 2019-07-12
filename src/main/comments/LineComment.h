#include <utility>

#include <utility>

#ifndef MUSTARD_LINECOMMENT_H
#define MUSTARD_LINECOMMENT_H

#include <string>
#include <vector>
#include <sstream>
#include <optional>
#include <functional>
#include "Todo.h"

using namespace std;

class LineComment
{
public:
    LineComment ( const unsigned int line,
                  string comment,
                  string author = "",
                  optional<unsigned long> id = {},
                  vector<LineComment> replies = {} )
        : line ( line ),
          comment ( std::move ( comment ) ),
          author ( std::move ( author ) ),
          replies ( std::move ( replies ) ),
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

    vector<LineComment> &getReplies() {
        return replies;
    }

    const vector<LineComment> &getReplies() const {
        return replies;
    }

    void addReply ( const LineComment &lineComment ) {
        replies.push_back ( lineComment );
    }

    void addTodo ( const Todo &todo ) {
        todos.push_back ( todo );
    }

    const vector<Todo> &getTodos() {
        return todos;
    }

    const optional<unsigned long> &getId() const {
        return id;
    }

    const string getAuthorAndId() const {
        if ( !id.has_value() ) {
            return "@author(" + getAuthor() + ")";
        }
        stringstream ss;
        ss << "@author(" << getAuthor() << ") @id(" << getId().value() << ")";
        return ss.str();
    }

    void forEachReply ( std::function<void ( const LineComment & ) > lineCommentFunction ) const {
        for ( auto &reply : replies ) {
            lineCommentFunction ( reply );
        }
    }

private:
    const unsigned int line;
    const string comment;
    const string author;
    vector<LineComment> replies;
    vector<Todo> todos;
    const optional<unsigned long> id;
};


#endif //MUSTARD_LINECOMMENT_H

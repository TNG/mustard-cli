#ifndef MUSTARD_COMMENTSTATELISTENER_H
#define MUSTARD_COMMENTSTATELISTENER_H
#include <string>
#include <optional>
#include <vector>

#include "../Todo.h"

using namespace std;

class CommentStateListener
{
public:
    virtual void newFile ( const string &fileName ) = 0;
    virtual void newLine() = 0;
    virtual void newPhantomLine() = 0;
    virtual void newComment ( const string &author, const string &comment, optional <unsigned long> id, optional<unsigned long> replyToId ) = 0;
    virtual void withTodos ( vector<Todo> &todos ) = 0;
    void newComment ( const string &author, const string &comment ) {
        newComment ( author, comment, {}, {} );
    }
    virtual void setLine ( int i ) = 0;
};

#endif //MUSTARD_COMMENTSTATELISTENER_H

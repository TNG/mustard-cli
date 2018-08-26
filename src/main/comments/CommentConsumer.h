#ifndef MUSTARD_COMMENTCONSUMER_H
#define MUSTARD_COMMENTCONSUMER_H

#include <string>

using namespace std;

class CommentConsumer
{
public:
    virtual void consume ( const string &file, unsigned int line, const string &comment ) = 0;
};

#endif //MUSTARD_COMMENTCONSUMER_H

#ifndef MUSTARD_COMMENEXTRACTOR_H
#define MUSTARD_COMMENEXTRACTOR_H

#include "Comments.h"

class CommentExtractor
{
public:
    virtual Comments extract() = 0;
    virtual ~CommentExtractor() {}
};

#endif //MUSTARD_COMMENEXTRACTOR_H

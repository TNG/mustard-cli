#ifndef MUSTARD_COMMENTS_H
#define MUSTARD_COMMENTS_H
#include <vector>
#include "FileComments.h"

using namespace std;

class Comments
{
public:
    Comments ( const vector<FileComments> &fileComments );
    void accept ( CommentConsumer &consumer ) const;

private:
    vector<FileComments> comments;
};


#endif //MUSTARD_COMMENTS_H

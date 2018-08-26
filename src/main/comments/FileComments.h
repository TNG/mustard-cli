#ifndef MUSTARD_FILECOMMENTS_H
#define MUSTARD_FILECOMMENTS_H

#include <vector>
#include "LineComment.h"
#include "CommentConsumer.h"

using namespace std;

class FileComments
{
public:
    FileComments ( const string &file, const vector<LineComment> &lineComments );
    void accept ( CommentConsumer &consumer ) const;
private:
    vector<LineComment> lineComments;
    string file;
};
#endif //MUSTARD_FILECOMMENTS_H

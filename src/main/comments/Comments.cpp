#include "Comments.h"
#include "CommentConsumer.h"

Comments::Comments ( const vector<FileComments> &fileComments ) :
    comments ( fileComments )
{}

void Comments::accept ( CommentConsumer &consumer ) const
{
    for ( const auto &fileComments : comments ) {
        fileComments.accept ( consumer );
    }
}

bool Comments::isEmpty() const
{
    return comments.empty();
}

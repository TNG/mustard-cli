#include "FileComments.h"

void FileComments::accept ( CommentConsumer &consumer ) const
{
    for ( const auto &lineComment : lineComments ) {
        consumer.consume ( file, lineComment );
    }
}

FileComments::FileComments ( const string &file, const vector<LineComment> &lineComments ) :
    file ( file ), lineComments ( lineComments ) {}

#include "FileDefinitionConsumer.h"
#include "../RegexMatcher.h"

FileDefinitionConsumer::FileDefinitionConsumer ( CommentStateListener *listener ) : listener ( listener ) {}

void FileDefinitionConsumer::consume ( const string &line )
{
    static RegexMatcher fileName ( R"(^\+\+\+ b/(.*)$)" );
    string extractedFileName = fileName.getSingleCaptureIn ( line );
    if ( !extractedFileName.empty() ) {
        listener->newFile ( extractedFileName );
    }
}

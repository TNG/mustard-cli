#include "FileDefinitionConsumer.h"
#include "../RegexMatcher.h"

FileDefinitionConsumer::FileDefinitionConsumer ( CommentStateListener *listener ) : listener ( listener ) {}

void FileDefinitionConsumer::consume ( const string &line )
{
    static RegexMatcher fileName ( R"(^\+\+\+ b/(.*)$)" );
    const auto extractedFileName = fileName.getSingleCaptureIn ( line );
    if ( extractedFileName.has_value() ) {
        listener->newFile ( extractedFileName.value() );
    }
}

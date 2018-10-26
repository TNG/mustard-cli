#include <fstream>
#include "CommentAppender.h"
#include "../error/MustardException.h"

void CommentAppender::consume ( const string &fileName, const LineComment &lineComment )
{
    if ( fileName != openFileName ) {
        if ( openFileName != "" ) {
            finishFile();
            openFileComments.clear();
        }
        openFileName = fileName;
    }
    openFileComments.push_back ( lineComment );
}

void CommentAppender::finish()
{
    finishFile();
}

void CommentAppender::finishFile()
{
    if ( openFileName.empty() ) {
        return;
    }
    ifstream file;
    file.open ( openFileName.c_str() );
    string currentLine;
    vector<string> fileLines;

    while ( true ) {
        getline ( file, currentLine );
        if ( file.eof() ) {
            break;
        }
        fileLines.push_back ( currentLine );
    }

    for ( auto lineComment : openFileComments ) {
        if ( lineComment.getLine() > fileLines.size() ) {
            throw MustardException ( "Could not append a comment past the last line of a file" );
        }
        string &originalLine = fileLines[lineComment.getLine() - 1];
        originalLine += "//~" + lineComment.getAuthor() + " - " + lineComment.getComment();
    }
    file.close();

    ofstream outfile;
    outfile.open ( openFileName.c_str(), ios_base::trunc );
    for ( auto line : fileLines ) {
        outfile << line << endl;
    }
    outfile.close();
}

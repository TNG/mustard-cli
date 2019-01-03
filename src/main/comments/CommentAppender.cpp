#include <fstream>
#include <algorithm>
#include <regex>
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
        if ( lineComment.getComment().size() > 30 || lineComment.getComment().find ( '\n' ) != string::npos ) {
            insertMultiLineComment ( fileLines, lineComment );
        } else {
            insertSingleLineComment ( fileLines, lineComment );
        };
    }
    file.close();

    ofstream outfile;
    outfile.open ( openFileName.c_str(), ios_base::trunc );
    for ( auto line : fileLines ) {
        outfile << line << endl;
    }
    outfile.close();
}

void CommentAppender::insertMultiLineComment ( vector<string> &fileLines, const LineComment &comment )
{
    string &originalLine = fileLines[comment.getLine() - 1];
    static regex newline ( "([^\n]{1,110})(?:( )|\n|$)" );
    const string replacedNewLines = regex_replace ( comment.getComment(), newline, "\n * $1" );
    stringstream ss;
    ss << endl << "/*~" << comment.getAuthor() << "~" << replacedNewLines << " */";
    fileLines[comment.getLine() - 1] += ss.str();
}

void CommentAppender::insertSingleLineComment ( vector<string> &fileLines, const LineComment &comment )
{
    string &originalLine = fileLines[comment.getLine() - 1];
    originalLine += "//~" + comment.getAuthor() + "~ " + comment.getComment();
}

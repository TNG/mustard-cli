#include <fstream>
#include <algorithm>
#include <regex>
#include "CommentAppender.h"
#include "Todo.h"
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
        insertMultiLineComment ( fileLines, lineComment );
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
    stringstream ss;
    ss << endl << "/*";
    formatComment ( comment, ss, 0, optional<unsigned long>() );
    ss << " */";
    fileLines[comment.getLine() - 1] += ss.str();
}

void CommentAppender::formatComment ( const LineComment &comment, stringstream &ss, unsigned int indentationLevel,
                                      optional<unsigned long> inReplyTo )
{
    string indentation;
    for ( int i = 0; i < indentationLevel * indentationDepth ; ++i ) {
        indentation += " ";
    }
    const string indentingReplace = "\n * " + indentation + "$1";
    const string replacedNewLines = regex_replace ( comment.getComment(), getIndentationRegexForIndentationLevel ( indentationLevel ), indentingReplace );
    ss << indentation << "~"
       << comment.getAuthorAndId();
    if ( inReplyTo.has_value() ) {
        ss << " @inReplyTo(" << inReplyTo.value() << ")";
    }
    ss << "~" << replacedNewLines ;
    for ( const auto &todo : comment.getTodos() ) {
        const char *tagName = todo.status == Todo::TodoStatus::DONE ? "done" : "todo";
        ss << endl << " * " << indentation << "@" << tagName << "(" << todo.text << ")";
    }
    for ( const auto &reply : comment.getReplies() ) {
        ss << endl << " *";
        formatComment ( reply, ss, indentationLevel + 1, comment.getId() );
    }
}

regex CommentAppender::getIndentationRegexForIndentationLevel ( unsigned int indentationLevel )
{
    if ( indentationLevel > 5 ) {
        indentationLevel = 5;
    }
    unsigned int untilChars = 110 - ( indentationLevel * indentationDepth );
    stringstream regexSs;
    regexSs << "([^\n]{1," << untilChars << "})(?:( )|\n|$)" ;
    return regex ( regexSs.str() );
}


#include <regex>
#include <Provide.h>
#include <Depend.h>
#include "CommentExtractorImpl.h"
#include "../error/MustardException.h"


using namespace std;

ProvideImplementationForDependency <CommentExtractorImpl, CommentExtractor> commentExtractorDependency;

CommentExtractorImpl::CommentExtractorImpl ( GitClient *gitClient ) : gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

map<CommentExtractorImpl::LineType, regex> CommentExtractorImpl::lineTypeRegexes = {
    {FILEDEFINITION, regex ( "^diff.*" ) },
    {CONTEXTDEFINITION, regex ( "^@@.*" ) },
    {ADDFILE, regex ( "^\\+\\+\\+.*" ) },
    {ADDLINE, regex ( "^\\+.*" ) },
    {DELLINE, regex ( "^\\-.*" ) }
};

Comments CommentExtractorImpl::extract()
{
    string file = "";
    std::vector<LineComment> comments;
    std::vector<FileComments> fileComments;

    std::vector<string> diffLines = getDiffLines();
    unsigned int lineNumber = 0;
    for ( const auto &line : diffLines ) {
        switch ( LineType type = getLineType ( line ) ) {
        case FILEDEFINITION:
            if ( file.length() != 0 ) {
                addFoundCommentsTo ( fileComments, comments, file );
            }
            comments.clear();
            file = "";
            break;
        case ADDFILE:
            if ( file.length() != 0 ) {
                break;
            }
            file = extractAddedFileFrom ( line );
            break;
        case CONTEXTDEFINITION:
            if ( file.length() == 0 ) {
                throw MustardException ( "Malformed diff: Context definition not preceded by file add line" );
            }
            lineNumber = extractContextLineNumber ( line ) - 1;
            break;
        case ADDLINE:
            if ( file.length() != 0 ) {
                static regex commentExtractor ( R"(^\+.*//([^~]*)$)" );
                const string comment = getSingleCaptureIn ( line, commentExtractor );
                if ( comment.length() > 0 ) {
                    comments.emplace_back ( LineComment ( lineNumber, comment ) );
                }
            }
            break;
        case DELLINE:
            lineNumber--;
            break;
        }
        ++lineNumber;
    }
    if ( file.length() != 0 && comments.size() != 0 ) {
        addFoundCommentsTo ( fileComments, comments, file );
    }
    return Comments ( fileComments );
}

vector<string> CommentExtractorImpl::getDiffLines()
{
    const string rawString = gitClient->getDiff();
    vector<string> lines;
    string line;
    stringstream ss ( rawString );
    while ( std::getline ( ss, line ) ) {
        lines.push_back ( line );
    }
    return move ( lines );
}

CommentExtractorImpl::LineType CommentExtractorImpl::getLineType ( const string &line )
{
    for ( auto lineTypeRegex : lineTypeRegexes ) {
        if ( regex_match ( line, lineTypeRegex.second ) ) {
            return lineTypeRegex.first;
        };
    }
    return UNKNOWN;
}

void
CommentExtractorImpl::addFoundCommentsTo ( vector<FileComments> &fileComments, vector<LineComment> comments, string file )
{
    fileComments.emplace_back ( FileComments ( file, comments ) );
}

string CommentExtractorImpl::extractAddedFileFrom ( const string &line )
{
    static regex extractor ( R"(^\+\+\+ b/(.*)$)" );
    return getSingleCaptureIn ( line, extractor );
}

string CommentExtractorImpl::getSingleCaptureIn ( const string &text, const regex &extractor ) const
{
    smatch matches;
    regex_match ( text, matches, extractor );
    if ( matches.size() != 2 ) {
        return "";
    }
    return matches[1];
}

int CommentExtractorImpl::extractContextLineNumber ( const string &line )
{
    static regex extractor ( R"(^@@ -\d* \+(\d*) @@.*)" );
    const string stringNumber = getSingleCaptureIn ( line, extractor );
    return atoi ( stringNumber.c_str() );
}

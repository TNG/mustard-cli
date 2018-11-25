#include <regex>
#include <Provide.h>
#include <Depend.h>
#include "CommentExtractorImpl.h"
#include "../error/MustardException.h"
#include "commentState/CommentState.h"
#include "commentState/states/DiffHeaderState.h"


using namespace std;

ProvideImplementationForDependency<CommentExtractorImpl, CommentExtractor> commentExtractorDependency;

CommentExtractorImpl::CommentExtractorImpl ( GitClient *gitClient, LineClassifier *lineClassifier ) :
    gitClient ( DependentOn<GitClient> ( gitClient ) ),
    lineClassifier ( DependentOn<LineClassifier> ( lineClassifier ) )
{}

Comments CommentExtractorImpl::extract()
{
    std::vector<string> diffLines = getDiffLines();
    CommentState *commentState = new DiffHeaderState ( this );

    for ( const auto &line : diffLines ) {
        CommentState *nextCommentState = commentState->traverse ( lineClassifier->classifyLine ( line ) );
        if ( nextCommentState != commentState ) {
            delete commentState;
            commentState = nextCommentState;
        }
        commentState->consume ( line );
    }
    delete commentState;
    newFile ( "" );
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

void CommentExtractorImpl::newFile ( const string &fileName )
{
    if ( !currentFile.empty() && currentLineComments.size() != 0 ) {
        fileComments.push_back ( {
            currentFile,
            currentLineComments
        } );
    }
    currentFile = fileName;
    currentLineComments.clear();
    currentLine = 1;
}


void CommentExtractorImpl::newLine()
{
    ++currentLine;
}

void CommentExtractorImpl::setLine ( int lineNumber )
{
    currentLine = lineNumber;
}

void CommentExtractorImpl::newComment ( const string &author, const string &comment )
{
    currentLineComments.push_back ( {currentLine, comment, author} );
}

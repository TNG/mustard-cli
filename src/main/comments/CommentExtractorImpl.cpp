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
    lineClassifier ( DependentOn<LineClassifier> ( lineClassifier ) ) {}

Comments CommentExtractorImpl::extract()
{
    const auto diffLines = getDiffLines();

    const auto classifiedLines = classifyLines ( diffLines );
    consumeLineContent ( classifiedLines );

    finishOwnState();
    return Comments ( fileComments );
}

void CommentExtractorImpl::finishOwnState()
{
    newFile ( "" );
}

void CommentExtractorImpl::consumeLineContent (
    const vector<pair<shared_ptr<CommentState>, const string *>> &classifiedLines ) const
{
    shared_ptr<CommentState> precedingState;
    for ( const auto &lineState : classifiedLines ) {
        if ( precedingState != nullptr && precedingState != lineState.first ) {
            precedingState->scopeChange();
        }
        lineState.first->consume ( *lineState.second );
        precedingState = lineState.first;
    }

    if ( precedingState ) {
        precedingState->scopeChange();
    }
}

vector<pair<shared_ptr<CommentState>, const string *>> CommentExtractorImpl::classifyLines (
            const vector<string> &diffLines )
{
    shared_ptr<CommentState> commentState = shared_ptr<DiffHeaderState> ( new DiffHeaderState ( this ) );

    vector<pair<shared_ptr<CommentState>, const string *>> lineStates;
    for ( const auto &line : diffLines ) {
        commentState = commentState->traverse ( lineClassifier->classifyLine ( line ) );
        lineStates.emplace_back ( commentState, &line );
    }
    commentState->traverse ( LineClassifier::ENDOFFILE );
    return lineStates;
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
    phantomLines = 0;
}


void CommentExtractorImpl::newLine()
{
    ++currentLine;
}

void CommentExtractorImpl::newPhantomLine()
{
    ++phantomLines;
}

void CommentExtractorImpl::setLine ( int lineNumber )
{
    currentLine = lineNumber - phantomLines;
}

void CommentExtractorImpl::newComment ( const string &author, const string &comment )
{
    currentLineComments.push_back ( {currentLine, comment, author} );
}

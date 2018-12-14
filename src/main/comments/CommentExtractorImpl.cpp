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
    std::vector<string> diffLines = getDiffLines();
    vector<pair<CommentState *, const string *>> classifiedLines = classifyLines ( diffLines );

    consumeLineContent ( classifiedLines );

    deleteStates ( classifiedLines );

    finishOwnState();
    return Comments ( fileComments );
}

void CommentExtractorImpl::finishOwnState()
{
    newFile ( "" );
}

void CommentExtractorImpl::deleteStates ( const vector<pair<CommentState *, const string *>> &classifiedLines ) const
{
    set<CommentState *> commentStates;
    for ( auto &lineState : classifiedLines ) {
        commentStates.insert ( lineState.first );
    }

    for ( auto currentCommentState : commentStates ) {
        delete currentCommentState;
    }
}

void CommentExtractorImpl::consumeLineContent ( const vector<pair<CommentState *, const string *>> &classifiedLines ) const
{
    CommentState *precedingState = nullptr;
    for ( auto &lineState : classifiedLines ) {
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

vector<pair<CommentState *, const string *>> CommentExtractorImpl::classifyLines ( const vector<string> &diffLines )
{
    CommentState *commentState = new DiffHeaderState ( this );

    vector<pair<CommentState *, const string *>> lineStates;
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

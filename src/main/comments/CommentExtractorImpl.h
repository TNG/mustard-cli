#ifndef MUSTARD_COMMENTEXTRACTORIMPL_H
#define MUSTARD_COMMENTEXTRACTORIMPL_H
#include <map>
#include <regex>

using namespace std;

#include "../git/GitClient.h"
#include "Comments.h"
#include "CommenExtractor.h"
#include "commentState/CommentStateListener.h"
#include "commentState/LineClassifier.h"
#include "commentState/CommentState.h"

class CommentExtractorImpl : public CommentExtractor, CommentStateListener
{
public:
    CommentExtractorImpl ( GitClient *gitClient = nullptr, LineClassifier *lineClassifier = nullptr );
    virtual Comments extract();
    virtual ~CommentExtractorImpl() {}

private:

    void newFile ( const string &fileName ) override;
    void newLine() override;
    void newPhantomLine() override;
    void newComment ( const string &author, const string &comment ) override;
    void setLine ( int lineNumber ) override;

    vector<string> getDiffLines();
    vector<pair<shared_ptr<CommentState>, const string *>> classifyLines ( const vector<string> &diffLines );
    void consumeLineContent ( const vector<pair<shared_ptr<CommentState>, const string *>> &classifiedLines ) const;

    void finishOwnState();

    GitClient *gitClient;
    LineClassifier *lineClassifier;

    string currentFile;
    unsigned int currentLine;
    unsigned int phantomLines;
    vector<LineComment> currentLineComments;
    vector<FileComments> fileComments;
};


#endif //MUSTARD_COMMENTEXTRACTOR_H

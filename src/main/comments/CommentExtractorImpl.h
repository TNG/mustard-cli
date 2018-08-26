#ifndef MUSTARD_COMMENTEXTRACTOR_H
#define MUSTARD_COMMENTEXTRACTOR_H
#include <map>
#include <regex>

using namespace std;

#include "../git/GitClient.h"
#include "Comments.h"
#include "CommenExtractor.h"

class CommentExtractorImpl : public CommentExtractor
{
public:
    CommentExtractorImpl ( GitClient *gitClient = nullptr );
    virtual Comments extract();
    virtual ~CommentExtractorImpl() {}
private:
    vector<string> getDiffLines();

    GitClient *gitClient;
    enum LineType { UNKNOWN, FILEDEFINITION, CONTEXTDEFINITION, ADDFILE, ADDLINE, DELLINE};
    static map<LineType, regex> lineTypeRegexes;

    LineType getLineType ( const string &line );

    void addFoundCommentsTo ( vector<FileComments> &fileComments, vector<LineComment> comments, string file );

    string extractAddedFileFrom ( const string &line );

    int extractContextLineNumber ( const string &line );

    string getSingleCaptureIn ( const string &text, const regex &extractor ) const;
};


#endif //MUSTARD_COMMENTEXTRACTOR_H

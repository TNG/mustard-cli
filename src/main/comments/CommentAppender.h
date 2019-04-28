#ifndef MUSTARD_COMMENTAPPENDER_H
#define MUSTARD_COMMENTAPPENDER_H


#include <vector>
#include <regex>
#include "CommentConsumer.h"
#include "LineComment.h"

class CommentAppender : public CommentConsumer
{
public:
    void consume ( const string &fileName, const LineComment &lineComment ) override;
    void finish();
private:
    void finishFile();
    string openFileName = "";
    vector<LineComment> openFileComments;
    const unsigned int indentationDepth = 8;

    void insertMultiLineComment ( vector<string> &fileLines, const LineComment &comment );

    void insertSingleLineComment ( vector<string> &vector, const LineComment &comment );
    regex getIndentationRegexForIndentationLevel ( unsigned int indentationLevel );

    void formatComment ( const LineComment &comment, stringstream &ss, unsigned int indentationLevel,
                         optional<unsigned long> inReplyTo );
};


#endif //MUSTARD_COMMENTAPPENDER_H

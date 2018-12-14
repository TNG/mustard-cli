#ifndef MUSTARD_COMMENTAPPENDER_H
#define MUSTARD_COMMENTAPPENDER_H


#include <vector>
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

    void insertMultiLineComment ( vector<string> &fileLines, const LineComment &comment );

    void insertSingleLineComment ( vector<string> &vector, const LineComment &comment );
};


#endif //MUSTARD_COMMENTAPPENDER_H

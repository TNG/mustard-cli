#ifndef MUSTARD_STOPREVIEWWORKFLOW_H
#define MUSTARD_STOPREVIEWWORKFLOW_H


#include "Workflow.h"
#include "../comments/CommenExtractor.h"

class StopReviewWorkflow : public Workflow
{
public:
    StopReviewWorkflow ( CommentExtractor *commentExtractor = nullptr ) ;

    int run ( int argc, const char **argv ) override;
private:
    CommentExtractor *commentExtractor;
};


#endif //MUSTARD_STOPREVIEWWORKFLOW_H

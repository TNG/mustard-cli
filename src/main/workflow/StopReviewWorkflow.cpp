#include <Depend.h>
#include "StopReviewWorkflow.h"

StopReviewWorkflow::StopReviewWorkflow ( CommentExtractor *commentExtractor ) :
    commentExtractor ( DependentOn<CommentExtractor> ( commentExtractor ) )
{}

int StopReviewWorkflow::run ( int argc, const char **argv )
{
    const Comments comments = commentExtractor->extract();
    return 0;
}

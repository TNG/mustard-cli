
#include "workflow/StartReviewWorkflow.h"
#include "error/MustardException.h"
#include "workflow/WorkflowFactory.h"
#include "workflow/WorkflowException.h"

void printUsage();

int main ( const int argc, const char **argv )
{
    WorkflowFactory workflowFactory;
    try {
        Workflow &workflow = workflowFactory.getWorkflowFor ( argc, argv );
        return workflow.run ( argc, argv );
    } catch ( WorkflowException exception ) {
        printf ( "%s\n", exception.what() );
        printUsage();
        return -1;
    } catch ( MustardException exception ) {
        printf ( "Error: %s\n", exception.what() );
        return -1;
    }
}

void printUsage()
{
    printf ( "Usage: mustard start\n" );
}
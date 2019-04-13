#include <iostream>
#include "workflow/StartReviewWorkflow.h"
#include "error/MustardException.h"
#include "workflow/WorkflowFactory.h"
#include "workflow/WorkflowException.h"

using namespace std;

void printUsage ( WorkflowFactory &factory );

int main ( const int argc, const char **argv )
{
    WorkflowFactory workflowFactory;
    try {
        Workflow &workflow = workflowFactory.getWorkflowFor ( argc, argv );
        return workflow.run ( argc, argv );
    } catch ( WorkflowException exception ) {
        printf ( "%s\n", exception.what() );
        printUsage ( workflowFactory );
        return -1;
    } catch ( MustardException exception ) {
        printf ( "Error: %s\n", exception.what() );
        return -1;
    }
}

void printUsage ( WorkflowFactory &factory )
{
    cout << "Usage: mustard [" ;
    bool notFirst = false;
    for ( auto availableCommand : factory.availableCommands() ) {
        if ( notFirst ) {
            cout << "|";

        }
        notFirst = true;
        cout << availableCommand;
    }
    cout << "]\n";
}
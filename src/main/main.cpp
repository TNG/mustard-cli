
#include "workflow/StartReviewWorkflow.h"
#include "error/MustardException.h"

int main ( const int argc, const char **argv )
{
    try {
        StartReviewWorkflow startReviewWorkflow;
        return startReviewWorkflow.run ( argc, argv );
    } catch ( MustardException exception ) {
        printf ( "Error:\n%s\n", exception.what() );
        return -1;
    }
}
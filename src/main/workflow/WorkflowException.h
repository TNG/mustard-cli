#ifndef MUSTARD_WORKFLOWEXCEPTION_H
#define MUSTARD_WORKFLOWEXCEPTION_H

#include "../error/MustardException.h"

class WorkflowException : public MustardException
{
public:
    WorkflowException ( const char *reason ) : MustardException ( reason ) {}
};

#endif //MUSTARD_WORKFLOWEXCEPTION_H

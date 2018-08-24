#ifndef MUSTARD_WORKFLOW_H
#define MUSTARD_WORKFLOW_H

class Workflow
{
public:
    virtual int run ( int argc, const char **argv ) = 0;
};

#endif //MUSTARD_WORKFLOW_H

#ifndef MUSTARD_WORKFLOWENGINE_H
#define MUSTARD_WORKFLOWENGINE_H


#include <functional>
#include <map>
#include <vector>
#include "Workflow.h"

using namespace std;

class WorkflowFactory
{
public:
    Workflow &getWorkflowFor ( int argc, const char **argv );
    vector<string> availableCommands();
    ~WorkflowFactory();

private:
    static map<string, std::function<Workflow *() >> workflowConstructors;
    vector<Workflow *> generatedWorkflows;
};


#endif //MUSTARD_WORKFLOWENGINE_H

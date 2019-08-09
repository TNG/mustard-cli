#include "WorkflowFactory.h"
#include "WorkflowException.h"
#include "StartReviewWorkflow.h"
#include "StopReviewWorkflow.h"
#include "InfoWorkflow.h"
#include "AutoconfWorkflow.h"
#include "PasswdWorkflow.h"

map<string, std::function<Workflow*() >> WorkflowFactory::workflowConstructors = {
    {
        "start", []()
        {
            return new StartReviewWorkflow();
        }
    }, {"stop", []()
        {
            return new StopReviewWorkflow();
        }
    }, {"info", []()
        {
            return new InfoWorkflow();
        }
    }, {"autoconf", []()
        {
            return new AutoconfWorkflow();
        }
    }, {"passwd", []()
        {
            return new PasswdWorkflow();
        }
    }
};

Workflow &WorkflowFactory::getWorkflowFor ( int argc, const char **argv )
{
    if ( argc < 2 ) {
        throw WorkflowException ( "Missing commandline arguments" );
    }
    auto workflowConstructor = workflowConstructors.find ( argv[1] );
    if ( workflowConstructor == workflowConstructors.end() ) {
        string reason ( "Unknown command: " );
        throw WorkflowException ( ( reason + argv[1] ).c_str() );
    }
    Workflow *workflow = workflowConstructor->second();
    generatedWorkflows.push_back ( workflow );
    return *workflow;
}

WorkflowFactory::~WorkflowFactory()
{
    for ( auto workflow : generatedWorkflows ) {
        delete workflow;
    }
}

vector<string> WorkflowFactory::availableCommands()
{
    vector<string> availableCommands;
    for ( auto workflow : workflowConstructors ) {
        availableCommands.push_back ( workflow.first );
    }
    sort ( availableCommands.begin(), availableCommands.end() );
    return availableCommands;
}

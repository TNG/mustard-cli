#include <Provide.h>
#include "CommandlineConfiguration.h"
#include "../comments/commentState/RegexMatcher.h"

ProvideDependency<CommandlineConfiguration> commandLineConfigurationDependency;

void CommandlineConfiguration::parse ( const int argc, const char **argv )
{
    if ( argc <= 1 ) {
        return;
    }

    RegexMatcher debugMatcher ( "--debug" );

    for ( int a = 1; a < argc; ++a ) {
        if ( debugMatcher.isMatching ( argv[a] ) ) {
            debug = true;
        }
    }
}

bool CommandlineConfiguration::isDebug()
{
    return debug;
}

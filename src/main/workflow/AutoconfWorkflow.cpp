#include <Depend.h>
#include "AutoconfWorkflow.h"
#include "../git/GitClientException.h"
#include "../system/UserConfirmation.h"

AutoconfWorkflow::AutoconfWorkflow ( BitBucketConfigGuesser *guesser, GitClient *gitClient ) :
    guesser ( DependentOn<BitBucketConfigGuesser> ( guesser ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) )
{
}

int AutoconfWorkflow::run ( int argc, const char **argv )
{
    guesser->guess();
    suggestIfNotConfigured ( "Bitbucket URL", "mustard.bitbucket", guesser->getServer() );
    suggestIfNotConfigured ( "repository slug", "mustard.repositorySlug", guesser->getRepositorySlug() );
    suggestIfNotConfigured ( "project key", "mustard.projectKey", guesser->getProjectKey() );
    return 0;
}

void AutoconfWorkflow::suggestIfNotConfigured ( const char *explanation, const char *key,
        const string &guessedValue )
{
    try {
        string value = gitClient->getConfigValue ( key );
        cout << explanation << " is set to " << value << endl;
    } catch ( GitClientException exception ) {
        suggest ( explanation, key, guessedValue );
    }
}

void AutoconfWorkflow::suggest ( const char *explanation, const char *key, const string &guessedValue )
{
    cout << key << " has not been configured." << endl
         << " I'd guess that your " << explanation << " is " << guessedValue << endl;
    if ( UserConfirmation ( "Is that correct?" ).askUser() == YES ) {
        gitClient->setConfigValue ( key, guessedValue );
    } else {
        cout << "You can configure this value later using 'git config --add " << key << " <value>'" << endl;
    };
}

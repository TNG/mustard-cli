#ifndef MUSTARD_BITBUCKETCONFIGGUESSER_H
#define MUSTARD_BITBUCKETCONFIGGUESSER_H

#include <regex>
#include "../git/GitClient.h"

using namespace std;
class BitBucketConfigGuesser
{
public:
    BitBucketConfigGuesser ( GitClient *gitClient = nullptr );
    void guess();
    string getServer() {
        return server;
    }

    string getRepositorySlug() {
        return repositorySlug;
    }

    string getProjectKey() {
        return projectKey;
    }

private:
    string server;
    string repositorySlug;
    string projectKey;
    GitClient *gitClient;

    string getSingleCaptureIn ( const string &text, const regex &extractor ) const;
};


#endif //MUSTARD_BITBUCKETCONFIGGUESSER_H

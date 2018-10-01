#ifndef MUSTARD_BITBUCKETCOMMENTUPLOADER_H
#define MUSTARD_BITBUCKETCOMMENTUPLOADER_H


#include "../comments/CommentConsumer.h"
#include "../system/HttpClient.h"
#include "BitBucketConfiguration.h"
#include "PullRequest.h"

class BitBucketCommentUploader : public CommentConsumer
{
public:
    BitBucketCommentUploader ( PullRequest pullRequest, HttpClient *httpClient = nullptr,  BitBucketConfiguration *bitBucketConfiguration = nullptr );

    void consume ( const string &file, unsigned int line, const string &comment ) override;

private:
    PullRequest pullRequest;
    HttpClient *httpClient;
    BitBucketConfiguration *bitBucketConfiguration;

    string getCommentPostUrl() const;

    string buildCommentPostUrl() const;

    string serializeComment ( const string &file, unsigned int line, const string &comment ) const;
};


#endif //MUSTARD_BITBUCKETCOMMENTUPLOADER_H

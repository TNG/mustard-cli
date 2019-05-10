#ifndef MUSTARD_BITBUCKETCOMMENTUPLOADER_H
#define MUSTARD_BITBUCKETCOMMENTUPLOADER_H


#include "../comments/CommentConsumer.h"
#include "../system/HttpClient.h"
#include "BitBucketConfiguration.h"
#include "model/PullRequest.h"
#include "../comments/LineComment.h"

class BitBucketCommentUploader : public CommentConsumer
{
public:
    BitBucketCommentUploader ( PullRequest pullRequest, HttpClient *httpClient = nullptr,  BitBucketConfiguration *bitBucketConfiguration = nullptr );

    void consume ( const string &file, const LineComment &lineComment ) override;
    void consume ( const string &file, const LineComment &lineComment, optional<unsigned long> inReplyTo );
    unsigned int getUploadedCommentNumber() {
        return uploaded;
    }
    unsigned int getSeenCommentNumber() {
        return seen;
    }

private:
    PullRequest pullRequest;
    HttpClient *httpClient;
    BitBucketConfiguration *bitBucketConfiguration;

    string getCommentPostUrl() const;

    string buildCommentPostUrl() const;

    string serializeComment ( const string &file, unsigned int line, const string &comment, const optional<unsigned long> replyToId ) const;

    unsigned int uploaded = 0;
    unsigned int seen = 0;

    void uploadComment ( const string &file, unsigned int line, const string &comment,
                         const optional<unsigned long> inReplyTo );
};


#endif //MUSTARD_BITBUCKETCOMMENTUPLOADER_H

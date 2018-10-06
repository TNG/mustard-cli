#include <Depend.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "BitBucketCommentUploader.h"

using namespace rapidjson;

BitBucketCommentUploader::BitBucketCommentUploader ( PullRequest pullRequest,
        HttpClient *httpClient,
        BitBucketConfiguration *bitBucketConfiguration ) :
    pullRequest ( pullRequest ),
    httpClient ( DependentOn<HttpClient> ( httpClient ) ),
    bitBucketConfiguration ( DependentOn<BitBucketConfiguration> ( bitBucketConfiguration ) )
{}

void BitBucketCommentUploader::consume ( const string &file, unsigned int line, const string &comment )
{
    const string postUrl = getCommentPostUrl();
    const string json = serializeComment ( file, line, comment );
    httpClient->post ( postUrl, json );
}

string BitBucketCommentUploader::getCommentPostUrl() const
{

    static const string finalUrl = buildCommentPostUrl();
    return finalUrl;
}

string BitBucketCommentUploader::buildCommentPostUrl() const
{
    stringstream ss;
    ss << bitBucketConfiguration->getPullRequestEndpoint();
    ss << "/" << pullRequest.id << "/comments";
    return ss.str();
}

string BitBucketCommentUploader::serializeComment ( const string &file, unsigned int line, const string &comment ) const
{
    Document jsonDocument;
    Value root ( kObjectType );
    jsonDocument.SetObject();
    Value text, mLine, lineType, fileType, path, srcPath;
    text.SetString ( comment.c_str(), jsonDocument.GetAllocator() );
    mLine.SetInt ( line );
    fileType.SetString ( "TO" );
    lineType.SetString ( "ADDED" );
    path.SetString ( file.c_str(), jsonDocument.GetAllocator() );

    Value anchor;
    anchor.SetObject();

    anchor.AddMember ( "line", line, jsonDocument.GetAllocator() );
    anchor.AddMember ( "lineType", lineType, jsonDocument.GetAllocator() );
    anchor.AddMember ( "fileType", fileType, jsonDocument.GetAllocator() );
    anchor.AddMember ( "path", Value ( path, jsonDocument.GetAllocator() ), jsonDocument.GetAllocator() );
    anchor.AddMember ( "srcPath", path, jsonDocument.GetAllocator() );

    root.AddMember ( "text", text, jsonDocument.GetAllocator() );
    root.AddMember ( "anchor", anchor, jsonDocument.GetAllocator() );

    StringBuffer buffer;
    Writer<StringBuffer> writer ( buffer );
    root.Accept ( writer );
    return buffer.GetString();
}

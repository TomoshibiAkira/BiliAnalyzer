#ifndef STATUS_H
#define STATUS_H

#define URL_NOT_PARSED 0                    // This video has not been parsed yet.
#define URL_PARSED_OK 1                     // This video has been parsed correctly.
#define URL_PARSED_VIDEO_NOT_FOUND 2        // 404 a.k.a Cannot find the video ( video has been removed or ... )
#define URL_PARSED_NETWORK_BAD 3            // Cannot connect to host or other network error.
#define URL_PARSED_REDIRECTION 4            // URL is requesting redirection ( hardly used )
#define URL_PARSED_HTML_BAD 5               // Cannot read CID from HTML ( maybe needs user login )
#define URL_PARSED_XML_BAD 6                // Cannot read XML from interface ( almost impossible )

#endif // STATUS_H

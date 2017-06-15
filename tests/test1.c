#include "../src/networkstream.h"
#include "../src/debug.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define REMOTE_HOST "www.csdn.net"
#define REMOTE_URL "/"
#define HTTP_VERSION "HTTP/1.1"
#define SPACE " "
#define HTTP_METHOD "GET"
#define CRLF "\r\n"
#define CONTENT_LENGTH "Content-Length: "

static int find_header_length(char const *buffer, size_t buflen) {
    char *p = NULL;
    puts(buffer);
    p = strstr(buffer, CRLF CRLF);
    if (!p)
        return -1;
    return p + 4 - buffer;
}

static long find_content_length(char const *http_header, size_t header_len) {
    char *p = NULL;
    long len = 0;
    p = strstr(http_header, CONTENT_LENGTH);
    if (!p)
        return -1L;
    if (sscanf(p + strlen(CONTENT_LENGTH), "%ld", &len) != 1)
        return -1L;
    return len;
}

int main(int argc, char* *argv) {
    int rc = 0;
    int header_length = 0;
    long content_length = 0;
    char buf[0x1ff] = {0};
    char const *HTTP_REQUEST = HTTP_METHOD SPACE "http://" REMOTE_HOST REMOTE_URL SPACE HTTP_VERSION CRLF
        "Host:" SPACE REMOTE_HOST CRLF
        CRLF;
    size_t HTTP_REQUEST_SIZE = strlen(HTTP_REQUEST);
    NetworkStream *stream = NULL;

    stream = NetworkStreamCreate(REMOTE_HOST, 80);
    if (!stream)
        goto failure;

    if ((rc = NetworkStreamWrite(stream, (void*) HTTP_REQUEST, HTTP_REQUEST_SIZE)) != HTTP_REQUEST_SIZE)
        goto failure;

    if ((rc = NetworkStreamRead(stream, &buf[0], sizeof(buf) - 1)) < 1)
       goto failure;
    buf[rc] = '\0';
    if ((header_length = find_header_length(&buf[0], rc)) < 0) {
        printf("Header length is %d\n", header_length);
        goto failure;
    }
    if ((content_length = find_content_length(&buf[0], header_length)) < 0) {
        printf("Content-Length is %ld\n", content_length);
        goto failure;
    }

    content_length += header_length;
    do {
        buf[rc] = '\0';
        printf("%s", &buf[0]);
        content_length -= rc;
    } while (content_length > 0 && (rc = NetworkStreamRead(stream, &buf[0], sizeof(buf) - 1)) > 0);
    printf("\ncontent_length = %ld\n", content_length);

    goto success;
exit:
    return rc;
success:
    rc = EXIT_SUCCESS;
    goto cleanup;
failure:
    rc = EXIT_FAILURE;
    goto cleanup;
cleanup:
    if (stream) {
        NetworkStreamDestroy(stream);
        stream = NULL;
    }
    goto exit;
}

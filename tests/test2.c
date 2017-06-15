#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../src/networkstream.h"
#include "../src/debug.h"

#define SPACE " "
#define DUBBO_CMD_INVOKE "invoke"
#define SERVICE "org.sqg.HelloWorld.greetings"
#define CUSTOMER "QXWZ Inc. LTD,."
#define REQUEST DUBBO_CMD_INVOKE SPACE SERVICE "(\"" CUSTOMER "\")\n"

#define TEST_ASSERT(CONDITION) \
    if (!(CONDITION)) { \
        fprintf(stderr, "[ASSERT]: %s failed!\n", #CONDITION); \
        goto failure; \
    }

char* strrstr(char const *search, char const *substr) {
    size_t slen = 0;
    char *loc = NULL;

    slen = strlen(substr);
    if (slen < 1)
        return loc;
    loc = strrchr(search, *substr);
    if (strncmp(loc, substr, slen) == 0)
        return loc;
}

int main(int argc, char* *argv) {
    int rc = 0;
    char buf[0xff];
    NetworkStream *stream = NULL;

    TEST_ASSERT((stream = NetworkStreamCreate("127.0.0.1", 54321)) != NULL);
    TEST_ASSERT(NetworkStreamWrite(stream, (void const*) REQUEST, strlen(REQUEST)) == strlen(REQUEST));
    TEST_ASSERT((rc = NetworkStreamRead(stream, &buf[0], sizeof(buf) - 1)) > 0);
    buf[sizeof(buf) - 1] = '\0';
    printf("[REQUEST ] %s\n", REQUEST);
    printf("[RESPONSE] rc = %d\n", rc);
    printf("[RESPONSE] %s\n", &buf[0]);

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

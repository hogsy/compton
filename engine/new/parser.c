
#include "parser.h"

#include <jansson.h>

typedef struct DEFScript {
    char *buf;

    unsigned int length;
} DEFScript;

DEFScript *LoadDEFScript(const char *path) {
    if(!plFileExists(path)) {
        return NULL;
    }

    FILE *file = fopen(path, "r");
    if(!file) {
        PRINT_ERROR("Failed to load DEF script! (%s)\n", path);
    }

#if 1
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if(length <= 0) {
        PRINT_ERROR("Encountered an invalid or empty DEF script! (%s : length(%d))\n", path, length);
    }
#else
    struct stat st;
    stat(path, &st);
    length = st.st_size;
#endif

    char *buf = calloc((size_t)length, sizeof(char));
    if(!buf) {
        PRINT_ERROR("Failed to allocate memory for buffer! (%s : length(%d))\n", path, length);
    }

    fclose(file);

    DEFScript *script = malloc(sizeof(DEFScript));
    if(!script) {
        PRINT_ERROR("Failed to allocate memory for DEFScript instance! (%s : length(%d))", path, length);
    }

    memset(script, 0, sizeof(DEFScript));

    script->buf = calloc((size_t)length, sizeof(char));
    if(!script->buf) {
        PRINT_ERROR("Failed to allocate memory for buffer! (%s : length(%d))\n", path, length);
    }

    memmove(script->buf, buf, (size_t)length);
    free(buf);

    return script;
}

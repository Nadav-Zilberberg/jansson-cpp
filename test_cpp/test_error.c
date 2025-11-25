#include "src/jansson.h"
#include <stdio.h>

int main() {
    json_error_t error;
    json_error_init(&error, "test.c");
    json_error_set(&error, 1, 2, 3, json_error_invalid_syntax, "Test error: %s", "invalid syntax");
    
    printf("Error source: %s\n", error.source);
    printf("Error line: %d\n", error.line);
    printf("Error column: %d\n", error.column);
    printf("Error position: %d\n", error.position);
    printf("Error text: %s\n", error.text);
    printf("Error code: %d\n", json_error_get_code(&error));
    
    return 0;
}

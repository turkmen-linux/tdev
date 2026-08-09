#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <tdev.h>
#include <time.h>
#include <unistd.h>

#ifdef DEBUG
static int modprobe_fn(char **args) {
    for (size_t i = 0; args[i]; i++) {
        if (strcmp(args[i], "MODALIAS=") == 0) {
            pid_t pid = fork();
            if (pid) {
                char *argv[] = { "modprobe", "-ab", args[i] + 9, NULL };
                execv("/sbin/modprobe", argv);
            }
        }
    }
    printf("\n");
    return 0;
}
#endif

visible void plugin_init() {
#ifdef DEBUG
    register_handler((handler_fn) modprobe_fn);
#endif
}

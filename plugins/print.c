#include <stddef.h>
#include <stdio.h>
#include <tdev.h>
#include <time.h>

#ifdef DEBUG
static int print_fn(char**args){
    time_t now = time(NULL);
    const struct tm *t = localtime(&now);
    printf("========= %d:%d:%d =========\n", t->tm_hour, t->tm_min, t->tm_sec);
    for(size_t i=0; args[i]; i++){
        printf("%s\n", args[i]);
    }
    printf("\n");
    return 0;
}
#endif

visible void plugin_init(){
#ifdef DEBUG
    register_handler((handler_fn) print_fn);
#endif
}
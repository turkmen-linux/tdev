#include <stddef.h>
#include <stdio.h>
#include <tdev.h>

#ifdef DEBUG
static int print_fn(char**args){
    for(size_t i=0; args[i]; i++){
        printf("%s\n", args[i]);
    }
    return 0;
}
#endif

visible void plugin_init(){
#ifdef DEBUG
    register_handler((handler_fn) print_fn);
#endif
}
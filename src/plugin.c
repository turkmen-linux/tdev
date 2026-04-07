#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <dlfcn.h>

#include "tdev.h"

extern handler_fn *handler;
static int total = 0;
static int cur = 0;

visible void register_handler(handler_fn h){
    // init if required
    if(!handler){
        total = 32;
        handler = malloc(total*sizeof(handler_fn));
        if(!handler){
            perror("malloc");
            return;
        }
    }
    // reallocate if required
    if (total <= cur){
        total += 32;
        handler_fn* tmp = realloc(handler, total*sizeof(handler_fn));
        if(tmp){
            handler = tmp;
        } else {
            perror("malloc");
            return;
        }
    }
    debug("Register handler: %p\n", &h);
    handler[cur] = h;
}

void load_plugin(const char* path){
    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        printf("Failed to load plugin: %s from %s\n",dlerror(), path);
        return;
    }
    dlerror();
    void (*plugin_func)(void);
    *(void**)(&plugin_func) = dlsym(handle, "plugin_init");
    if(!plugin_func){
        return;
    }
    plugin_func();
}


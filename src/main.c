#include <stddef.h>
#include <stdio.h>

#include "tdev.h"

int test(char**args){
    for(size_t i=0; args[i]; i++){
        printf("%s\n", args[i]);
    }
    return 0;
}

int main(int argc, char** argv){
    handler = (handler_fn) test;
    return netlink_main();
}
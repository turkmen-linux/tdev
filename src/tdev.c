#include <stdio.h>
#include <string.h>
#include <dirent.h>

extern int netlink_main();
extern void load_plugin(const char* path);

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

static void load_plugins(){
    // load plugins
    DIR *d = opendir(PLUGINDIR);
    if (!d) {
        perror("opendir");
        return;
    }
    const struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) {
            continue;
        }
        const char* ext = (e->d_name+strlen(e->d_name)-3);
        // load only libtdev_xxx.so
        if(strncmp("libtdev_", e->d_name, 8) != 0 || strncmp(ext, ".so", 3) != 0){
            continue;
        }
        char plugin[PATH_MAX];
        strcpy(plugin, PLUGINDIR);
        strcat(plugin, "/");
        strcat(plugin, e->d_name);
        load_plugin(plugin);
        printf("Plugin loaded: %s\n", plugin);
    }
    closedir(d);
}
visible void tdev_main(){
    load_plugins();
    netlink_main();
    return;
}

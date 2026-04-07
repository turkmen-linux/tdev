#ifndef	 _tdev_h
#define _tdev_h
typedef int (*handler_fn)(char** envs);
void register_handler(handler_fn h);
int tdev_main(int argc, char** argv);
#endif

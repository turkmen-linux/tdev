#ifndef	 _tdev_h
#define _tdev_h
typedef int (*handler_fn)(char** envs);
extern handler_fn handler;
int netlink_main();
#endif
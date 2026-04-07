#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#include "tdev.h"

#define MAX_PAYLOAD 1024  /* maximum payload size*/
static struct sockaddr_nl src_addr, dest_addr;
static struct nlmsghdr *nlh = NULL;
static struct iovec iov;
static int sock_fd;
static struct msghdr msg;

handler_fn *handler;

int netlink_main() {
    int ret;

    debug("Creating socket\n");
    sock_fd=socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if(sock_fd<0) {
        printf("Socket creating failed\n");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();  /* self pid */
    src_addr.nl_groups = 1;
    /* interested in group 1<<0 */
    ret = bind(sock_fd, (struct sockaddr*)&src_addr,
               sizeof(src_addr));
    if (ret < 0) {
        printf("Bind Failed\n");
        perror("bind:");
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;   /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;   /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if(!nlh){
        perror("malloc");
        return -1;
    }
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_flags = 0;
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    debug("Waiting for message from kernel\n");
    /* Read message from kernel */
    while (1) {
        ssize_t len = recvmsg(sock_fd, &msg, 0);
        if (len < 0) {
            perror("recvmsg");
            break;
        }

        char *payload = NLMSG_DATA(nlh);
        const char *end = payload + len;
        char *p = payload;

        len = 0;

        //count len
        while (p < end && *p) {
            size_t sl = strlen(p);
            p += sl + 1;
            len++;
        }

        // create array then reset
        char* arr[len] = {};
        len = 0;
        p = payload;

        // fill array
        while (p < end && *p) {
            len++;
            arr[len-1] = p;
            size_t sl = strlen(p);
            p += sl + 1;
        }
        arr[len] = NULL;

        // run handler
        if(handler){
            for(size_t i=0; handler[i]; i++){
                if(handler[i](arr)){
                    perror("handler");
                }
            }
        }
    }
    close(sock_fd);
    return 0;
}
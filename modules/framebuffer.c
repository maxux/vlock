/* framebuffer.c -- a framebuffer wallpaper support for vlock,
 *                  the VT locking program for linux
 *
 * This program is copyright (C) 2019 Maxime Daniel, and is free
 * software which is freely distributable under the terms of the
 * GNU General Public License version 2, included as the file COPYING in this
 * distribution.  It is NOT public domain software, and any
 * redistribution not permitted by the GNU General Public License is
 * expressly forbidden without prior written permission from
 * the author.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define IMAGE_FILE ".vlock/framebuffer.raw"

#include "vlock_plugin.h"

const char *depends[] = { "all", NULL };
const char *conflicts[] = { "blank", NULL };

// nothing which goes wrong are fatal
// we never return 1

bool vlock_start(void __attribute__ ((__unused__)) **ctx) {
    char imgpath[512], buffer[2048];
    char *userhome;
    int fdimg, fdfb;
    ssize_t length;

    if(!(userhome = getenv("HOME")))
        return true;

    snprintf(imgpath, sizeof(imgpath), "%s/%s", userhome, IMAGE_FILE);

    if((fdfb = open("/dev/fb0", O_WRONLY)) < 0) {
        perror("/dev/fb0");
        return true;
    }

    if((fdimg = open(imgpath, O_RDONLY)) < 0) {
        perror(imgpath);
        close(fdfb);
        return true;
    }

    while((length = read(fdimg, buffer, sizeof(buffer))) > 0) {
        if(write(fdfb, buffer, length) != length) {
            perror("write");
        }
    }

    close(fdimg);
    close(fdfb);

    return true;
}

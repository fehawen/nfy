#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

int main(int argc, char *argv[]) {
    char *msg = argc > 1 && strcmp(argv[1], "") != 0
        ? argv[1]
        : "No message to display.";

    const int place = argc > 2 ? atoi(argv[2]) : 0;
    const int charsize = 32;
    const int winheight = 96;
    const int wingap = 48;

    int winoffset = wingap;

    Display *d = XOpenDisplay(NULL);

    if (d == NULL) {
        fprintf(stderr, "Failed to open display.\n");
        exit(1);
    }

    int s = DefaultScreen(d);

    XWindowAttributes xwa;
    XGetWindowAttributes(d, RootWindow(d, s), &xwa);
    const int screenwidth = xwa.width;
    const int screenheight = xwa.height;

    if (place == 1) {
        winoffset = (wingap * 2) + winheight;
    } else if (place > 1) {
        winoffset = (wingap * place) + (winheight * place) + wingap;
    }

    if ((winoffset + winheight) >= screenheight) {
        fprintf(stderr, "Message list is longer than screen height.\n");
        exit(0);
    }

    unsigned long getcolor(const char *col) {
        Colormap m = DefaultColormap(d, s);
        XColor c;
        return (!XAllocNamedColor(d, m, col, &c, &c)) ? 0 : c.pixel;
    }

    const int winlen = (strlen(msg) * 16) + (charsize * 2);

    if ((winlen + wingap) >= screenwidth) {
        fprintf(stderr, "Message is longer than the screen width.\n");
        exit(0);
    }

    Window w = XCreateSimpleWindow(d, RootWindow(d, s), wingap, winoffset, winlen, winheight, 0,
                                    getcolor("#a3ebf5"), getcolor("#192d36"));

    XSelectInput(d, w, ExposureMask | KeyReleaseMask | ButtonReleaseMask);

    XFontStruct *font = XLoadQueryFont(d, "-xos4-terminus-medium-r-normal--32-320-72-72-c-160-iso10646-1");

    XGCValues values;
    values.font = font->fid;
    values.foreground = getcolor("#a3ebf5");

    GC context = XCreateGC(d, w, GCFont+GCForeground, &values);

    XMapWindow(d, w);
    XStoreName(d, w, "nfy");

    XEvent ev;
    while (1) {
        XNextEvent(d, &ev);

        if (ev.type == Expose) {
            XDrawString(d, w, context, charsize, 56, msg, strlen(msg));
        }

        if (ev.type == KeyRelease) {
            if (XLookupKeysym(&ev.xkey, 0) == XK_Tab) {
                break;
            }
        }

        if (ev.type == ButtonRelease) {
            break;
        }
    }

    XUnmapWindow(d, w);
    XDestroyWindow(d, w);
    XCloseDisplay(d);

    return 0;
}

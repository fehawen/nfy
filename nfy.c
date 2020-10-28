#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

int main(int argc, char *argv[]) {
    Display *d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Failed to open display.\n");
        exit(1);
    }

    int s = DefaultScreen(d);

    unsigned long getcolor(const char *col) {
        Colormap m = DefaultColormap(d, s);
        XColor c;
        return (!XAllocNamedColor(d, m, col, &c, &c)) ? 0 : c.pixel;
    }

    Window w = XCreateSimpleWindow(d, RootWindow(d, s), 48, 48, 900, 96, 0,
                                    getcolor("#8cbeb8"), getcolor("#022527"));

    char *msg = argc > 1 ? argv[1] : "No message to display.";
    const int MAX = 48;
    int len = strlen(msg);
    if (len > MAX) {
        msg[len - (len - MAX)] = 0;
        strcat(msg, "...");
        len = strlen(msg);
    }

    XSelectInput(d, w, ExposureMask | KeyReleaseMask | ButtonReleaseMask);

    XFontStruct *font = XLoadQueryFont(d, "-xos4-terminus-medium-r-normal--32-320-72-72-c-160-iso10646-1");

    XGCValues values;
    values.font = font->fid;
    values.foreground = getcolor("#8cbeb8");

    GC context = XCreateGC(d, w, GCFont+GCForeground, &values);

    XMapWindow(d, w);

    XStoreName(d, w, "nfy");

    XEvent ev;
    while (1) {
        XNextEvent(d, &ev);

        if (ev.type == Expose) {
            XDrawString(d, w, context, 32, 56, msg, len);
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

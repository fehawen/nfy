#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

int main(int argc, char *argv[]) {
    char *msg = argc > 1 ? argv[1] : "No message to display.";
    const int FONT_SIZE = 32;

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

    int winlen = (strlen(msg) * 16) + (FONT_SIZE * 2);

    Window w = XCreateSimpleWindow(d, RootWindow(d, s), 48, 48, winlen, 96, 0,
                                    getcolor("#8cbeb8"), getcolor("#022527"));

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
            XDrawString(d, w, context, FONT_SIZE, 56, msg, strlen(msg));
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

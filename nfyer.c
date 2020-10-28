#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

int main(int argc, char *argv[]) {
    char *msg = argc > 1 ? argv[1] : "No message to display.";

    const int PLACE = argc > 2 ? atoi(argv[2]) : 0;
    const int FONT_SIZE = 32;
    const int WIN_HEIGHT = 96;
    const int WIN_GAP = 48;

    int WIN_OFFSET = WIN_GAP;

    if (PLACE == 1) {
        WIN_OFFSET = (WIN_GAP * 2) + WIN_HEIGHT;
    } else if (PLACE > 1) {
        WIN_OFFSET = (WIN_GAP * PLACE) + (WIN_HEIGHT * PLACE) + WIN_GAP;
    }

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

    Window w = XCreateSimpleWindow(d, RootWindow(d, s), WIN_GAP, WIN_OFFSET, winlen, WIN_HEIGHT, 0,
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

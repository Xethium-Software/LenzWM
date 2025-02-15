#include <X11/Xlib.h>
#include <stdio.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);

    XSelectInput(display, root, SubstructureRedirectMask | SubstructureNotifyMask);

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == MapRequest) {
            XMapWindow(display, event.xmaprequest.window);
            XMoveResizeWindow(display, event.xmaprequest.window, 0, 0, 400, 300); 
        }
    }

    return 0;
}
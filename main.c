#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

void draw_titlebar(Display *dpy, Window win, const char *title, int width) {
    GC gc = XCreateGC(dpy, win, 0, NULL);
    XFontStruct *font = XLoadQueryFont(dpy, "fixed");
    
    XColor text_color, bg_color;
    Colormap cmap = DefaultColormap(dpy, DefaultScreen(dpy));
    XParseColor(dpy, cmap, "#FFFFFF", &text_color);
    XParseColor(dpy, cmap, "#333333", &bg_color);
    XAllocColor(dpy, cmap, &text_color);
    XAllocColor(dpy, cmap, &bg_color);

    XSetForeground(dpy, gc, bg_color.pixel);
    XSetBackground(dpy, gc, text_color.pixel);
    XSetFont(dpy, gc, font->fid);

    int text_width = XTextWidth(font, title, strlen(title));

    XFillRectangle(dpy, win, gc, 0, 0, width, 20);
    XSetForeground(dpy, gc, text_color.pixel);
    XDrawString(dpy, win, gc, (width - text_width)/2, 15, title, strlen(title));

    XFreeGC(dpy, gc);
    XFreeFont(dpy, font);
}

int main() {
    Display *dpy = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(dpy);
    
    // Get screen dimensions
    XWindowAttributes root_attrs;
    XGetWindowAttributes(dpy, root, &root_attrs);
    int screen_width = root_attrs.width;
    int screen_height = root_attrs.height;

    XSelectInput(dpy, root, SubstructureRedirectMask | SubstructureNotifyMask);

    XSetWindowAttributes attrs = {
        .event_mask = SubstructureRedirectMask | SubstructureNotifyMask
    };
    XChangeWindowAttributes(dpy, root, CWEventMask, &attrs);

    XEvent ev;
    while (1) {
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case MapRequest: {
                Window client = ev.xmaprequest.window;
                
                Window frame = XCreateSimpleWindow(dpy, root, 0, 0, 
                                                  screen_width, screen_height,
                                                  1, BlackPixel(dpy, 0),
                                                  WhitePixel(dpy, 0));
                
                XReparentWindow(dpy, client, frame, 0, 20);
                
                XSelectInput(dpy, frame, ExposureMask);
                XMapWindow(dpy, frame);
                XMapWindow(dpy, client);
                
                XMoveResizeWindow(dpy, client, 0, 20, 
                                 screen_width, screen_height - 20);

                XTextProperty name;
                char *title = "LenzWM Window";
                if (XGetWMName(dpy, client, &name)) {
                    title = (char *)name.value;
                }
                
                draw_titlebar(dpy, frame, title, screen_width);
                break;
            }
            
            case Expose:
                if (ev.xexpose.window != root) {
                    Window root_ret, parent_ret;
                    Window *children;
                    unsigned int nchildren;
                    XQueryTree(dpy, ev.xexpose.window, &root_ret,
                              &parent_ret, &children, &nchildren);
                    
                    char *title = "LenzWM Window";
                    XTextProperty name;
                    if (XGetWMName(dpy, parent_ret, &name)) {
                        title = (char *)name.value;
                    }
                    draw_titlebar(dpy, parent_ret, title, screen_width);
                }
                break;
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
#include <X11/Xlib.h>
#include <math.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#define STATUS_LEFT 0x01
#define STATUS_RIGHT 0x02
#define STATUS_UP 0x04
#define STATUS_DOWN 0x08
#define STATUS_MOVING 0xF
#define STATUS_TAPJUMP 0x100
#define STATUS_RUNNING 0x80000000

#define MIN(a,b) ({int __a = (a), __b = (b); (__a < __b) ? __a : __b; })
#define MAX(a,b) ({int __a = (a), __b = (b); (__a > __b) ? __a : __b; })

unsigned long long now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000000 + tv.tv_usec;
}

void main()
{
	int e;
	XEvent ev;
	Display *dpy;
	int status = 0, dmy;
	char key = 0, lastkey = 0;
	unsigned long long lasttime = 0;
	int repeats = 1;
	int starttime = 0;
	
	dpy = XOpenDisplay(NULL);
	if (!dpy)
	{
		printf("Display open failed; bailing out\n");
		exit(1);
	}
	
	if (!XTestQueryExtension(dpy, &dmy, &dmy, &dmy, &dmy))
	{
		printf("XTest NOT supported\n");
		exit(1);
	}
	
	printf("Grabbing keyboard...\n");
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("grave")), Mod1Mask, RootWindow(dpy, DefaultScreen(dpy)), True, GrabModeAsync, GrabModeAsync);
	
	while (1)
	{
		while (XCheckMaskEvent(dpy, 0xFFFFFFFF, &ev))
		{
			switch (ev.type)
			{
			case KeyPress:
			case KeyRelease:
			{
				XKeyEvent *ke = (XKeyEvent*)&ev;
				char *keystr = XKeysymToString(XKeycodeToKeysym(dpy, ke->keycode, 0));
				//printf("Key %s: %s\n", ev.type == KeyPress ? "PRESS  " : "RELEASE", keystr);
				
				if (!strcmp(keystr, "grave")) {
					if (ev.type == KeyPress)
					{
						XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("grave")), AnyModifier, RootWindow(dpy, DefaultScreen(dpy)), True, GrabModeAsync, GrabModeAsync);
						XAutoRepeatOff(dpy);
						status |= STATUS_RUNNING;
					} else {
						XUngrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("grave")), AnyModifier, RootWindow(dpy, DefaultScreen(dpy)));
						XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("grave")), Mod1Mask, RootWindow(dpy, DefaultScreen(dpy)), True, GrabModeAsync, GrabModeAsync);
						XAutoRepeatOn(dpy);
						status &= ~STATUS_RUNNING;
					}
				} else if (!strcmp(keystr, "u")) 
					XTestFakeButtonEvent(dpy, 1, ev.type == KeyPress, 0);
				else if (!strcmp(keystr, "o"))
					XTestFakeButtonEvent(dpy, 3, ev.type == KeyPress, 0);
				else if (!strcmp(keystr, "apostrophe") || !strcmp(keystr, "semicolon")) {
					if (ev.type == KeyPress)
						status |= STATUS_TAPJUMP;
					else
						status &= ~STATUS_TAPJUMP;
					repeats = 1;
				} else if (!strcmp(keystr, "j")) {
					if (ev.type == KeyPress)
					{
						status |= STATUS_LEFT;
						if ((lastkey == 'j') && ((now() - lasttime) < 250000) && (status & STATUS_TAPJUMP))
							XTestFakeRelativeMotionEvent(dpy, -100.0 * pow(repeats++, 0.8), 0, 0);
					} else
						status &= ~STATUS_LEFT;
				} else if (!strcmp(keystr, "l")) {
					if (ev.type == KeyPress)
					{
						status |= STATUS_RIGHT;
						if ((lastkey == 'l') && ((now() - lasttime) < 250000) && (status & STATUS_TAPJUMP))
							XTestFakeRelativeMotionEvent(dpy, 100.0 * pow(repeats++, 0.8), 0, 0);
					} else
						status &= ~STATUS_RIGHT;
				} else if (!strcmp(keystr, "i")) {
					if (ev.type == KeyPress)
					{
						status |= STATUS_UP;
						if ((lastkey == 'i') && ((now() - lasttime) < 250000) && (status & STATUS_TAPJUMP))
							XTestFakeRelativeMotionEvent(dpy, 0, -100.0 * pow(repeats++, 0.8), 0);
					} else
						status &= ~STATUS_UP;
				} else if (!strcmp(keystr, "k")) {
					if (ev.type == KeyPress)
					{
						status |= STATUS_DOWN;
						if (lastkey == 'k' && (now() - lasttime) < 250000 && (status & STATUS_TAPJUMP))
							XTestFakeRelativeMotionEvent(dpy, 0, 100.0 * pow(repeats++, 0.8), 0);
					} else
						status &= ~STATUS_DOWN;
				}
				if (strlen(keystr) == 1 && ev.type == KeyPress)
				{
					lastkey = keystr[0];
					lasttime = now();
				} else if (ev.type == KeyPress) {
					lastkey = 0;
					repeats = 0;
				}
				break;
			}
			default:
				printf("NFI what %d is\n", ev.type);
			}
		}
		if (!(status & STATUS_RUNNING))
			status = 0;
		if (!(status & STATUS_MOVING))	// if we haven't anything to do, don't sit and suck up cycles
		{
			XPeekEvent(dpy, &ev);
			starttime = now();
		}
		usleep(20000 - MIN(MAX(pow(13 * (now() - starttime), 0.6), 0), 18000));
		if (now() - lasttime > 250000)
			repeats = 1;
		if (status & STATUS_LEFT)
			XTestFakeRelativeMotionEvent(dpy, -2, 0, 0);
		if (status & STATUS_RIGHT)
			XTestFakeRelativeMotionEvent(dpy, 2, 0, 0);
		if (status & STATUS_UP)
			XTestFakeRelativeMotionEvent(dpy, 0, -2, 0);
		if (status & STATUS_DOWN)
			XTestFakeRelativeMotionEvent(dpy, 0, 2, 0);
	}
	
	
	exit(0);
}
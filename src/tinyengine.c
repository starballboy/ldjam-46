/**

	tinyengine by: @starballboy
	started april 15th, 2020

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.

**/

#ifdef __cplusplus
extern "C" {
#endif

#define TE_VERSION_MAJOR 1
#define TE_VERSION_MINOR 0
#define TE_VERSION_BUILD 0

/* COMPILE TIME PLATFORM DETECTION */

#if defined(TE_WIN32) || defined(TE_WIN32_UWP) ||  defined(TE_WIN32_UWP) || defined(TE_POSIX) || defined(TE_UNIX) || defined(TE_BSD) || defined(TE_LINUX) || defined(TE_APPLE) || defined(TE_ANDROID) ||  defined(TE_EMSCRIPTEN)
	#warning "tinyengine: User override of platform detection."
#elif defined(_WIN32)
    #define TE_WIN32
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
        #define TE_WIN32_UWP
    #else
        #define TE_WIN32_DESKTOP
    #endif
#else
    #define TE_POSIX

    #ifdef __unix__
        #define TE_UNIX
        #if defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
            #define TE_BSD
        #endif
    #endif
    #ifdef __linux__
        #define TE_LINUX
    #endif
    #ifdef __APPLE__
        #define TE_APPLE
    #endif
    #ifdef __ANDROID__
        #define TE_ANDROID
    #endif
    #ifdef __EMSCRIPTEN__
        #define TE_EMSCRIPTEN
    #endif
#endif

/* END COMPILE TIME PLATFORM DETECTION */

/* STANDARD TYPE DEFINITIONS */

#include <stdint.h>

typedef uint8_t			te_u8;
typedef uint16_t		te_u16;
typedef uint32_t		te_u32;
typedef uint64_t		te_u64;

typedef int8_t			te_i8;
typedef int16_t			te_i16;
typedef int32_t			te_i32;
typedef int64_t			te_i64;

typedef float				te_f32;
typedef double			te_f64;

#define TE_TRUE 1
#define TE_FALSE 0

typedef uint8_t			te_bool_u8;

#if !defined(NULL)
#define NULL 0
#endif

#define TE_D2STR(x) _TE_D2STR(x)
#define _TE_D2STR(x) #x

/* END STANDARD TYPE DEFINITIONS */

/* THREADING HEADER */

#if defined(TE_POSIX)
	// TODO: Check for MinGW pthreads aswell
	// TODO: Not all posix enviroments support pthreads.
	#define TE_PTHREADS
#endif

/* END THREADING HEADER */

/* DEBUG HEADER */

#ifdef TE_DEBUG

#if defined(TE_DEBUG_LEVEL_INFO)
	#define TE_DEBUG_LEVEL_WARNING
	#define TE_DEBUG_LEVEL_ERROR
	#define TE_DEBUG_OUTPUT_ENABLED
#elif defined(TE_DEBUG_LEVEL_WARNING)
	#define TE_DEBUG_LEVEL_ERROR
	#define TE_DEBUG_OUTPUT_ENABLED
#elif !defined(TE_DEBUG_LEVEL_NONE)
	#define TE_DEBUG_LEVEL_ERROR
	#define TE_DEBUG_OUTPUT_ENABLED
#else
	#define TE_DEBUG_LEVEL_NONE
#endif

#ifndef TE_DEBUG_LOG_PREFIX
	#define TE_DEBUG_LOG_PREFIX
#endif

#define _TE_DEBUG_INFO "[" TE_D2STR(__FILE__) ":" TE_D2STR(__LINE__) "]"

#if defined(TE_DEBUG_OUTPUT_ENABLED)
	#include <stdio.h> // FILE
	void _tinyengine_debug_fprintf(FILE* stream, const char* prefix, const char * format, ...);
	#if defined(TE_LINUX)
		void _tinyengine_debugPrintStackTrace(const char* prefix);
	#endif
#endif

#if defined(TE_DEBUG_LEVEL_INFO)
	#define TE_LOG(f_, ...)		_tinyengine_debug_fprintf(stdout, TE_D2STR(TE_DEBUG_LOG_PREFIX) "[LOG]" _TE_DEBUG_INFO " ",(f_), ##__VA_ARGS__)
#else
	#define TE_LOG(f_, ...)
#endif

#if defined(TE_DEBUG_LEVEL_WARNING)
	#define TE_WARN(f_, ...)	_tinyengine_debug_fprintf(stdout, TE_D2STR(TE_DEBUG_LOG_PREFIX) "[WARN]" _TE_DEBUG_INFO " ",(f_), ##__VA_ARGS__)
#else
	#define TE_WARN(f_, ...)
#endif

#if defined(TE_DEBUG_LEVEL_ERROR)
	#define TE_ERROR(f_, ...)	_tinyengine_debug_fprintf(stderr, TE_D2STR(TE_DEBUG_LOG_PREFIX) "[ERROR]" _TE_DEBUG_INFO " ",(f_), ##__VA_ARGS__)
	#define TE_FATAL(f_, ...)	_tinyengine_debug_fprintf(stderr, TE_D2STR(TE_DEBUG_LOG_PREFIX) "[FATAL]" _TE_DEBUG_INFO " ",(f_), ##__VA_ARGS__)
#endif

#if defined(TE_LINUX) && defined(TE_DEBUG_OUTPUT_ENABLED)
	#define TE_TRACE() _tinyengine_debugPrintStackTrace(TE_D2STR(TE_DEBUG_LOG_PREFIX) "[TRACE]" _TE_DEBUG_INFO " ")
#else
	#define TE_TRACE()
#endif

#else

#define TE_LOG(f_, ...)
#define TE_WARN(f_, ...)
#define TE_ERROR(f_, ...)
#define TE_FATAL(f_, ...)
#define TE_TRACE()

#endif

/* END DEBUG HEADER */

/* ENGINE STRUCTURE DEF */

// TODO: decide on the windowing system in header, no by platform alone, allow override
#if defined(TE_LINUX)

	#include <X11/Xlib.h> // Display; Window;
	#include <GL/glx.h> // GLXContext;

	typedef struct _tinyengine_platformWindowContext_t {
		Window			x11WindowID;
		GLXContext	glxContext;
	} _tinyengine_platformWindowContext;

#elif defined(TE_WIN32)
	#include <windows.h>
	#include <GL/gl.h>

	typedef struct _tinyengine_platformWindowContext_t {
		te_bool_u8 windowReady;

		te_u32 minWidth;
		te_u32 minHeight;
		te_u32 maxWidth;
		te_u32 maxHeight;
		te_u32 aspectRatioWidth;
		te_u32 aspectRatioHeight;

		HWND win32window;
		HGLRC win32openGLcontext;
		HDC win32deviceContext;
	} _tinyengine_platformWindowContext;
#else
	typedef struct _tinyengine_platformWindowContext_t {} _tinyengine_platformWindowContext;
#endif

typedef struct tinyengine_windowContext_t{
	te_bool_u8 closeRequested;
	void(*characterCallback)(struct tinyengine_windowContext_t*,te_u32);
	void(*keyCallback)(struct tinyengine_windowContext_t*,te_i32,te_i32,te_i32);
	void(*frameCallback)(struct tinyengine_windowContext_t*,te_f64,te_u32,te_u32);
	void(*closeCallback)(struct tinyengine_windowContext_t*);
	_tinyengine_platformWindowContext platform;
} tinyengine_windowContext;

typedef void (*tinyengine_windowCharacterCallback)(tinyengine_windowContext*,te_u32);
typedef void (*tinyengine_windowKeyCallback)(tinyengine_windowContext*,te_i32,te_i32,te_i32);
typedef void (*tinyengine_windowFrameCallback)(tinyengine_windowContext*,te_f64,te_u32,te_u32);
typedef void (*tinyengine_windowCloseCallback)(tinyengine_windowContext*);

/* END ENGINE STRUCTURE DEF */

/* ENGINE FUNCTION DEF */

te_bool_u8	tinyengine_init();
void				tinyengine_terminate();

/* END ENGINE FUNCTION DEF */

/* ENGINE IMPLEMENTATION */

#ifndef TE_HEADER_ONLY

//// Internal types

// TODO: Add checking if rendering system is included
// TODO: Make more consistant with the window context style

#if defined(TE_LINUX)
	typedef struct tinyengine_x11_state_t {

		Display* display;
		XVisualInfo* visualFormat;

		Atom wm_size_hints;
		Atom wm_delete_window;
		XContext windowPointerContextID;

		te_bool_u8 trapErrors;
		te_bool_u8 errorCaught;
		te_u8 lastErrorCode;
		int (*nativeErrorHandler)(Display *, XErrorEvent *);

	}	tinyengine_x11_state;
#elif defined(TE_WIN32)
	// TODO: Should this string be moved into the state?
	const char* tinyengine_win32_className = "tinyengineWin32";
	typedef struct tinyengine_win32_state_t {
	} tinyengine_win32_state;
#endif

typedef struct tinyengine_windowContextList_node_t{
	tinyengine_windowContext* value;
	struct tinyengine_windowContextList_node_t* next;
} tinyengine_windowContextList_node;

typedef struct {
	tinyengine_windowContextList_node* head;
	tinyengine_windowContextList_node* tail;
	size_t length;
} tinyengine_windowContextList;

struct tinyengine_state_t {

// Core
	te_bool_u8 valid;
// Window
	#if defined(TE_LINUX)
		tinyengine_x11_state x11state;
	#endif
	tinyengine_windowContextList windowContextList;
// Debug
	// TODO: Implement other threading methods
	#if defined(TE_PTHREADS) && defined(TE_DEBUG_OUTPUT_ENABLED)
		pthread_mutex_t debugPrintLock;
	#endif

} tinyengine_state = {0};

//// Debugging

#if defined(TE_DEBUG_OUTPUT_ENABLED)

#include <string.h> // strlen(); memcpy();
#include <stdlib.h> // malloc(); free();
#include <stdio.h> // fprintf(); FILE; std..; vfprintf();
#include <stdarg.h> // va_list; va_start(); va_end();
#include <pthread.h> // pthread_mutex_lock(); pthread_mutex_unlock(); pthread_mutex_t

#if defined(TE_LINUX)
#include <execinfo.h> // backtrace(); backtrace_symbols(); backtrace_symbols_fd();
#endif

void _tinyengine_debug_fprintf(FILE* stream, const char* prefix, const char * format, ...) {

	// TODO: Implement other threading methods
	#if defined(TE_PTHREADS)
		pthread_mutex_lock(&tinyengine_state.debugPrintLock);
	#endif

	va_list args;

	size_t prefixLength = strlen(prefix);
	size_t formatLength = strlen(format);
 	size_t bufferLength = prefixLength + formatLength + 1;

	char* buffer = (char*) malloc(bufferLength);
	memcpy(buffer, prefix, prefixLength);
	memcpy(buffer + prefixLength, format, formatLength);
	buffer[bufferLength-1] = '\0';

	va_start(args, format);
	vfprintf(stream,buffer, args);
	va_end(args);

	free(buffer);

	#if defined(TE_PTHREADS)
		pthread_mutex_unlock(&tinyengine_state.debugPrintLock);
	#endif
}

#if defined(TE_LINUX)
void _tinyengine_debugPrintStackTrace(const char* prefix) {
	pthread_mutex_lock(&tinyengine_state.debugPrintLock);
	int nptrs;
	void *buffer[100];

	nptrs = backtrace(buffer, 100);
	fprintf(stdout,"%sbacktrace() returned %d addresses:\n", prefix, nptrs);
	char **strings = backtrace_symbols(buffer, nptrs);

	if (strings == NULL) {
		return;
	}

	for (int j = 1; j < nptrs; j++){
		printf("[TRACE][%i] %s\n",j - 1,strings[j]);
	}

	free(strings);
	pthread_mutex_unlock(&tinyengine_state.debugPrintLock);
}
#endif

void _tinyengine_debugInit() {
	// TODO: Implement other threading methods
	#if defined(TE_PTHREADS)
		pthread_mutex_init(&tinyengine_state.debugPrintLock, NULL);
	#endif
	TE_LOG("Debugging enabled.\n");
}

#endif

//// Crash Handler

// TODO: find simular method for win32 crash handler
#if defined(TE_DEBUG) && defined(TE_LINUX)

#include <signal.h> // struct sigaction; sigaction(); SIG...
#include <unistd.h> // write(); STDERR_FILENO;
#include <stdlib.h> // free();
#include <string.h> // strlen();
#include <execinfo.h> // backtrace(); backtrace_symbols(); backtrace_symbols_fd();

void _tinyengine_linux_signalPrint(const char* msg) {
	write(STDERR_FILENO,msg,strlen(msg));
}

void _tinyengine_linux_signalPrintStackTrace() {
	int nptrs;
	void *buffer[100];

	nptrs = backtrace(buffer, 100);
	//signalPrint("backtrace() returned %d addresses\n", nptrs);

	backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);

	/*
		char **strings = backtrace_symbols(buffer, nptrs);

		if (strings == NULL) {
			signalPrint("No backtrace symbols returned.\n");
			return;
		}

		for (int j = 0; j < nptrs; j++){
			signalPrint(strings[j]);
			signalPrint("\n");
		}

		free(strings);
	*/
}

void _tinyengine_linux_signalCrashHandler(int signal, siginfo_t *info, void *ucontext) {
	switch(signal){
		case SIGSEGV:{
			if(info->si_addr == NULL) {
				_tinyengine_linux_signalPrint("NULL derefrenced!\n");
			}else {
				_tinyengine_linux_signalPrint("Invalid memory refrence!\n");
			}
			break;
		}
		case SIGFPE:{
			switch(info->si_code) {
				case FPE_INTDIV: // Integer divide by zero.
					_tinyengine_linux_signalPrint("Integer divide by zero!\n");
					break;
				case FPE_INTOVF: // Integer overflow.
					_tinyengine_linux_signalPrint("Integer overflow!\n");
					break;
				case FPE_FLTDIV: // Floating-point divide by zero.
					_tinyengine_linux_signalPrint("Floating-point divide by zero!\n");
					break;
				case FPE_FLTOVF: // Floating-point overflow.
					_tinyengine_linux_signalPrint("Floating-point overflow!\n");
					break;
				case FPE_FLTUND: // Floating-point underflow.
					_tinyengine_linux_signalPrint("Floating-point underflow!\n");
					break;
				case FPE_FLTRES: // Floating-point inexact result.
					_tinyengine_linux_signalPrint("Floating-point inexact result!\n");
					break;
				case FPE_FLTINV: // Floating-point invalid operation.
					_tinyengine_linux_signalPrint("Floating-point invalid operation!\n");
					break;
				case FPE_FLTSUB: // Subscript out of range.
					_tinyengine_linux_signalPrint("Subscript out of range!\n");
					break;
				default:
					_tinyengine_linux_signalPrint("Unkown math error!\n");
					break;
			}
			break;
		}
		case SIGILL:{
			_tinyengine_linux_signalPrint("Illegal Instruction!\n");
			break;
		}
		default: {
			_tinyengine_linux_signalPrint("Unkown signal!\n");
			break;
		}
	}
	_tinyengine_linux_signalPrintStackTrace();
	_exit(-1); // abort();
}

void _tinyengine_linux_registerSignalHandlers() {
	struct sigaction handler = {0};
	handler.sa_flags = SA_SIGINFO;
	handler.sa_sigaction = &_tinyengine_linux_signalCrashHandler;

	sigaction(SIGSEGV,&handler,NULL);
	sigaction(SIGFPE,&handler,NULL);
	sigaction(SIGILL,&handler,NULL);
}


#endif

//// Window System

#include <stdlib.h> // malloc(); free();

void _tinyengine_pushWindowContext(tinyengine_windowContext* value) {
	tinyengine_windowContextList_node* node = (tinyengine_windowContextList_node*)malloc(sizeof(tinyengine_windowContextList_node));
	node->next = NULL;
	node->value = value;
	if(tinyengine_state.windowContextList.head == NULL){
		tinyengine_state.windowContextList.head = node;
		tinyengine_state.windowContextList.tail = node;
		tinyengine_state.windowContextList.length = 1;
	} else {
		tinyengine_state.windowContextList.tail->next = node;
		tinyengine_state.windowContextList.length++;
		tinyengine_state.windowContextList.tail = node;
	}
}

void _tinyengine_removeWindowContext(tinyengine_windowContext* window) {
	tinyengine_windowContextList_node* node = tinyengine_state.windowContextList.head;
	tinyengine_windowContextList_node* prev = NULL;
	while(node != NULL) {
		if(node->value == window){
			if(prev == NULL) {
				tinyengine_state.windowContextList.head = NULL;
				tinyengine_state.windowContextList.tail = NULL;
				tinyengine_state.windowContextList.length = 0;
				return;
			}

			if(node->next == NULL) { tinyengine_state.windowContextList.tail = prev; }

			prev->next = node->next;
			tinyengine_state.windowContextList.length--;
			free(node);

			return;
		}
		prev = node;
		node = node->next;
	}
}

// TODO: decide on the windowing system in header, not by platform alone, allow override
#if defined(TE_LINUX)

// TODO: Load xlib,gtx,and other x11 extensions by dynamic loader instead of static link
// TODO: Maybe be compatable with windows version of xserver?


// TODO: should make these trap calls thread safe if I can
void _tinyengine_x11_enableErrorTrap() {
	tinyengine_state.x11state.errorCaught = TE_FALSE;
	tinyengine_state.x11state.lastErrorCode = 0;
	tinyengine_state.x11state.trapErrors = TE_TRUE;
}

void _tinyengine_x11_disableErrorTrap() {
	XSync(tinyengine_state.x11state.display,TE_FALSE);
	tinyengine_state.x11state.trapErrors = TE_FALSE;
}

int _tinyengine_x11_errorHandler(Display* display, XErrorEvent* event) {

	#if defined(TE_DEBUG_OUTPUT_ENABLED)
		char messageBuffer[100];
		XGetErrorText(display, event->error_code, &messageBuffer[0], 100);
	#endif

	if(tinyengine_state.x11state.display != display) {
		TE_WARN("Caught XError on unregistered display: %s",&messageBuffer[0]);
		return 0;
	}

	if(tinyengine_state.x11state.trapErrors){
		tinyengine_state.x11state.lastErrorCode = event->error_code;
		tinyengine_state.x11state.errorCaught = TE_TRUE;
		TE_ERROR("Caught X11 Error: %s\n",&messageBuffer[0]);
	}else{
		// TODO: handle potientially fatal xlib errors and pass information into render loop
		TE_ERROR("X11 Error: %s\n",&messageBuffer[0]);
		exit(-1);
	}

	return 0; // TODO: Find out what this return means
}

tinyengine_windowContext* _tinyengine_x11_translateWindowIDToWindowContext(Window windowID) {
	XPointer windowContext = NULL;
	if(XFindContext(tinyengine_state.x11state.display, windowID, tinyengine_state.x11state.windowPointerContextID, &windowContext) == 0 && windowContext != NULL) {
		return (tinyengine_windowContext*) windowContext;
	}else{
		TE_ERROR("Could not translate window id %i to windowContext pointer!\n",windowID);
		return NULL;
	}
}

te_bool_u8 _tinyengine_x11_createWindow(tinyengine_windowContext* window) {

	Window root = DefaultRootWindow(tinyengine_state.x11state.display);

	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = XCreateColormap(tinyengine_state.x11state.display, root, tinyengine_state.x11state.visualFormat->visual, AllocNone);
	windowAttributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;

	_tinyengine_x11_enableErrorTrap();

	window->platform.x11WindowID = XCreateWindow(
		tinyengine_state.x11state.display, root,
		0,0,300,300,0, // posX, posY, width, height, border
		tinyengine_state.x11state.visualFormat->depth,InputOutput,tinyengine_state.x11state.visualFormat->visual,
		CWColormap | CWEventMask,&windowAttributes
	);

	_tinyengine_x11_disableErrorTrap();

	if(tinyengine_state.x11state.errorCaught) {
		TE_ERROR("Error while creating window.\n");
		return TE_FALSE;
	}

	XSaveContext(tinyengine_state.x11state.display,window->platform.x11WindowID,tinyengine_state.x11state.windowPointerContextID,(XPointer) window);

	XSetWMProtocols(tinyengine_state.x11state.display, window->platform.x11WindowID, &(tinyengine_state.x11state.wm_delete_window), 1);

	// TODO: Do I need a context for each window? Or can they all share?
	window->platform.glxContext = glXCreateContext(tinyengine_state.x11state.display, tinyengine_state.x11state.visualFormat, NULL, GL_TRUE);

	return TE_TRUE;
}

void _tinyengine_x11_destroyWindow(tinyengine_windowContext* window){

}

void _tinyengine_x11_setWindowMaxSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	XSizeHints* hints = XAllocSizeHints();
	long returnedHints;
	XGetWMNormalHints(tinyengine_state.x11state.display, window->platform.x11WindowID, hints, &returnedHints);
	hints->flags |= PMaxSize;
	hints->max_width = width;
	hints->max_height = height;
	XSetWMNormalHints(tinyengine_state.x11state.display, window->platform.x11WindowID, hints);
	XFree(hints);
}

void _tinyengine_x11_setWindowMinSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	XSizeHints* hints = XAllocSizeHints();
	long returnedHints;
	XGetWMNormalHints(tinyengine_state.x11state.display, window->platform.x11WindowID, hints, &returnedHints);
	hints->flags |= PMinSize;
	hints->min_width = width;
	hints->min_height = height;
	XSetWMNormalHints(tinyengine_state.x11state.display, window->platform.x11WindowID, hints);
	XFree(hints);
}

void _tinyengine_x11_setWindowAspectRatio(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	XSizeHints* hints = XAllocSizeHints();
	long returnedHints;
	XGetWMNormalHints(tinyengine_state.x11state.display, window->platform.x11WindowID, hints, &returnedHints);
	hints->flags |= PAspect;
	hints->min_aspect.x = hints->max_aspect.x = width;
	hints->min_aspect.y = hints->max_aspect.y = height;
	XSetWMNormalHints(tinyengine_state.x11state.display, window->platform.x11WindowID, hints);
	XFree(hints);
}

void _tinyengine_x11_setWindowSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	XResizeWindow(tinyengine_state.x11state.display, window->platform.x11WindowID, width, height);
}

void _tinyengine_x11_setWindowTitle(tinyengine_windowContext* window, const char* title) {
	XStoreName(tinyengine_state.x11state.display, window->platform.x11WindowID, title);
}

void _tinyengine_x11_showWindow(tinyengine_windowContext* window){
	XMapWindow(tinyengine_state.x11state.display, window->platform.x11WindowID);
}

void _tinyengine_x11_pollDisplayEvents() {

	while(XPending(tinyengine_state.x11state.display)){

		XEvent event;
		XNextEvent(tinyengine_state.x11state.display,&event);

		switch(event.type) {

			case ClientMessage:
			{
				tinyengine_windowContext* window = _tinyengine_x11_translateWindowIDToWindowContext(event.xclient.window);
				if(window == NULL) { TE_WARN("ClientMessage event for unregistered window!\n"); break; }
				if (event.xclient.data.l[0] == tinyengine_state.x11state.wm_delete_window) {
					window->closeRequested = TE_TRUE;
					window->closeCallback(window);
				}
			} break;

			case KeyPress: case KeyRelease:
			{

				tinyengine_windowContext* window = _tinyengine_x11_translateWindowIDToWindowContext(event.xkey.window);
				if(window == NULL) { TE_WARN("KeyMessage event for unregistered window!\n"); break; }

				char buf[2];
				KeySym ks;
				XComposeStatus comp;

				te_i32 len = XLookupString(&event.xkey, buf, 1, &ks, &comp);
				if (event.type == KeyRelease && len > 0) { window->characterCallback(window,buf[0]); }

				// TODO: translate x11 key codes to universal keycodes

			}	break;

			default: break;

		}

	}
}

te_bool_u8 _tinyengine_x11_init() {

	// TODO: make everything a local variable then just assign them all to the x11state at the end? (is this worst?)

	tinyengine_state.x11state.nativeErrorHandler = XSetErrorHandler(&_tinyengine_x11_errorHandler);
	tinyengine_state.x11state.windowPointerContextID = XUniqueContext();
	tinyengine_state.x11state.display = XOpenDisplay(NULL);

	if(tinyengine_state.x11state.display == NULL) {
		TE_ERROR("Could not connect to X11 server!\n");
		return TE_FALSE;
	}

	TE_LOG("X11 window manager loaded.\n");
	TE_LOG("X11 display connected on %s\n",XDisplayString(tinyengine_state.x11state.display));
	TE_LOG("X11 Display Version: %i.%i\n",XProtocolVersion(tinyengine_state.x11state.display),XProtocolRevision(tinyengine_state.x11state.display));
	TE_LOG("X11 Vendor: %s\n",XServerVendor(tinyengine_state.x11state.display));

	te_i32 glxAttributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	tinyengine_state.x11state.visualFormat = glXChooseVisual(tinyengine_state.x11state.display, 0, glxAttributes);

	if(tinyengine_state.x11state.visualFormat == NULL) {
		TE_ERROR("GLX could not find any compatable OpenGL visual format for display!\n");
		return TE_FALSE;
	}

	tinyengine_state.x11state.wm_delete_window = XInternAtom(tinyengine_state.x11state.display, "WM_DELETE_WINDOW", False);

	tinyengine_state.x11state.wm_size_hints = XInternAtom(tinyengine_state.x11state.display, "WM_SIZE_HINTS", False);

	return TE_TRUE;
}

void _tinyengine_x11_terminate() {

}

void _tinyengine_glx_makeCurrent(tinyengine_windowContext* window){
	glXMakeCurrent(tinyengine_state.x11state.display, window->platform.x11WindowID, window->platform.glxContext);
}

void _tinyengine_glx_swapBuffers(tinyengine_windowContext* window) {
	glXSwapBuffers(tinyengine_state.x11state.display, window->platform.x11WindowID);
}

#elif defined(TE_WIN32)
// TODO: tinyengine_win32 descripes the platform, need a new name for the windowing system specificly

#define _TE_WINSTYLE WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME

LRESULT CALLBACK _tinyengine_win32_eventCallback(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam) {

	tinyengine_windowContext* window = GetPropA(hWindow,"tinyengine");
	if(!window && Message == WM_CREATE) { window = (tinyengine_windowContext*) ((LPCREATESTRUCTA)lParam)->lpCreateParams; }
	if(!window) { return DefWindowProc(hWindow,Message,wParam,lParam); }

	switch(Message) {
		case WM_CREATE: window->platform.windowReady = TE_TRUE; return 0;
		case WM_CLOSE: window->closeRequested = TE_TRUE; window->closeCallback(window); return 0;
		case WM_DESTROY: window->closeRequested = TE_TRUE; window->closeCallback(window); return 0;

		case WM_MOUSEACTIVATE: break;
		case WM_CAPTURECHANGED: break;

		case WM_SETFOCUS: break;
		case WM_KILLFOCUS: break;

		case WM_SYSCOMMAND: break;

		case WM_INPUTLANGCHANGE: break;

		// TODO: Use this for char input instead of keyrelease?
		case WM_CHAR: break;
		case WM_SYSCHAR: break;
		case WM_UNICHAR: break;

		case WM_KEYDOWN: break;
		case WM_SYSKEYDOWN: break;
		case WM_KEYUP: break;
		case WM_SYSKEYUP: break;

		case WM_LBUTTONDOWN: break;
		case WM_RBUTTONDOWN: break;
    case WM_MBUTTONDOWN: break;
    case WM_XBUTTONDOWN: break;
    case WM_LBUTTONUP: break;
    case WM_RBUTTONUP: break;
    case WM_MBUTTONUP: break;
    case WM_XBUTTONUP: break;

		case WM_MOUSEMOVE: break;

		case WM_INPUT: break;

		case WM_ERASEBKGND: return TRUE;


		case WM_SIZING: // TODO: Seems to be busted on wine?
			{
				if(window->platform.aspectRatioWidth == 0 || window->platform.aspectRatioHeight == 0) {break;}
				int edge = wParam;
				RECT* size = (RECT*) lParam;

				RECT reportedSize;
				RECT adjustedSize;

				// TODO: Should probably just memcpy this
				GetClientRect(window->platform.win32window, &reportedSize);
				GetClientRect(window->platform.win32window, &adjustedSize);
				AdjustWindowRect(&adjustedSize,_TE_WINSTYLE,0);

				te_i32 widthOffset = (reportedSize.right - reportedSize.left) - (adjustedSize.right - adjustedSize.left);
				te_i32 heightOffset = (reportedSize.bottom - reportedSize.top) - (reportedSize.bottom - reportedSize.top);

				const float ratio = (float) window->platform.aspectRatioWidth / (float) window->platform.aspectRatioHeight;

				if (edge == WMSZ_LEFT  || edge == WMSZ_BOTTOMLEFT || edge == WMSZ_RIGHT || edge == WMSZ_BOTTOMRIGHT) {
		        size->bottom = size->top + heightOffset + (int) ((size->right - size->left - widthOffset) / ratio);
		    } else if (edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT) {
		        size->top = size->bottom - heightOffset - (int) ((size->right - size->left - widthOffset) / ratio);
		    } else if (edge == WMSZ_TOP || edge == WMSZ_BOTTOM) {
		        size->right = size->left + widthOffset + (int) ((size->bottom - size->top - heightOffset) * ratio);
		    }

				return TRUE;
			}
			break;
		case WM_GETMINMAXINFO: // TODO: Seems to be busted in wine?
			{
				MINMAXINFO* info = (MINMAXINFO*) lParam;

				RECT reportedSize;
				RECT adjustedSize;

				// TODO: Should probably just memcpy this
				GetClientRect(window->platform.win32window, &reportedSize);
				GetClientRect(window->platform.win32window, &adjustedSize);
				AdjustWindowRect(&adjustedSize,_TE_WINSTYLE,0);

				te_i32 widthOffset = (reportedSize.right - reportedSize.left) - (adjustedSize.right - adjustedSize.left);
				te_i32 heightOffset = (reportedSize.bottom - reportedSize.top) - (reportedSize.bottom - reportedSize.top);

				TE_LOG("MaxSize: %lix%li MinSize: %lix%li\n",info->ptMaxTrackSize.x,info->ptMaxTrackSize.y,info->ptMinTrackSize.x,info->ptMinTrackSize.y);

				if(window->platform.minWidth > 0 && window->platform.minHeight > 0 ){
					info->ptMinTrackSize.x = window->platform.minWidth + widthOffset;
          info->ptMinTrackSize.y = window->platform.minHeight + heightOffset;
				}

				if(window->platform.maxWidth > 0 && window->platform.maxHeight > 0 ){
					info->ptMaxTrackSize.x = window->platform.maxWidth + widthOffset;
          info->ptMaxTrackSize.y = window->platform.maxHeight + heightOffset;
				}

				TE_LOG("MaxSize: %lix%li MinSize: %lix%li\n",info->ptMaxTrackSize.x,info->ptMaxTrackSize.y,info->ptMinTrackSize.x,info->ptMinTrackSize.y);

				break;
				return 0;
			}
			TE_LOG("TE1\n");
			break;
	}

	return DefWindowProc(hWindow,Message,wParam,lParam);
}

void _tinyengine_win32_pollEvents() {
	MSG message;

	while(PeekMessageA(&message,0,0,0,PM_REMOVE)){
		//GetMessage(&message,0,0,0);
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void _tinyengine_win32_setWindowSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {


	RECT adjustedSize = {0,0,width,height};
	AdjustWindowRect(&adjustedSize,_TE_WINSTYLE,0);
	width = adjustedSize.right - adjustedSize.left;
	height = adjustedSize.bottom - adjustedSize.top;

	SetWindowPos(
		window->platform.win32window,
		HWND_TOP,0,0,
		width, height,
		SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER
	);
}

void _tinyengine_win32_forceSizeUpdate(tinyengine_windowContext* window) {
	RECT size;
	GetWindowRect(window->platform.win32window, &size);
	MoveWindow(window->platform.win32window,size.left, size.top, size.right - size.left, size.bottom - size.top, TRUE);
}

void _tinyengine_win32_setWindowMaxSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	window->platform.maxWidth = width;
	window->platform.maxHeight = height;
	_tinyengine_win32_forceSizeUpdate(window);
}

void _tinyengine_win32_setWindowMinSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	window->platform.minWidth = width;
	window->platform.minHeight = height;
	_tinyengine_win32_forceSizeUpdate(window);
}

void _tinyengine_win32_setWindowAspectRatio(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	window->platform.aspectRatioWidth = width;
	window->platform.aspectRatioHeight = height;
	_tinyengine_win32_forceSizeUpdate(window);
}

void _tinyengine_win32_getWindowSize(tinyengine_windowContext* window, te_u32* width, te_u32* height) {
	RECT adjustedSize;
	GetClientRect(window->platform.win32window, &adjustedSize);
	AdjustWindowRect(&adjustedSize,_TE_WINSTYLE,0);
	*width = adjustedSize.right - adjustedSize.left;
	*height = adjustedSize.bottom - adjustedSize.top;
}

void _tinyengine_win32_setWindowTitle(tinyengine_windowContext* window, const char* title) {
	SetWindowTextA(window->platform.win32window, title);
}

void _tinyengine_win32_showWindow(tinyengine_windowContext* window) {
	ShowWindow(window->platform.win32window, SW_SHOWNA);
}

te_bool_u8 _tinyengine_win32_createWindow(tinyengine_windowContext* window){

	// TODO: Load modern GL pipelines

	HINSTANCE Instance = GetModuleHandle(NULL);

	RECT adjustedSize = {0,0,300,300};
	AdjustWindowRect(&adjustedSize,_TE_WINSTYLE,0);

	int winWidth = adjustedSize.right - adjustedSize.left;
	int winHeight = adjustedSize.bottom - adjustedSize.top;

	window->platform.win32window = CreateWindowExA(0,tinyengine_win32_className,"tinyengine",_TE_WINSTYLE,CW_USEDEFAULT,CW_USEDEFAULT,winWidth,winHeight,NULL,NULL,Instance,window);

	if(!window->platform.win32window) {
		#if defined(TE_DEBUG_OUTPUT_ENABLED)
			DWORD errorCode = GetLastError();
			LPSTR errorMessage = NULL;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	                                 NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errorMessage, 0, NULL);
			TE_ERROR("Error while creating window: %s",errorMessage);
		#endif
		return TE_FALSE;
	}

	SetPropA(window->platform.win32window, "tinyengine", window);

	// TODO: Is this even neccesary?
	UINT_PTR timeout = SetTimer(NULL,0,1000,NULL);
	while(!window->platform.windowReady){
		MSG message;
		GetMessageA(&message,0,0,0);
		if(message.message == WM_TIMER && message.wParam == timeout) {
			TE_ERROR("Timeout while waiting for windowCreate event!\n");
			return TE_FALSE;
		}
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	KillTimer(NULL,timeout);

	window->platform.win32deviceContext = GetDC(window->platform.win32window);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};



	int pixelFormat = ChoosePixelFormat(window->platform.win32deviceContext,&pfd);
	SetPixelFormat(window->platform.win32deviceContext,pixelFormat,&pfd);

	window->platform.win32openGLcontext = wglCreateContext(window->platform.win32deviceContext);

	return TE_TRUE;
}

void _tinyengine_win32_destroyWindow() {}

te_bool_u8 _tinyengine_win32_init() {
	HINSTANCE Instance = GetModuleHandle(NULL);
	WNDCLASSEXA winClassParams = {0};
	winClassParams.cbSize = sizeof(WNDCLASSEX);
	winClassParams.style = CS_OWNDC;
	winClassParams.lpfnWndProc = _tinyengine_win32_eventCallback;
	winClassParams.hInstance = Instance;
	//	winClass.hIcon = ; TODO: Implement Window Icon
	winClassParams.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClassParams.lpszClassName = tinyengine_win32_className;
	RegisterClassExA(&winClassParams);
	TE_LOG("win32 windows manager loaded.\n");
}

void _tinyengine_win32_terminate() {

}

void _tinyengine_wgl_makeCurrent(tinyengine_windowContext* window) {
	wglMakeCurrent (window->platform.win32deviceContext, window->platform.win32openGLcontext);
}

void _tinyengine_wgl_swapBuffers(tinyengine_windowContext* window) {
	SwapBuffers(window->platform.win32deviceContext);
}

#else // empty window system



#endif

#include <string.h>

void _tinyengine_windowCallbackStub() { };

tinyengine_windowContext* tinyengine_createWindow() {

	tinyengine_windowContext* window = malloc(sizeof(tinyengine_windowContext));
	memset(window, 0, sizeof(tinyengine_windowContext));

	te_bool_u8 valid = TE_FALSE;

	#if defined(TE_LINUX)
		valid = _tinyengine_x11_createWindow(window);
	#elif defined(TE_WIN32)
		valid = _tinyengine_win32_createWindow(window);
	#endif

	if(!valid) { free(window); return NULL; }

	window->keyCallback = &_tinyengine_windowCallbackStub;
	window->closeCallback = &_tinyengine_windowCallbackStub;
	window->characterCallback = &_tinyengine_windowCallbackStub;
	window->frameCallback = &_tinyengine_windowCallbackStub;

	_tinyengine_pushWindowContext(window);

	return window;
}

void tinyengine_destroyWindow(tinyengine_windowContext* window) {
	if(window == NULL) { return; }
	#if defined(TE_LINUX)
		_tinyengine_x11_destroyWindow(window);
	#elif defined(TE_WIN32)
		_tinyengine_win32_destroyWindow(window);
	#endif
	_tinyengine_removeWindowContext(window);
	free(window);
}

void tinyengine_destroyAllWindows() {

	tinyengine_windowContextList_node* node = tinyengine_state.windowContextList.head;

	while(node != NULL) {
		tinyengine_windowContext* window = node->value;
		if(window != NULL) {
			#if defined(TE_LINUX)
				_tinyengine_x11_destroyWindow(window);
			#elif defined(TE_WIN32)
				_tinyengine_win32_destroyWindow(window);
			#endif
			free(window);
		}
		free(node);
		node = node->next;
	}

	tinyengine_state.windowContextList.head = NULL;
	tinyengine_state.windowContextList.tail = NULL;
	tinyengine_state.windowContextList.length = 0;
}

void tinyengine_setWindowMaxSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	#if defined(TE_LINUX)
		_tinyengine_x11_setWindowMaxSize(window,width,height);
	#elif defined(TE_WIN32)
		_tinyengine_win32_setWindowMaxSize(window,width,height);
	#endif
}

void tinyengine_setWindowMinSize(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	#if defined(TE_LINUX)
		_tinyengine_x11_setWindowMinSize(window,width,height);
	#elif defined(TE_WIN32)
		_tinyengine_win32_setWindowMinSize(window,width,height);
	#endif
}

void tinyengine_setWindowAspectRatio(tinyengine_windowContext* window, te_u32 width, te_u32 height) {
	#if defined(TE_LINUX)
		_tinyengine_x11_setWindowAspectRatio(window,width,height);
	#elif defined(TE_WIN32)
		_tinyengine_win32_setWindowAspectRatio(window,width,height);
	#endif
}

void tinyengine_showWindow(tinyengine_windowContext* window) {
	#if defined(TE_LINUX)
		_tinyengine_x11_showWindow(window);
	#elif defined(TE_WIN32)
		_tinyengine_win32_showWindow(window);
	#endif
}

void tinyengine_setWindowTitle(tinyengine_windowContext* window, const char* title){
	#if defined(TE_LINUX)
		_tinyengine_x11_setWindowTitle(window,title);
	#elif defined(TE_WIN32)
		_tinyengine_win32_setWindowTitle(window,title);
	#endif
}

void tinyengine_setWindowSize(tinyengine_windowContext* window, te_u32 width, te_u32 height){
	#if defined(TE_LINUX)
		_tinyengine_x11_setWindowSize(window,width,height);
	#elif defined(TE_WIN32)
		_tinyengine_win32_setWindowSize(window,width,height);
	#endif
}

// TODO: this name isnt very descriptive to the window system, but also cant be generalized for the input?
void tinyengine_pollEvents() {
	#if defined(TE_LINUX)
		_tinyengine_x11_pollDisplayEvents();
	#elif defined(TE_WIN32)
		_tinyengine_win32_pollEvents();
	#endif
}

void tinyengine_swapBuffers(tinyengine_windowContext* window) {
	#if defined(TE_LINUX)
		_tinyengine_glx_swapBuffers(window);
	#elif defined(TE_WIN32)
		_tinyengine_wgl_swapBuffers(window);
	#endif
}

void tinyengine_makeCurrent(tinyengine_windowContext* window) {
	#if defined(TE_LINUX)
		_tinyengine_glx_makeCurrent(window);
	#elif defined(TE_WIN32)
		_tinyengine_wgl_makeCurrent(window);
	#endif
}

//// Renderer

// TODO: Support multiple renderers and allow selection of them with flags outside of platform
#if defined(TE_LINUX) || defined(TE_WIN32)
// opengl renderer
// TODO: Create fallback opengl 1.0 renderer

static const char* TE_GL3_SPRITE_VERTEX_SRC =
		"#version 130                                                            \n"
		"in vec4 vertex;                                                         \n"
		"out vec2 tex_cords;                                                     \n"
		"                                                                        \n"
		"uniform mat4 projection;                                                \n"
		"                                                                        \n"
		"void main()                                                             \n"
		"{                                                                       \n"
		"    tex_cords = vertex.zw;                                              \n"
		"    gl_Position = vec4(vertex.x, vertex.y, 1.0, 1.0) * projection;      \n"
		"}                                                                       \n"
;

static const char* TE_GL3_SPRITE_FRAGMENT_SRC =
		"#version 130                                                            \n"
		"in vec2 tex_cords;                                                      \n"
		"out vec4 color;                                                         \n"
		"                                                                        \n"
		"uniform sampler2D texture_bank;                                         \n"
//		"uniform vec3 sprite_color;                                              \n"
		"                                                                        \n"
		"void main()                                                             \n"
		"{                                                                       \n"
    "    color = texture(texture_bank, tex_cords);                           \n"
		"}                                                                       \n"
;

static const char* TE_GL3_TEXT_VERTEX_SRC =
		"#version 130                                                            \n"
		"in vec4 vertex;                                                         \n"
		"out vec2 tex_cords;                                                     \n"
		"                                                                        \n"
		"uniform mat4 projection;                                                \n"
		"                                                                        \n"
		"void main()                                                             \n"
		"{                                                                       \n"
		"    tex_cords = vertex.xy;                                              \n"
		"    gl_Position = vec4(vertex.z, vertex.w, 1.0, 1.0) * projection;      \n"
		"}                                                                       \n"
;

static const char* TE_GL3_TEXT_FRAGMENT_SRC =
		"#version 130                                                            \n"
		"in vec2 tex_cords;                                                      \n"
		"out vec4 color;                                                         \n"
		"                                                                        \n"
		"uniform sampler2D texture_bank;                                         \n"
		"uniform vec3 text_color;                                                \n"
		"                                                                        \n"
		"void main()                                                             \n"
		"{                                                                       \n"
		"    float sample = texture(texture_bank, tex_cords).a;                  \n"
    "    color = vec4(text_color,sample);                                    \n"
		"}                                                                       \n"
;

static const char* TE_GL3_FLAT_VERTEX_SRC =
		"#version 130                                                            \n"
		"in vec2 vertex;                                                         \n"
		"                                                                        \n"
		"uniform mat4 projection;                                                \n"
		"                                                                        \n"
		"void main()                                                             \n"
		"{                                                                       \n"
		"    gl_Position = vec4(vertex.x, vertex.y, 1.0, 1.0) * projection;      \n"
		"}                                                                       \n"
;

static const char* TE_GL3_FLAT_FRAGMENT_SRC =
		"#version 130                                                            \n"
		"out vec4 color;                                                         \n"
		"                                                                        \n"
		"uniform vec4 sprite_color;                                              \n"
		"                                                                        \n"
		"void main()                                                             \n"
		"{                                                                       \n"
    "    color = sprite_color;                                               \n"
		"}                                                                       \n"
;

// TODO: Move these to the header

#if defined(TE_WIN32)
	#define _TE_GL_FUNCTION __stdcall
#else
	#define _TE_GL_FUNCTION
#endif

typedef te_u32	te_GLenum;

typedef te_i8		te_GLbyte;
typedef te_u8		te_GLubyte;
typedef te_i16 	te_GLshort;
typedef te_u16 	te_GLushort;
typedef te_i32 	te_GLint;
typedef te_u32 	te_GLuint;

typedef te_i32 	te_GLsizei;
typedef te_f32	te_GLfloat;

typedef char		te_GLchar;

#define TE_GL_FALSE 0
#define TE_GL_TRUE 1

#define TE_GL_VENDOR 0x1F00
#define TE_GL_RENDERER 0x1F01
#define TE_GL_VERSION 0x1F02
// Use this only in the GL 3.0 version not 1.0
#define TE_GL_SHADING_LANGUAGE_VERSION 0x8B8C

#define TE_GL_FRAGMENT_SHADER 0x8B30
#define TE_GL_VERTEX_SHADER 0x8B31
#define TE_GL_COMPILE_STATUS 0x8B81
#define TE_GL_LINK_STATUS 0x8B82

void _tinyengine_gl3_stub() {
	TE_FATAL("!!! Using unloaded GL3 function!\n");
	TE_TRACE();
	exit(-1);
}

// TODO: Should this be moved into the state? or remain out for use by gamedevs and null otherwise?

typedef struct tinyengine_gl3_functionTable {
	_TE_GL_FUNCTION te_GLenum (*glGetError)();
	_TE_GL_FUNCTION te_GLuint (*glCreateShader)(te_GLenum);
	_TE_GL_FUNCTION void (*glCompileShader)(te_GLuint);
	_TE_GL_FUNCTION void (*glGetShaderiv)(te_GLuint, te_GLenum, te_GLint*);
	_TE_GL_FUNCTION void (*glGetShaderInfoLog)(te_GLuint, te_GLsizei, te_GLsizei*, te_GLchar*);
	_TE_GL_FUNCTION void (*glBindAttribLocation)(te_GLuint, te_GLuint, const te_GLchar*);
	_TE_GL_FUNCTION te_GLuint (*glCreateProgram)();
	_TE_GL_FUNCTION void (*glAttachShader)(te_GLuint, te_GLuint);
	_TE_GL_FUNCTION void (*glLinkProgram)(te_GLuint);
	_TE_GL_FUNCTION void (*glGetProgramiv)(te_GLuint, te_GLenum, te_GLint*);
	_TE_GL_FUNCTION void (*glGetProgramInfoLog)(te_GLuint, te_GLsizei, te_GLsizei*, te_GLchar*);
	_TE_GL_FUNCTION void (*glDeleteShader)(te_GLuint);
	_TE_GL_FUNCTION void (*glDeleteProgram)(te_GLuint);
	_TE_GL_FUNCTION void (*glShaderSource)(te_GLuint, te_GLsizei, const te_GLchar *const*, const te_GLint*);
}	te_gl3_functions;

// TODO: Compiler check and switch on this
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
te_gl3_functions te_gl3 = {
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub,
	&_tinyengine_gl3_stub
};
#pragma GCC diagnostic pop

void* _tinyengine_gl_loadProc(const char* function) {
	#if defined(TE_WIN32)
		void* address = wglGetProcAddress(function);
		if(address == NULL || address <= (void*)3){
			HMODULE module = LoadLibraryA("opengl32.dll");
			address = (void *)GetProcAddress(module, function);
		}
		return address;
	#elif defined(TE_LINUX)
		return glXGetProcAddress(function);
	#endif
	return NULL;
}

#define _TE_GL_FUNCTION_LOAD(_f) te_gl3._f = _tinyengine_gl_loadProc(TE_D2STR(_f));

te_bool_u8 _tinyengine_gl3_init() {
	_TE_GL_FUNCTION const GLubyte* (*te_glGetString)(te_GLenum name) = _tinyengine_gl_loadProc("glGetString");
	if(!te_glGetString) { TE_ERROR("Could not get address of glGetString!\n"); return TE_FALSE; }
	if(te_glGetString(TE_GL_VERSION) == NULL) { TE_ERROR("glContext not valid!\n"); return TE_FALSE; }
	if(atof(te_glGetString(TE_GL_VERSION)) < 3.0 ) { TE_ERROR("openGL version < 3.0!\n"); return TE_FALSE; }

	TE_LOG("loaded openGL 3.0 render backend.\n");

	TE_LOG("OpenGL Version: %s\n",te_glGetString(TE_GL_VERSION));
	TE_LOG("OpenGL Shader Language Version: %s\n",te_glGetString(TE_GL_SHADING_LANGUAGE_VERSION));
	TE_LOG("OpenGL Vendor: %s\n",te_glGetString(TE_GL_VENDOR));
	TE_LOG("OpenGL Renderer: %s\n",te_glGetString(TE_GL_RENDERER));

	// TODO: Load opengl functions
	_TE_GL_FUNCTION_LOAD(glGetError);
	_TE_GL_FUNCTION_LOAD(glCreateShader);
	_TE_GL_FUNCTION_LOAD(glCompileShader);
	_TE_GL_FUNCTION_LOAD(glGetShaderiv);
	_TE_GL_FUNCTION_LOAD(glGetShaderInfoLog);
	_TE_GL_FUNCTION_LOAD(glBindAttribLocation);
	_TE_GL_FUNCTION_LOAD(glCreateProgram);
	_TE_GL_FUNCTION_LOAD(glAttachShader);
	_TE_GL_FUNCTION_LOAD(glLinkProgram);
	_TE_GL_FUNCTION_LOAD(glGetProgramiv);
	_TE_GL_FUNCTION_LOAD(glGetProgramInfoLog);
	_TE_GL_FUNCTION_LOAD(glDeleteShader);
	_TE_GL_FUNCTION_LOAD(glDeleteProgram);
	_TE_GL_FUNCTION_LOAD(glShaderSource);

	return TE_TRUE;
}


te_bool_u8 tinyengine_gl3_compileShader(te_GLuint* program, const char* vertexSrc, const char* fragmentSrc) {

	te_GLuint vertex, fragment;
	te_GLint success ;
	char infoLog[512];

	// vertex Shader
	vertex = te_gl3.glCreateShader(TE_GL_VERTEX_SHADER);
	te_gl3.glShaderSource(vertex, 1, &vertexSrc, NULL);
	te_gl3.glCompileShader(vertex);
	// print compile errors if any
	te_gl3.glGetShaderiv(vertex, TE_GL_COMPILE_STATUS, &success);
	if(success == TE_GL_FALSE) {
		te_gl3.glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		TE_ERROR("Vertex shader compilation failed: %s\n",infoLog);
		return TE_FALSE;
	};

	// fragment Shader
	fragment = te_gl3.glCreateShader(TE_GL_FRAGMENT_SHADER);
	te_gl3.glShaderSource(fragment, 1, &fragmentSrc, NULL);
	te_gl3.glCompileShader(fragment);
	// print compile errors if any
	te_gl3.glGetShaderiv(fragment, TE_GL_COMPILE_STATUS, &success);
	if(success == TE_GL_FALSE) {
		te_gl3.glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		TE_ERROR("Fragment shader compilation failed: %s\n",infoLog);
		return TE_FALSE;
	};

	// shader Program
	*program = te_gl3.glCreateProgram();
	te_gl3.glBindAttribLocation(*program,0,"vertex");
	te_gl3.glAttachShader(*program, vertex);
	te_gl3.glAttachShader(*program, fragment);
	te_gl3.glLinkProgram(*program);
	// print linking errors if any
	te_gl3.glGetProgramiv(*program, TE_GL_LINK_STATUS, &success);
	if(success == TE_GL_FALSE) {
		te_gl3.glGetProgramInfoLog(*program, 512, NULL, infoLog);
		TE_ERROR("shader program link failed: %s\n",infoLog);

		// TODO: Do I need to delete this if shader failure is a critical error?
		te_gl3.glDeleteShader(vertex);
		te_gl3.glDeleteShader(fragment);
		te_gl3.glDeleteProgram(*program);

		*program = 0;

		return TE_FALSE;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	te_gl3.glDeleteShader(vertex);
	te_gl3.glDeleteShader(fragment);

	return TE_TRUE;
}

te_bool_u8 _tinyengine_gl3_createWindowRenderContext(tinyengine_windowContext* window) {
	// TODO: check if opengl3 has been initilized and init it if not first

}



#else
// empty renderer

#endif

//// Engine Core

te_bool_u8 tinyengine_init() {

	#if defined(TE_DEBUG_OUTPUT_ENABLED)
		_tinyengine_debugInit();
	#endif

	TE_LOG("tinyengine version %i.%i.%i\n",TE_VERSION_MAJOR,TE_VERSION_MINOR,TE_VERSION_BUILD);
	TE_LOG("Fixed engine state size: %lu bytes\n",sizeof(tinyengine_state));

	#if defined(TE_LINUX)
		TE_LOG("tinyengine linux backend loaded.\n");
		#if defined(TE_DEBUG_OUTPUT_ENABLED)
			_tinyengine_linux_registerSignalHandlers();
		#endif
		if(!_tinyengine_x11_init()) { TE_ERROR("Could not load x11 window manager backend!\n"); return TE_FALSE; }
		//if(!_tinyengine_gl3_init()) { TE_ERROR("Could not load openGL 3.0 render backend!\n"); return TE_FALSE;}
	#elif defined(TE_WIN32)
		TE_LOG("tinyengine win32 backend loaded.\n");

		#if defined(TE_DEBUG_OUTPUT_ENABLED)
			static const char *(CDECL *pwine_get_version)(void);
			HMODULE hntdll = GetModuleHandle("ntdll.dll");
			if(!hntdll) {
				TE_WARN("No ntdll.dll detected?\n");
			}else {
				pwine_get_version = (void *)GetProcAddress(hntdll, "wine_get_version");
				if(pwine_get_version) { TE_LOG("running on Wine %s\n",pwine_get_version());}
			}
		#endif

		if(!_tinyengine_win32_init()) { TE_ERROR("Couldn not load win32 window manager backend!\n"); return TE_FALSE; }
		//if(!_tinyengine_gl3_init()) { TE_ERROR("Could not load openGL 3.0 render backend!\n"); return TE_FALSE;}
	#else
		TE_WARN("tinyengine compiled for unknown platform!\n");
	#endif


	return TE_TRUE;
}

void tinyengine_terminate() {
	tinyengine_destroyAllWindows();
}

#endif /* TE_HEADER_ONLY */

/* END ENGINE IMPLEMENTATION */

#ifdef __cplusplus
}
#endif


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

#ifndef TE_INCLUDE_TINYENGINE_H
#define TE_INCLUDE_TINYENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#define TE_VERSION_MAJOR 1
#define TE_VERSION_MINOR 1
#define TE_VERSION_BUILD 0
	
/* COMPILE TIME PLATFORM DETECTION */

#if defined(TE_WIN32) || defined(TE_LINUX) || defined(TE_APPLE) || defined(TE_ANDROID) ||  defined(TE_EMSCRIPTEN)
	#warning "tinyengine: User override of platform detection."
#elif defined(_WIN32)
    #define TE_WIN32
#else
    #if defined(__linux__)
        #define TE_LINUX
    #elif defined(__APPLE__)
        #define TE_APPLE
    #elif defined(__ANDROID__)
        #define TE_ANDROID
    #elif defined(__EMSCRIPTEN__)
        #define TE_EMSCRIPTEN
    #else
        #error "tinyengine: Could not detect build platform. Manually define a supported platform to continue build."
    #endif
#endif

#if defined(TE_WIN32)
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
        #define TE_WIN32_UWP
    #else
        #define TE_WIN32_DESKTOP
    #endif
#endif

#if defined(TE_LINUX) || defined(TE_APPLE) || defined(TE_ANDROID) || defined(TE_EMSCRIPTEN)
    #define TE_POSIX
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

struct te_vector4_f32 { 
	union { te_f32 x; te_f32 r; te_f32 s; };
	union { te_f32 y; te_f32 g; te_f32 t; };
	union { te_f32 z; te_f32 b; te_f32 p; };
	union { te_f32 w; te_f32 a; te_f32 q; };
};
typedef struct te_vector4_f32		te_v4_f32;

struct te_vector3_f32 {
	union { te_f32 x; te_f32 r; te_f32 s; };
	union { te_f32 y; te_f32 g; te_f32 t; };
	union { te_f32 z; te_f32 b; te_f32 p; };
};
typedef struct te_vector3_f32		te_v3_f32;

struct te_vector2_f32 {
	union { te_f32 x; te_f32 r; te_f32 s; };
	union { te_f32 y; te_f32 g; te_f32 t; };
};
typedef struct te_vector2_f32		te_v2_f32;

#if !defined(NULL)
#define NULL 0
#endif

#define TE_D2STR(x) _TE_D2STR(x)
#define _TE_D2STR(x) #x

/* END STANDARD TYPE DEFINITIONS */
 
#line 1 "debug.h"

/* DEBUG HEADER */

#ifdef TE_DEBUG

	#include <stdio.h> // fprintf(); FILE; std..; vfprintf();
	#include <stdarg.h> // va_list; va_start(); va_end();

	struct te_debug_state_T {
		te_bool_u8 valid;
	} te_debug_state;

	enum te_debug_logLevel {
		FATAL,
		ERROR,
		WARNING,
		INFO
	};

	void te_debug_printf(const char* prefix, enum te_debug_logLevel logLevel, const char* fileName, te_u32 lineNumber, const char* format, ...);
	void te_debug_printStackTrace();

	te_bool_u8 te_p_debugInit();
	void te_p_debugTerminate();

	#if !defined(TE_DEBUG_PREFIX)
		#define TE_DEBUG_PREFIX [TINYENGINE] 
	#endif

	#if !defined(TE_DEBUG_LOG_FUNCTION)
		#define TE_DEBUG_LOG_FUNCTION te_debug_printf
	#endif

	#define TE_DINFO(f_, ...) TE_DEBUG_LOG_FUNCTION(TE_D2STR(TE_DEBUG_PREFIX),INFO,TE_D2STR(__FILE__),__LINE__,(f_), ##__VA_ARGS__)
	#define TE_DWARN(f_, ...) TE_DEBUG_LOG_FUNCTION(TE_D2STR(TE_DEBUG_PREFIX),WARNING,TE_D2STR(__FILE__),__LINE__,(f_), ##__VA_ARGS__)
	#define TE_DERROR(f_, ...) TE_DEBUG_LOG_FUNCTION(TE_D2STR(TE_DEBUG_PREFIX),ERROR,TE_D2STR(__FILE__),__LINE__,(f_), ##__VA_ARGS__)
	#define TE_DFATAL(f_, ...) TE_DEBUG_LOG_FUNCTION(TE_D2STR(TE_DEBUG_PREFIX),FATAL,TE_D2STR(__FILE__),__LINE__,(f_), ##__VA_ARGS__)

	#define TE_DTRACE() te_debug_printStackTrace()

#else

	#define TE_DINFO(f_, ...)
	#define TE_DWARN(f_, ...)
	#define TE_DERROR(f_, ...)
	#define TE_DFATAL(f_, ...)
	
	#define TE_DTRACE()

#endif

/* END DEBUG HEADER */
 
#line 1 "windows.h"

/* WINDOWS PLATFORM LAYER HEADER */

#if defined(TE_WIN32)

#include <stdarg.h> // va_list;

#define TE_W32_WINAPI __stdcall
#define TE_W32_CALLBACK __stdcall

#define TE_W32_FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define TE_W32_FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#define TE_W32_FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200

#define TE_W32_LANG_NEUTRAL 0x00
#define TE_W32_SUBLANG_DEFAULT 0x01

#define TE_W32_MAKELANGID(p,s) ((((te_win32_WORD)(s)) << 10) | (te_win32_WORD)(p))

#define TE_W32_CS_OWNDC 0x0020

#define TE_W32_WS_CAPTION 0x00C00000L
#define TE_W32_WS_OVERLAPPED 0x00000000L
#define TE_W32_WS_SYSMENU 0x00080000L
#define TE_W32_WS_THICKFRAME 0x00040000L

#define TE_W32_PM_REMOVE 0x0001

#define TE_W32_CW_USEDEFAULT 0x80000000

#define TE_W32_MAKEINTRESOURCEA(i) ((te_win32_LPSTR)((te_win32_ULONG_PTR)((te_win32_WORD)(i))))
#define TE_W32_IDC_ARROW TE_W32_MAKEINTRESOURCEA(32512)

typedef void* te_win32_PVOID;
typedef void* te_win32_LPVOID;
typedef void* te_win32_HANDLE;
typedef void* te_win32_HLOCAL;
typedef void* te_win32_HBRUSH;
typedef void* te_win32_HICON;
typedef void* te_win32_HCURSOR;
typedef void* te_win32_HMENU;
typedef void* te_win32_HINSTANCE;
typedef void* te_win32_HMODULE;
typedef void* te_win32_HWND;

typedef const void* te_win32_LPCVOID;

typedef te_u16 te_win32_WORD;
typedef te_u32 te_win32_DWORD;

typedef te_u32 te_win32_UINT;

typedef te_i32 te_win32_LONG;

typedef te_i32 te_win32_BOOL;
typedef te_u16 te_win32_ATOM;

typedef char* te_win32_LPSTR;
typedef __nullterminated const char* te_win32_LPCSTR;

#if defined(_WIN64)
	typedef te_u64 te_win32_ULONG_PTR;
	typedef te_i64 te_win32_LONG_PTR;
#else
	typedef te_u32 te_win32_ULONG_PTR;
	typedef te_i32 te_win32_LONG_PTR;
#endif

typedef te_win32_LONG_PTR te_win32_LRESULT;

typedef te_win32_LRESULT (TE_W32_CALLBACK *te_win32_WindowProc)(te_win32_HWND hwnd,te_u32 uMsg, te_u32* wParam, void* lParam);

typedef struct te_win32_WNDCLASSEXA_T {
  te_u32 cbSize;
  te_u32 style;
  te_win32_WindowProc lpfnWndProc;
  te_i32 cbClsExtra;
  te_i32 cbWndExtra;
  te_win32_HINSTANCE hInstance;
  te_win32_HICON hIcon;
  te_win32_HCURSOR hCursor;
  te_win32_HBRUSH hbrBackground;
  te_win32_LPCSTR lpszMenuName;
  te_win32_LPCSTR lpszClassName;
  te_win32_HICON hIconSm;
} te_win32_WNDCLASSEXA;

typedef struct te_win32_SYSTEM_INFO_T {
  union {
    te_win32_DWORD dwOemId;
    struct {
      te_win32_WORD wProcessorArchitecture;
      te_win32_WORD wReserved;
    };
  };
  te_win32_DWORD dwPageSize;
  te_win32_LPVOID lpMinimumApplicationAddress;
  te_win32_LPVOID lpMaximumApplicationAddress;
  te_win32_DWORD* dwActiveProcessorMask;
  te_win32_DWORD dwNumberOfProcessors;
  te_win32_DWORD dwProcessorType;
  te_win32_DWORD dwAllocationGranularity;
  te_win32_WORD wProcessorLevel;
  te_win32_WORD wProcessorRevision;
} te_win32_SYSTEM_INFO;

typedef struct te_win32_SECURITY_ATTRIBUTES_T {
  te_win32_DWORD  nLength;
  te_win32_LPVOID lpSecurityDescriptor;
  te_win32_BOOL   bInheritHandle;
} te_win32_SECURITY_ATTRIBUTES;

typedef struct te_win32_POINT_T {
  te_win32_LONG x;
  te_win32_LONG y;
} te_win32_POINT;

typedef struct te_win32_MSG_T {
  te_win32_HWND   hwnd;
  te_win32_UINT   message;
  te_u32* wParam;
  void* lParam;
  te_win32_DWORD  time;
  te_win32_POINT  pt;
  te_win32_DWORD  lPrivate;
} te_win32_MSG;

typedef struct te_windowsApiFunctionTable_T {
	te_win32_HMODULE user32dll;
	int (TE_W32_WINAPI *MessageBoxA)(te_win32_HWND hWnd, te_win32_LPCSTR lpText, te_win32_LPCSTR lpCaption, te_u32 uType);
	te_win32_HWND (TE_W32_WINAPI *CreateWindowExA)(te_win32_DWORD dwExStyle, te_win32_LPCSTR lpClassName, te_win32_LPCSTR lpWindowName, te_win32_DWORD dwStyle, te_i32 X, te_i32 Y, te_i32 nWidth, te_i32 nHeight, te_win32_HWND hWndParent, te_win32_HMENU hMenu, te_win32_HINSTANCE hInstance, te_win32_LPVOID lpParam);
	te_win32_LRESULT (TE_W32_WINAPI *DefWindowProcA)(te_win32_HWND hwnd,te_u32 uMsg, te_u32* wParam, void* lParam);
	te_win32_ATOM (TE_W32_WINAPI *RegisterClassExA)(const te_win32_WNDCLASSEXA* lpClassExA);
	te_win32_HCURSOR (TE_W32_WINAPI *LoadCursorA)(te_win32_HINSTANCE hInstance, te_win32_LPCSTR lpCursorName);
  te_win32_BOOL (TE_W32_WINAPI *PeekMessageA)(te_win32_MSG* lpMsg, te_win32_HWND hWnd, te_win32_UINT wMsgFilterMin, te_win32_UINT wMsgFilterMax, te_win32_UINT wRemoveMsg);
	te_win32_BOOL (TE_W32_WINAPI *TranslateMessage)(const te_win32_MSG* lpMsg);
  te_win32_LRESULT (TE_W32_WINAPI *DispatchMessageA)(const te_win32_MSG* lpMsg);
  te_win32_BOOL (TE_W32_WINAPI *GetMessageA)(te_win32_MSG* lpMsg, te_win32_HWND  hWnd, te_win32_UINT wMsgFilterMin, te_win32_UINT wMsgFilterMax);
  te_win32_BOOL (TE_W32_WINAPI *DestroyWindow)(te_win32_HWND hWnd);
  te_win32_HMODULE ntdll;
	te_win32_HMODULE gdi32dll;
} te_windowsApiFunctionTable;

te_windowsApiFunctionTable te_win32 = {0};
const char* te_win32_windowClassName = "tinyengineWin32";

// Kernel32.lib functions

extern te_win32_HMODULE TE_W32_WINAPI GetModuleHandleA(te_win32_LPCSTR lpModuleName);
extern te_win32_HMODULE TE_W32_WINAPI LoadLibraryA(te_win32_LPCSTR lpLibFileName);
extern void* TE_W32_WINAPI GetProcAddress(te_win32_HMODULE hModule, te_win32_LPCSTR lpProcName);
extern te_win32_BOOL TE_W32_WINAPI FreeLibrary(te_win32_HMODULE hLibModule);
extern void TE_W32_WINAPI GetNativeSystemInfo(te_win32_SYSTEM_INFO* lpSystemInfo);
extern te_win32_HANDLE TE_W32_WINAPI CreateMutexA(te_win32_SECURITY_ATTRIBUTES* lpMutexAttributes, te_win32_BOOL bInitialOwner, te_win32_LPCSTR lpName);
extern te_win32_DWORD TE_W32_WINAPI WaitForSingleObject(te_win32_HANDLE hHandle, te_win32_DWORD dwMilliseconds);
extern te_win32_DWORD TE_W32_WINAPI GetLastError();
extern te_win32_DWORD TE_W32_WINAPI FormatMessageA(te_win32_DWORD dwFlags,te_win32_LPCVOID lpSource,te_win32_DWORD dwMessageId,te_win32_DWORD dwLanguageId,te_win32_LPSTR* lpBuffer,te_win32_DWORD nSize,va_list *Arguments);
extern te_win32_HLOCAL TE_W32_WINAPI LocalFree(te_win32_HLOCAL hMem);

te_bool_u8 te_p_win32_init();
void te_p_win32_terminate();

#endif

/* END WINDOWS PLATFORM LAYER HEADER */
 
#line 1 "graphics_driver_symbols.c"

/* GRAPHIC CARD DRIVER SYMBOLS */

#if !defined(TE_NO_GRAPHICS_CARD_SELECT_EXPORTS)
	// Used to flag executable as needing high performance graphics
	__declspec(dllexport) const te_u32 NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) const te_i32 AmdPowerXpressRequestHighPerformance = 1;
#endif

/* END GRAPHIC CARD DRIVER SYMBOLS */
 
#line 1 "debug.c"

/* DEBUG IMPLEMENTATION */

#if defined(TE_DEBUG)

	void te_debug_printf(const char* prefix, enum te_debug_logLevel logLevel, const char* fileName, te_u32 lineNumber, const char* format, ...) {
		
		FILE* outputStream;
		char* levelString;
		switch(logLevel) {
			case INFO: levelString = "INFO"; outputStream = stdout; break;
			case WARNING: levelString = "WARNING"; outputStream = stdout; break;
			case ERROR: levelString = "ERROR"; outputStream = stderr; break;
			case FATAL: levelString = "FATAL"; outputStream = stderr; break;
			default: levelString = "UNKNOWN"; outputStream = stdout; break;
		}
		fprintf(outputStream,"%s[%s][%s:%u] ",prefix,levelString,fileName,lineNumber);
		
		va_list args;
		
		va_start(args, format);
		vfprintf(outputStream,format,args);
		va_end(args);
	}
	
	void te_debug_printStackTrace() {
		// TODO: Implement
	}

	te_bool_u8 te_p_debugInit() {
		// TODO: Implement
		return TE_FALSE;
	}

	void te_p_debugTerminate() {
		// TODO: Implement
	}

#endif

/* END DEBUG IMPLEMENTATION */
 
#line 1 "windows.c"

/* WINDOWS PLATFORM LAYER */

#if defined(TE_WIN32)

te_win32_DWORD te_win32_checkError() {
	te_win32_DWORD errorCode = GetLastError();
	#if defined(TE_DEBUG)
		te_win32_LPSTR errorMessage = NULL;
		FormatMessageA(
			TE_W32_FORMAT_MESSAGE_ALLOCATE_BUFFER | TE_W32_FORMAT_MESSAGE_FROM_SYSTEM | TE_W32_FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, 
			errorCode, 
			TE_W32_MAKELANGID(TE_W32_LANG_NEUTRAL, TE_W32_SUBLANG_DEFAULT), 
			&errorMessage, 
			0, NULL
		);
		TE_DERROR("win32 error: %s",errorMessage);
		LocalFree(errorMessage);
	#endif
	return errorCode;
}

te_win32_LRESULT TE_W32_CALLBACK te_win32_windowProc( te_win32_HWND hWindow, te_u32 uMessage, te_u32* wParam, void* lParam) {
	switch(uMessage){
		
	}
	return te_win32.DefWindowProcA(hWindow,uMessage,wParam,lParam);
}

void te_win32_pollWindowEvents(te_win32_HWND window){
	te_win32_MSG message;

	while(te_win32.PeekMessageA(&message,window,0,0,TE_W32_PM_REMOVE)){
		te_win32.TranslateMessage(&message);
		te_win32.DispatchMessageA(&message);
	}
}

void te_win32_waitForWindowEvent(te_win32_HWND window){
	te_win32_MSG message;

	te_win32.GetMessageA(&message,window,0,0);
	te_win32.TranslateMessage(&message);
	te_win32.DispatchMessageA(&message);
}

void te_win32_pollAllWindowEvents(){ te_win32_pollWindowEvents(NULL); }

void te_win32_waitForAnyWindowEvent(){ te_win32_waitForWindowEvent(NULL); }

te_bool_u8 te_win32_registerEngineWindowClass() {
	te_win32_HMODULE hInstance = GetModuleHandleA(NULL);
	te_win32_WNDCLASSEXA windowClass = {
		.cbSize = sizeof(te_win32_WNDCLASSEXA),
		.style = TE_W32_CS_OWNDC | 0,
		.lpfnWndProc = &te_win32_windowProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = NULL,
		.hCursor = te_win32.LoadCursorA(hInstance, TE_W32_IDC_ARROW),
		.hbrBackground = NULL,
		.lpszMenuName = NULL,
		.lpszClassName = te_win32_windowClassName,
		.hIconSm = NULL
	};

	if(te_win32.RegisterClassExA(&windowClass) == 0){
		return TE_FALSE;
	}

	return TE_TRUE;
}

te_win32_HWND te_win32_createWindow(const char* title, te_u32 width, te_u32 height, void* userPointer) {
	te_win32_HWND window = te_win32.CreateWindowExA(
		0,
		te_win32_windowClassName, 
		title, 
		TE_W32_WS_OVERLAPPED | TE_W32_WS_CAPTION | TE_W32_WS_SYSMENU | TE_W32_WS_THICKFRAME, 
		TE_W32_CW_USEDEFAULT, TE_W32_CW_USEDEFAULT, width, height, 
		NULL, NULL, GetModuleHandleA(NULL), 
		userPointer
	);
	if(window == NULL) { te_win32_checkError(); return NULL; }
	return window;
}

void te_win32_destroyWindow(te_win32_HWND window) {
	te_win32.DestroyWindow(window);
}

te_bool_u8 te_win32_init() {
	
	TE_DINFO("tinyengine win32 backend loaded.\n");

	// TODO: Crash in a user understanable way here?
	// TODO: Wrap function loading to error check
	te_win32.user32dll = LoadLibraryA("user32.dll");
	if(te_win32.user32dll == NULL) { TE_DERROR("Could not load user32.dll!\n"); return TE_FALSE; }
	te_win32.MessageBoxA = GetProcAddress(te_win32.user32dll,"MessageBoxA");
	te_win32.CreateWindowExA = GetProcAddress(te_win32.user32dll,"CreateWindowExA");
	te_win32.DefWindowProcA = GetProcAddress(te_win32.user32dll,"DefWindowProcA");
	te_win32.RegisterClassExA = GetProcAddress(te_win32.user32dll,"RegisterClassExA");
	te_win32.LoadCursorA = GetProcAddress(te_win32.user32dll,"LoadCursorA");
	te_win32.PeekMessageA = GetProcAddress(te_win32.user32dll,"PeekMessageA");
	te_win32.TranslateMessage = GetProcAddress(te_win32.user32dll,"TranslateMessage");
  te_win32.DispatchMessageA = GetProcAddress(te_win32.user32dll,"DispatchMessageA");
  te_win32.GetMessageA = GetProcAddress(te_win32.user32dll,"GetMessageA");
  te_win32.DestroyWindow = GetProcAddress(te_win32.user32dll,"DestroyWindow");

	te_win32.ntdll = LoadLibraryA("ntdll.dll");
	if(te_win32.ntdll == NULL) { TE_DERROR("Could not load ntdll.dll!\n"); return TE_FALSE; }

#if defined(TE_DEBUG)
	const char* (__cdecl *wine_get_version)();
	void (__cdecl *wine_get_host_version)(const char **sysname, const char **release);
	wine_get_version = GetProcAddress(te_win32.ntdll, "wine_get_version");
	wine_get_host_version = GetProcAddress(te_win32.ntdll, "wine_get_host_version");
	if(wine_get_version != NULL) { TE_DINFO ("running on wine version %s\n",wine_get_version()); }
	if(wine_get_host_version != NULL) {
		const char* sysname;
		const char* release;
		wine_get_host_version(&sysname,&release);
		TE_DINFO ("wine host: %s %s\n",sysname,release);
	}
#endif

	te_win32.gdi32dll = LoadLibraryA("gdi32.dll");
	if(te_win32.gdi32dll == NULL) { TE_DERROR("Could not load gdi32.dll!\n"); return TE_FALSE; }

	te_win32_SYSTEM_INFO sysinfo;
	GetNativeSystemInfo(&sysinfo);

	TE_DINFO("Page size: %lu\n",sysinfo.dwPageSize);
	TE_DINFO("Address Range: 0x%x-0x%x\n",sysinfo.lpMinimumApplicationAddress, sysinfo.lpMaximumApplicationAddress);
	TE_DINFO("Logical Proccesors: %lu\n",sysinfo.dwNumberOfProcessors);

#if !defined(TE_HEADLESS)
	if(!te_win32_registerEngineWindowClass()) { TE_DERROR("Could not register windowClass!\n"); return TE_FALSE; }
#endif

	return TE_TRUE;
}

void te_win32_terminate() {
	// TODO: Undo all init work after init is in a stable place
	// TODO: Add options to skip any uninit thats handled by the proccess closing
}

#endif

/* END WINDOWS PLATFORM LAYER */
 
#line 1 "core.c"

/* ENGINE CORE IMPLEMENTATION */

te_bool_u8 te_init() {

	#if defined(TE_DEBUG)
		te_p_debugInit();
	#endif

	TE_DINFO("tinyengine version %i.%i.%i\n",TE_VERSION_MAJOR,TE_VERSION_MINOR,TE_VERSION_BUILD);
	//TE_DINFO("Fixed engine state size: %lu bytes\n",sizeof(tinyengine_state));

	#if defined(TE_LINUX)
		TE_DINFO("tinyengine linux backend loaded.\n");
	#elif defined(TE_WIN32)
		te_win32_init();
	#else
		TE_DWARN("tinyengine compiled for unknown platform!\n");
	#endif


	return TE_TRUE;
}

void te_terminate() {}

/* END ENGINE CORE IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif

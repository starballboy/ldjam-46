
#define TE_DEBUG
#define TE_DEBUG_LEVEL_INFO
#include "tinyengine.c"

int main() {
	tinyengine_init();

	tinyengine_windowContext* window = tinyengine_createWindow();
	if(!window) { return -1; }

	tinyengine_setWindowTitle(window,"LDJAM 46");
	tinyengine_setWindowSize(window,640,360); // 16:9 aspect ratio
	tinyengine_setWindowMinSize(window,640,360);
	tinyengine_setWindowAspectRatio(window,16,9);

	tinyengine_showWindow(window);
	tinyengine_makeCurrent(window);
	_tinyengine_gl3_init();
	_tinyengine_gl3_createWindowRenderContext(window);

	while(!window->closeRequested) {
		_tinyengine_gl3_startFrame(window);
		_tinyengine_gl3_endFrame(window);
		tinyengine_swapBuffers(window);
		tinyengine_pollEvents();
	}

	tinyengine_terminate();
	return 0;
}

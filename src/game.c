
#define TE_DEBUG
#define TE_DEBUG_LEVEL_INFO
#include "tinyengine.c"

int main() {
	tinyengine_init();

	tinyengine_windowContext* window = tinyengine_createWindow();
	if(!window) { return -1; }

	tinyengine_setWindowTitle(window,"tinyengine");
	tinyengine_setWindowSize(window,640,360); // 16:9 aspect ratio
	tinyengine_setWindowMinSize(window,640,360);
	tinyengine_setWindowAspectRatio(window,16,9);

	tinyengine_showWindow(window);
	tinyengine_makeCurrent(window);
	_tinyengine_gl3_init();
	_tinyengine_gl3_createWindowRenderContext(window);
	_tinyengine_gl3_updateView(window,640,360);

	while(!window->closeRequested) {
		_tinyengine_gl3_startFrame(window);
		_tinyengine_gl3_drawRectangle2D(window, 10, 10, 200, 100, (te_v4_f32){1.0f,0.0f,0.0f,1.0f});
		_tinyengine_gl3_drawRectangle2D(window, 20, 150, 100, 50, (te_v4_f32){0.0f,1.0f,0.0f,1.0f});
		_tinyengine_gl3_drawRectangle2D(window, 230, 10, 50, 200, (te_v4_f32){0.0f,0.0f,1.0f,1.0f});
		_tinyengine_gl3_endFrame(window);
		tinyengine_swapBuffers(window);
		tinyengine_pollEvents();
	}

	tinyengine_terminate();
	return 0;
}

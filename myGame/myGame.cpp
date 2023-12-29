#include "utils.cpp"
#include <windows.h>
#include <iostream>

/*

wnd - window
proc - process
h - handler

UINT - unsigned integer
param - parameter
l - long
rect - rectangle >> прямоугольник
alloc - allocate - размечать память

когда рисуем
h - horizontal
v - vertical

default - по умолчанию
use - использовать
destroy - разрушить
close - закрыть

src - source - откуда
dst - destination - куда

	 top
left      right
	bottom
*/

globalVariable bool running = true;

struct RenderState {
	void* memory;
	int width, height;
	BITMAPINFO bitmapInfo;
};

globalVariable RenderState renderState;
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK windowClick(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
	} break;

	case WM_SIZE: { // resize
		RECT rect;
		GetClientRect(hWnd, &rect);

		renderState.width = rect.right - rect.left; // renderState.width >> renderState.width
		renderState.height = rect.bottom - rect.top;

		int bufferSize = renderState.width * renderState.height * sizeof(unsigned int);

		if (renderState.memory)
			VirtualFree(renderState.memory, 0, MEM_RELEASE);
		renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
		renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
		renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
		renderState.bitmapInfo.bmiHeader.biPlanes = 1;
		renderState.bitmapInfo.bmiHeader.biBitCount = 32;
		renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;

	} break;

	default: {
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	} break;
	}
	return result;
};


// точка входа в программу
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// создать класс window (окна)
	WNDCLASS WindowClass = {};
	WindowClass.style = CS_HREDRAW | CS_VREDRAW; // <--- задаём когда будет перерисовываться окно
	WindowClass.lpszClassName = TEXT("Game Window Class"); // <--- название класса окна
	WindowClass.lpfnWndProc = windowClick; // <--- мы привязываем к классу функцию обработчик кликов по окну

	// зарегистрировать этот класс
	RegisterClass(&WindowClass); // <--- зарегистрировали класс в нашем приложении

	// создать окно
	HWND window = CreateWindow(WindowClass.lpszClassName, TEXT("Pong - Tutorial"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Input input = {};

	while (running)
	{
		// ввод
		MSG message;

		for (size_t i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				u32 vkCode = (u32)message.wParam;
				bool isDown = ((message.lParam & (1 << 31)) == 0);

#define processButton(b, vk)\
case vk: {\
	input.buttons[b].isDown = true;\
	input.buttons[b].changed = true;\
} break;
				switch (vkCode)
				{
					processButton(BUTTON_UP, VK_UP);
					processButton(BUTTON_DOWN, VK_DOWN);
					processButton(BUTTON_LEFT, VK_LEFT);
					processButton(BUTTON_RIGHT, VK_RIGHT);
				}


			} break;
			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			}

		}
		// симуляция - процесс игры
		simulateGame(&input);

		// перерисовка
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
};
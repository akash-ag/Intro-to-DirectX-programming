// the followinf code creates a window and displays "Hello, World!" when the user clicks on it
#include <Windows.h>

HWND ghMainWnd = 0; // the handle to the window we will be creating

bool InitWindowsApp(HINSTANCE instanceHandle, int show); // creates the window - returns true if successfull, false otherwise
int Run(); // maintains the message queue loop
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam); // our callback code - this will be replaced by our custom functionality

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) // our main function - entry point to our program
{
	if (!InitWindowsApp(hInstance, nShowCmd))
		return 0;

	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	WNDCLASS wc; // struct to set the properties for our window

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION); // class icon
	wc.hCursor = LoadCursor(0, IDC_ARROW); // class mouse cursor type
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "BasicWndClass";

	if (!RegisterClass(&wc)) // register the class and all its details (including information about the callback) before initializing it
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		return false;
	}


	ghMainWnd = CreateWindow( // initializing the window here - this function allocates memory for our window
		"BasicWndClass",
		"TestWindow", // title of our window
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instanceHandle, // handle to the window
		0);

	if (ghMainWnd == 0)
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		return false;
	}

	ShowWindow(ghMainWnd, show); // displays the window 
	UpdateWindow(ghMainWnd);

	return true;
}

int Run() // our primary message loop - essentially an infinite loop
{
	MSG msg = { 0 };

	BOOL bRet = 1;
	while ((bRet == GetMessage(&msg, 0, 0, 0)) != 0) // checks and pulls events from the event queue
												     // refer to the publisher/subscriber deisgn pattern for more details
	{
		if (bRet == -1)
		{
			MessageBox(0, "GetMessage FAILED", "Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg); // posts the messages to the calling threads message queue for processing - to be used when GetMessage() is called again
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	// our callback that runs in response to a message
															// of type MSG [as in the Run() funstion above] - this gets triggered when the user does something 
{															// note that unlike many other GUI frameworks, we don't explicitly call this function
															// Windows calls it for us when a message is processed
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, "Hello, World!", "Callback Action", MB_OK); // display the message when user clicks the mouse
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd); // kill the window when user presses the ESC key
		return 0;

	case WM_DESTROY: // user clicks the close button
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam); // forwards any unhandled message to the default message handler
}
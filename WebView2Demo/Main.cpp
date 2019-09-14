#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

// DWORD: The style of the window being created.
//
// Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowa#remarks
static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szLabelClass[] = _T("STATIC");
static TCHAR szTextboxClass[] = _T("EDIT");
static TCHAR szButtonClass[] = _T("BUTTON");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("YouTube Re-Player Desktop");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Pointer to WebView window
static ComPtr<IWebView2WebView> webviewWindow;

const int youtubeIdLength = 12;
HWND textBoxYoutubeId;
char enteredYoutubeId[youtubeIdLength];

const int buttonSubmitCommand = 1;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	//
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowa
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1300, 900,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	textBoxYoutubeId = CreateWindow(szTextboxClass,
		_T(""),
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		160, 10, 400, 30,
		hWnd, NULL, NULL, NULL);

	CreateWindow(szButtonClass,
		_T("Enjoy"),
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		570, 10, 80, 30,
		hWnd, (HMENU)buttonSubmitCommand, NULL, NULL);

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// <-- WebView2 sample code starts here -->

	// Known issue - app needs to run on PerMonitorV2 DPI awareness for WebView to look properly
	// This is only available on Win10, comment out the line on older OS versions.
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Locate the browser and set up the environment for WebView
	CreateWebView2EnvironmentWithDetails(nullptr, nullptr, nullptr,
		Callback<IWebView2CreateWebView2EnvironmentCompletedHandler>(
			[hWnd](HRESULT result, IWebView2Environment* env) -> HRESULT {

		// Create a WebView, whose parent is the main window hWnd
		env->CreateWebView(hWnd, Callback<IWebView2CreateWebViewCompletedHandler>(
			[hWnd](HRESULT result, IWebView2WebView* webview) -> HRESULT {
			if (webview != nullptr) {
				webviewWindow = webview;
			}

			// Add a few settings for the webview
			// this is a redundant demo step as they are the default settings values
			IWebView2Settings* Settings;
			webviewWindow->get_Settings(&Settings);
			Settings->put_IsScriptEnabled(TRUE);
			Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
			Settings->put_IsWebMessageEnabled(TRUE);

			// Resize WebView to fit the bounds of the parent window
			RECT bounds;
			GetClientRect(hWnd, &bounds);
			bounds.top += 50;
			webviewWindow->put_Bounds(bounds);

			// Schedule an async task to navigate to Bing
			webviewWindow->Navigate(L"https://www.youtube.com/");

			// Register an IWebView2NavigationStartingEventHandler to cancel any non-https navigation
			EventRegistrationToken token;
			webviewWindow->add_NavigationStarting(Callback<IWebView2NavigationStartingEventHandler>(
				[](IWebView2WebView* webview, IWebView2NavigationStartingEventArgs* args) -> HRESULT {
				PWSTR uri;
				args->get_Uri(&uri);
				std::wstring source(uri);
				if (source.substr(0, 5) != L"https") {
					args->put_Cancel(true);
				}
				CoTaskMemFree(uri);
				return S_OK;
			}).Get(), &token);

			// Schedule an async task to add initialization script that freezes the Object object
			webviewWindow->AddScriptToExecuteOnDocumentCreated(L"Object.freeze(Object);", nullptr);
			// Schedule an async task to get the document URL
			webviewWindow->ExecuteScript(L"window.document.URL;", Callback<IWebView2ExecuteScriptCompletedHandler>(
				[](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
				LPCWSTR URL = resultObjectAsJson;
				//doSomethingWithURL(URL);
				return S_OK;
			}).Get());

			// Set an event handler for the host to return received message back to the web content
			webviewWindow->add_WebMessageReceived(Callback<IWebView2WebMessageReceivedEventHandler>(
				[](IWebView2WebView* webview, IWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
				PWSTR message;
				args->get_WebMessageAsString(&message);
				// processMessage(&message);
				webview->PostWebMessageAsString(message);
				CoTaskMemFree(message);
				return S_OK;
			}).Get(), &token);

			return S_OK;
		}).Get());
		return S_OK;
	}).Get());

	// <-- WebView2 sample code ends here -->

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// How to convert std::string to LPCWSTR in C++ (Unicode)
//
// Reference: https://stackoverflow.com/a/27296/1177328
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
};

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR youtubeIdLabelText[] = _T("YouTube Video ID:");

	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc,
			10, 10,
			youtubeIdLabelText, _tcslen(youtubeIdLabelText));

		EndPaint(hWnd, &ps);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case buttonSubmitCommand:
			int getWindowTextStatus = 0;

			getWindowTextStatus = GetWindowTextA(textBoxYoutubeId, &enteredYoutubeId[0], youtubeIdLength);

			if (getWindowTextStatus == 0) 
			{
				MessageBox(hWnd,
					_T("Invalid input"),
					_T("Error"),
					NULL);
			}

			std::string url = "https://www.youtube.com/embed/";
			url += enteredYoutubeId;
			url += "?autoplay=1&loop=1&playlist=";
			url += enteredYoutubeId;	

			std::wstring stemp = s2ws(url);
			LPCWSTR result = stemp.c_str();

			webviewWindow->Navigate(result);

			break;
		}

		break;
	case WM_SIZE:
		if (webviewWindow != nullptr) {
			RECT bounds;
			GetClientRect(hWnd, &bounds);
			bounds.top += 50;
			webviewWindow->put_Bounds(bounds);
		};

		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

		break;
	}

	return 0;
}


# WebView2-YouTubeReplayer (WV2YTR)
A YouTube Replayer built with the Microsoft Edge WebView2 control.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

![Home screen of the program](github-images/screenshot.png?raw=true)

Inspired by WebView2Browser, WV2YTR is a sample Windows desktop application demonstrating the WebView2 control capabilities. It is built as a Win32 Visual Studio 2019 (v16.3 Preview 3) project with WebView2 to power its features.

This programme shows some of the simplest uses of WebView2, such as creating and navigating a WebView. In the traditional WebView (with Internet Explorer engine), it is not possible to loop YouTube video with its embed URL. Hence, now with WebView2, this feature is finally possible.

## Requisites
- [Microsoft Edge (Chromium)](https://www.microsoftedgeinsider.com/en-us/download/) installed on a supported OS.
- [Visual Studio](https://visualstudio.microsoft.com/vs/) with C++ support installed.

## Contributions are Welcome!

This project will continue to evolve. We will do modifications all the time as long as Azure DevOps and Azure Portal do not stop changing.

Any help will be appreciated. You can develop new features, fix bugs, improve the documentation, or do some other cool stuff.

If you have new ideas or want to complain about bugs, feel free to [create a new issue](https://github.com/goh-chunlin/WebView2-YouTubeReplayer/issues/new).

## Code of Conduct

This project has adopted the code of conduct defined by the [Contributor Covenant](http://contributor-covenant.org/)
to clarify expected behavior in our community.

## Singapore .NET Developers Community

This project is supported by the [Singapore .NET Developers Community](http://dotnet.sg).

## References
- [WebView2Browser: A rich sample for WebView2](https://blogs.windows.com/msedgedev/2019/08/15/webview2browse-sample-for-webview2/#rzhJEBfYkul3Mh7y.97)
- [Getting Started with WebView2 (developer preview)](https://docs.microsoft.com/en-us/microsoft-edge/hosting/webview2/gettingstarted#step-1---create-a-single-window-win32-app)
- [Microsoft.Web.WebView2](https://www.nuget.org/packages/Microsoft.Web.WebView2)
- [GitHub: HelloWebView - start](https://github.com/MicrosoftEdge/WebView2Samples)
- [Windows GDI](https://docs.microsoft.com/en-us/windows/win32/api/_gdi/)
- [Walkthrough: Create a traditional Windows Desktop application (C++)](https://docs.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=vs-2019)
- [WinMain function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain)
- [YouTube: Windows.h C++ Tutorial 5: Creating a Text Field and Storing its Content](https://www.youtube.com/watch?v=FXrulJXMiU8)
- [Using Rectangles](https://docs.microsoft.com/en-us/windows/win32/gdi/using-rectangles)
- [CreateWindowA macro](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowa#remarks)
- [GetWindowTextA function](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowtexta)
- [Window Styles](https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles)
- [How to convert std::string to LPCWSTR in C++ (Unicode)](https://stackoverflow.com/a/27296/1177328)
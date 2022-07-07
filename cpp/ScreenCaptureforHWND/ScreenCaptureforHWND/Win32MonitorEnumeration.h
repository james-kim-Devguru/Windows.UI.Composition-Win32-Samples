#pragma once
#include <dwmapi.h>

struct Monitor
{
public:
	Monitor(nullptr_t) {}
	Monitor(HMONITOR hMonitor, std::wstring const& title)
	{
		m_hMonitor = hMonitor;
		m_title = title;
	}

	HMONITOR Hmonitor() const noexcept { return m_hMonitor; }
	std::wstring Title() const noexcept { return m_title; }

private:
	HMONITOR m_hMonitor;
	std::wstring m_title;
};

std::wstring GetWindowText(HMONITOR hMonitor)
{
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	::GetMonitorInfo(hMonitor, &mi);

	std::wstring title(mi.szDevice);
	return title;
}

//bool IsAltTabWindow(Window const& window)
//{
//	HWND hwnd = window.Hwnd();
//	HWND shellWindow = GetShellWindow();
//
//	auto title = window.Title();
//	auto className = window.ClassName();
//
//	if (hwnd == shellWindow)
//	{
//		return false;
//	}
//
//	if (title.length() == 0)
//	{
//		return false;
//	}
//
//	if (!IsWindowVisible(hwnd))
//	{
//		return false;
//	}
//
//	if (GetAncestor(hwnd, GA_ROOT) != hwnd)
//	{
//		return false;
//	}
//
//	LONG style = GetWindowLong(hwnd, GWL_STYLE);
//	if (!((style & WS_DISABLED) != WS_DISABLED))
//	{
//		return false;
//	}
//
//	DWORD cloaked = FALSE;
//	HRESULT hrTemp = DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &cloaked, sizeof(cloaked));
//	if (SUCCEEDED(hrTemp) &&
//		cloaked == DWM_CLOAKED_SHELL)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
//{
//	auto class_name = GetClassName(hwnd);
//	auto title = GetWindowText(hwnd);
//
//	auto window = Window(hwnd, title, class_name);
//
//	if (!IsAltTabWindow(window))
//	{
//		return TRUE;
//	}
//
//	std::vector<Window>& windows = *reinterpret_cast<std::vector<Window>*>(lParam);
//	windows.push_back(window);
//
//	return TRUE;
//}
//
//const std::vector<Window> EnumerateWindows()
//{
//	std::vector<Window> windows;
//	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));
//
//
//	return windows;
//}

BOOL CALLBACK EnumMonitorProc(HMONITOR hMonitor, HDC hdc, LPRECT lPrect, LPARAM lParam)
{
	std::vector<Monitor>& monitors = *reinterpret_cast<std::vector<Monitor>*>(lParam);

	auto title = GetWindowText(hMonitor);

	auto monitor = Monitor(hMonitor, title);
	monitors.push_back(monitor);

	return TRUE;
}

const std::vector<Monitor> EnumerateMonitors()
{
	std::vector<Monitor> monitors;
	::EnumDisplayMonitors(NULL, NULL, EnumMonitorProc, reinterpret_cast<LPARAM>(&monitors));

	return monitors;
}
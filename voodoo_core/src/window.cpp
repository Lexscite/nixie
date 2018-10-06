// This file is part of Voodoo Engine.
//
// Voodoo Engine is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Voodoo Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <https://www.gnu.org/licenses/>.

#include "../include/voodoo/window.h"

namespace voodoo {
bool Window::Init(HINSTANCE instance, int width, int height,
                  std::wstring caption) {
  // Register window class
  WNDCLASSEX wc;
  std::wstring wc_name = caption + L"Window";
  DWORD style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;

  memset(&wc, 0, sizeof(wc));
  wc.cbClsExtra = NULL;
  wc.cbWndExtra = NULL;
  wc.cbSize = sizeof(wc);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.hInstance = instance;
  wc.lpfnWndProc = MsgRouter;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = wc_name.c_str();

  if (!RegisterClassEx(&wc)) {
    return false;
  }

  // Create window
  handle_ =
      CreateWindowEx(WS_EX_APPWINDOW, wc_name.c_str(), caption.c_str(), style,
                     0, 0, width, height, NULL, NULL, instance, NULL);
  if (!handle_) {
    return false;
  }

  // Adjust window position and size
  int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
  int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
  RECT wr = {x, y, x + width, y + height};
  DWORD ws = static_cast<DWORD>(GetWindowLongPtr(handle_, GWL_STYLE));
  DWORD wexs = static_cast<DWORD>(GetWindowLongPtr(handle_, GWL_EXSTYLE));
  AdjustWindowRectEx(&wr, ws, false, wexs);
  SetWindowPos(handle_, NULL, wr.left, wr.top, wr.right - wr.left,
               wr.bottom - wr.top, NULL);

  // Display window
  ShowWindow(handle_, SW_SHOW);

  return true;
}

HWND Window::GetHandle() { return handle_; }

Rect<int> Window::GetRect() {
  RECT r;
  GetClientRect(handle_, &r);
  return Rect<int>(static_cast<int>(r.left), static_cast<int>(r.top),
                   static_cast<int>(r.right - r.left),
                   static_cast<int>(r.bottom - r.top));
}

int Window::GetWidth() { return GetRect().size.x; }

int Window::GetHeight() { return GetRect().size.y; }

// Actual member msg processor.
LRESULT CALLBACK Window::MsgProc(HWND handle, UINT msg, WPARAM w_param,
                                 LPARAM l_param) {
  switch (msg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_MOVE:
      if (LOWORD(w_param) == WM_MOVING) {
      } else {
      }
      return 0;
    case WM_MENUCHAR:
      return MAKELRESULT(0, MNC_CLOSE);
    case WM_GETMINMAXINFO:
      ((MINMAXINFO*)l_param)->ptMinTrackSize.x = 200;
      ((MINMAXINFO*)l_param)->ptMinTrackSize.y = 200;
      return 0;
    case WM_ACTIVATEAPP:
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    default:
      return DefWindowProc(handle, msg, w_param, l_param);
  }
}

// Static window msg router.
LRESULT CALLBACK Window::MsgRouter(HWND handle, UINT msg, WPARAM w_param,
                                   LPARAM l_param) {
  if (msg == WM_NCCREATE) {
    SetWindowLongPtr(handle, GWLP_USERDATA,
                     (LONG_PTR)((CREATESTRUCT*)l_param)->lpCreateParams);
    return TRUE;
  }

  return ((Window*)GetWindowLongPtr(handle, GWLP_USERDATA))
      ->MsgProc(handle, msg, w_param, l_param);
}
}  // namespace voodoo
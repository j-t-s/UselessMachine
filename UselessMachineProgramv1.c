/*
	Useless Machine Program
	This C program was inspired by the Useless Machine which turns off a switch briefly after it was turned on.
	This program creates a window and moves the cursor to close the window.
	The user can move the mouse cursor, but the program will continuously attempt to close the window.
	The operating system required is Microsoft Windows.
	Author: jts
	Version: 1.2016.5.18
*/
#include <windows.h>

#define ID_TIMER 2

RECT rect;

void CenterWindow(HWND hWnd);
void SelfClose(HWND hWnd);

VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message) {
        case WM_CREATE:
            CenterWindow(hwnd);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, ID_TIMER);
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (VK_ESCAPE == wParam)
                DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    MSG msg;
    WNDCLASS wc;
    HWND hwnd;
    ZeroMemory(&wc, sizeof wc);
    wc.hInstance = hInstance;
    wc.lpszClassName = "Useless Machine";
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.style = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	
    if (FALSE == RegisterClass(&wc))
        return 0;
    hwnd = CreateWindow(
       "Useless Machine",
       "Useless Machine",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        360,//CW_USEDEFAULT,
        240,//CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0);

    if (NULL == hwnd)
        return 0;
		
    SetTimer(hwnd, ID_TIMER, 30, TimerProc);
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	
    return msg.wParam;
}
/*
	CenterWindow method was retrieved from Tiny C Compiler's Example "hello_win.c"
*/
void CenterWindow(HWND hwnd_self){
    HWND hwnd_parent;
    RECT rw_self, rc_parent, rw_parent;
    int xpos, ypos;

    hwnd_parent = GetParent(hwnd_self);
    if (NULL == hwnd_parent)
        hwnd_parent = GetDesktopWindow();

    GetWindowRect(hwnd_parent, &rw_parent);
    GetClientRect(hwnd_parent, &rc_parent);
    GetWindowRect(hwnd_self, &rw_self);

    xpos = rw_parent.left + (rc_parent.right + rw_self.left - rw_self.right) / 2;
    ypos = rw_parent.top + (rc_parent.bottom + rw_self.top - rw_self.bottom) / 2;

    SetWindowPos(
        hwnd_self, NULL,
        xpos, ypos, 0, 0,
        SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE
        );
}
/*
	Move the cursor closer to the close button, when it reaches there close the window.
*/
void SelfClose(HWND hwndSelf){
	RECT rw_self;
	POINT curPos;
	GetWindowRect(hwndSelf, &rw_self);
	GetCursorPos(&curPos);
	
	int delta = 0;
	if (curPos.x != rw_self.right - 30){
		delta = (rw_self.right - 30 - curPos.x)/5;
		curPos.x += delta;
		if (delta == 0){
			curPos.x = rw_self.right - 30;
		}
	}
	if (curPos.y != rw_self.top + 10){
		delta = (rw_self.top + 10 - curPos.y)/5;
		curPos.y += delta;
		if (delta == 0){
			curPos.y = rw_self.top + 10;
		}
	}
	/*
	if (curPos.x < rw_self.right - 30){curPos.x++;}
	if (curPos.x > rw_self.right - 30){curPos.x--;}
	if (curPos.y > rw_self.top + 10){curPos.y--;}
	if (curPos.y < rw_self.top + 10){curPos.y++;}
	*/
	
	if ((curPos.y == rw_self.top + 10) && (curPos.x == rw_self.right - 30)){
		DestroyWindow(hwndSelf);
	}
	SetCursorPos(curPos.x, curPos.y);
	//I tried to make it click the close button, but it was easier just to destroy the window.
	//INPUT Input;
	//Input.type = INPUT_MOUSE;
	//Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	//SendInput(1,&Input,sizeof(INPUT));
}

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime){
	SelfClose(hwnd);
}

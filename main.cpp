///-///////////-///
///             ///
/// LINK:       ///
///   gdi32     ///
///   user32    ///
///   kernel32  ///
///   comctl32  ///
///             ///
///-///////////-///

#include <windows.h>

///Includes
#include "backend/adapter.h"
#include "backend/NetScan.h"
#include "backend/ARPSpoof.h"
///Includes

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int wndWidth  = 700;
int wndHeight = 400;

char wndTitle[] = "NetTools";
char wndClass[] = "main";

HWND hMainWnd;
HINSTANCE hInst;

int WINAPI WinMain (HINSTANCE hInsta,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow){
    hInst = hInsta;
    WNDCLASSEX wincl;

    wincl.cbSize        = sizeof (WNDCLASSEX);
    wincl.hInstance     = hInst;
    wincl.lpszClassName = wndClass;
    wincl.lpfnWndProc   = WndProc;
    wincl.style         = CS_HREDRAW | CS_VREDRAW;

    wincl.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm       = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName  = NULL;
    wincl.cbClsExtra    = 0;
    wincl.cbWndExtra    = 0;
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    if (!RegisterClassEx (&wincl))
        return 0;

    int xPos = (GetSystemMetrics(SM_CXSCREEN) - wndWidth) / 2;
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - wndHeight) / 2;

    hMainWnd = CreateWindow(wndClass, wndTitle, WS_OVERLAPPEDWINDOW, xPos, yPos, wndWidth, wndHeight, NULL, NULL, hInst, NULL);

    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

Adapter adp;
NetScan scan;
ARPSpoof spoof;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch (msg){
        case WM_CREATE: {
            ///onCreate
            adp.enumAdapters();
            cout<<"Adapters: "<<endl;
            for(int i = 0; i < adp.nAdapters(); i++){
                cout<<"  "<<i+1<<". "<<adp.getAdapter(i)->Description<<endl;
            }
            int iAdp;
            cout<<"Choose adapter: ";
            cin>>iAdp;
            adp.open(iAdp - 1);

            scan.setAdapter(&adp);

            UINT tScan = 0;
            cout<<"Scan time (ms): ";
            cin>>tScan;
            scan.setScanTime(tScan);

            CHAR fARP = 0;
            cout<<"Enable ARPWatch (y/n): ";
            cin>>fARP;
            if(fARP == 'y')
                scan.switchARPWatch();
            scan.scan(adp.address().ip & adp.address().mask, (adp.address().ip & adp.address().mask) | ~adp.address().mask);

            Sleep(tScan);

            for(int i = 0; i < scan.data.size(); i++){
                cout<<i+1<<". ";
                printIP(ntohl(scan.data[i].ip));
                cout<<endl;
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
            ///onPaint
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
            ///onDestroy
            scan.join();
            //spoof.join();
            adp.close();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

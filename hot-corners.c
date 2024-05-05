#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double currentElapsedTime = 0;
double lastElapsedTime = 0;
double intervalTime = 0;
double minIntervalTime = 800; // ms

HHOOK hMouseHook;
int numberOfTriggers = 0;
__declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        switch(wParam) {
            case WM_MOUSEMOVE:
            {
                MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
                if (pMouseStruct != NULL) {
                    if (pMouseStruct->pt.x <= 0 && pMouseStruct->pt.y <= 0) { // Top left corner but you could change the corner
                        numberOfTriggers++;
                        //printf("Mouse is in the top left corner of the screen %d.\n", numberOfTriggers);
                        #include <windows.h>

                        // Inside your function where you want to get the current time
                        LARGE_INTEGER frequency;        // ticks per second
                        LARGE_INTEGER t1;               // ticks
                        double elapsedTime;

                        // get ticks per second
                        QueryPerformanceFrequency(&frequency);

                        // start timer
                        QueryPerformanceCounter(&t1);

                        elapsedTime = (t1.QuadPart) * 1000.0 / frequency.QuadPart;
                        currentElapsedTime = elapsedTime;
                        intervalTime = currentElapsedTime - lastElapsedTime;
                        lastElapsedTime = currentElapsedTime;


                        //printf("Interval time : %.3lf ms.\n", intervalTime); // Print the time between two calls
                        if (intervalTime > minIntervalTime) {
                            //printf("Interval time is more than 1000 ms.\n");
                            // Do something here
                            keybd_event(VK_LWIN, 0, 0, 0); // Press the left Windows key
                            keybd_event(VK_TAB, 0, 0, 0); // Press the Tab key

                            keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); // Release the Tab key
                            keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0); // Release the left Windows key
                        }

                    }
                }
            }
        }
    }

    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

int main(int argc, char* argv[]) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!hInstance) hInstance = LoadLibrary((LPCSTR) argv[0]);
    if (!hInstance) return 1;
    
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, NULL); // USED TO BE NULL

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    UnhookWindowsHookEx(hMouseHook);
    return 0;
}
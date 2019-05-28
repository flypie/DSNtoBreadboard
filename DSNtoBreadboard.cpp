// DSNtoBreadboard.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "commdlg.h"

#include <string>

#include "shobjidl_core.h"


#include "DSNtoBreadboard.h"
#include "DSNFile.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


DSNFile	*DSNFileIn=0;


int		DListLen;
std::wstring* DList[1000];

#define	MAXFILENAME	260


void AddToLog(const std::wstring FileName)
{
	if(!DList[DListLen])
	{
		DList[DListLen]=new std::wstring;
	}

	*DList[DListLen++]+=FileName;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DSNTOBREADBOARD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DSNTOBREADBOARD));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DSNTOBREADBOARD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DSNTOBREADBOARD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}




HRESULT BasicFileOpen(HWND hWnd,std::wstring &File)
{
    WCHAR szFile[260];       // buffer for file name
	OPENFILENAME ofn;

    HRESULT hr=S_FALSE;

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAXFILENAME;
    ofn.lpstrFilter = _TEXT("DSN\0*.DSN\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

	if(GetOpenFileName(&ofn))
	{
		hr=S_OK;
		File=szFile;
	}

	return hr;
}

HRESULT BasicFileSave(HWND hWnd,std::wstring& File)
{
	WCHAR szFile[260];       // buffer for file name
	OPENFILENAME ofn;

	HRESULT hr=S_FALSE;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=hWnd;
	ofn.lpstrFile=szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0]='\0';
	ofn.nMaxFile=MAXFILENAME;
	ofn.lpstrFilter=_TEXT("dsn\0*.dsn\0");
	ofn.nFilterIndex=1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;
	ofn.Flags=OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt=(LPCWSTR)L"dsn";
	// Display the Open dialog box. 

	if(GetSaveFileName(&ofn))
	{
		hr=S_OK;
		File=szFile;
	}

	return hr;
}




//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr=-1;

	std::wstring szFile;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case ID_FILE_OPENFILE:
                hr=BasicFileOpen(hWnd,szFile);

				if(hr==S_OK)
				{	
					try
					{
						AddToLog(szFile);
						DSNFileIn=new DSNFile(szFile);
						AddToLog(L"Parse OK");
					}
					catch(int e)
					{
						switch(e)
						{
							case DSNExceptions::FILEOPENFAILED:
								cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
								break;

							case DSNExceptions::FORMATERROR:
								cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
								break;

							default:
								cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
								break;
						}
					}
				}

				RedrawWindow(hWnd,NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);

                break;
				

			case ID_FILE_SAVEFILE:
				try
				{
					DSNFileIn->FileOut(L"");
					AddToLog(L"Write OK");
				}
				catch(int e)
				{
					switch(e)
					{
						case DSNExceptions::FILEOPENFAILED:
							cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
							break;

						case DSNExceptions::FORMATERROR:
							cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
							break;

						default:
							cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
							break;
						}
					}


				RedrawWindow(hWnd,NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);

				break;
 
			case ID_FILE_SAVEAS:
				hr=BasicFileSave(hWnd,szFile);

				try
				{
					DSNFileIn->FileOut(szFile);
					AddToLog(L"Write OK "+szFile);
				}
				catch(int e)
				{
					switch(e)
					{
					case DSNExceptions::FILEOPENFAILED:
						cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
						break;

					case DSNExceptions::FORMATERROR:
						cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
						break;

					default:
						cout<<"An exception occurred. Exception Nr. "<<e<<'\n';
						break;
					}
				}


				RedrawWindow(hWnd,NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);

				break;
			
			case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);


			if(hdc)
			{ 
				for(int i=0; i<DListLen; i++)
				{
					RECT	rc;

					rc.top=i*20;
					rc.bottom=rc.top+20;
					rc.left=0;
					rc.right=1000;


					DrawText(hdc,DList[i]->c_str(),
						-1,
						&rc,
						DT_VCENTER|DT_LEFT
					);
				}
			}



            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		if(DSNFileIn)
		{
			delete DSNFileIn;
			DSNFileIn=0;
		}
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



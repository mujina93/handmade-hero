
#include <windows.h>

LRESULT // ptr to long (to return any kind of result)
CALLBACK // CALLBACK It's a calling convention - it describes the way function call is arranged on the low level 
MainWindowCallback(HWND Window,
		   UINT Message, // message codes https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-destroy
		   WPARAM WParam,
		   LPARAM LParam)
{
  LRESULT Result = 0;
  switch(Message)
    {
    case WM_SIZE: // when user changes size of window
      {
	OutputDebugStringA("WM_SIZE\n");
      } break;
    case WM_DESTROY: // window is destroyed
      {
	OutputDebugStringA("WM_DESTROY\n");
      } break;
    case WM_CLOSE: // signals a window or application sould terminate
      {
	OutputDebugStringA("WM_CLOSE\n");
      } break;
    case WM_ACTIVATEAPP: // a window belonging to a different application is about to be activated
      {
	OutputDebugStringA("WM_ACTIVATEAPP\n");
      } break;
    case WM_PAINT:
      {
	PAINTSTRUCT paint;
	HDC device_context = BeginPaint(Window, & paint);
	LONG x = paint.rcPaint.left;
	LONG y = paint.rcPaint.top;
	LONG height = paint.rcPaint.bottom - paint.rcPaint.top;
	LONG width = paint.rcPaint.right - paint.rcPaint.left;
	// epileptic window!
	static DWORD operation = WHITENESS;
	operation = (operation == WHITENESS)?BLACKNESS:WHITENESS;
	PatBlt(device_context, x, y, width, height, operation);
	EndPaint(Window, & paint);
      } break;
    default:
      {
	Result = DefWindowProc(Window, Message, WParam, LParam); // call default winwow procedure to provide processing for messages that were not handled (so that we are sure every message gets processed)
      } break;
    }

  return(Result);
}


int CALLBACK
WinMain(
  HINSTANCE Instance,
  HINSTANCE PrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd)
{
  // Day 2

  // Contains the window class attributes that are registered by the RegisterClass function.
  WNDCLASS WindowClass = {}; // initialize all struct to 0 (c++ syntax)
  WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW; // class styles https://docs.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
  WindowClass.lpfnWndProc = MainWindowCallback; // callback for processing messages
  WindowClass.hInstance = Instance; // A handle to the instance that contains the window procedure for the class
  WindowClass.lpszClassName = "HandmadeHeroWindowName";

  // Register window inside the if (returns 0 if registration error)
  if(RegisterClass(& WindowClass)) // Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function
  {
    // Create window
    HWND WindowHandle = CreateWindowExA(
		  0, //     dwExStyle.
		  WindowClass.lpszClassName, //    lpClassName,
		  "Handmade Hero", //    lpWindowName,
		  WS_OVERLAPPEDWINDOW|WS_VISIBLE, //     dwStyle,
		  CW_USEDEFAULT, //       X,
		  CW_USEDEFAULT, //       Y,
		  CW_USEDEFAULT, //       nWidth,
		  CW_USEDEFAULT, //       nHeight,
		  0, //      hWndParent, // we are a top level window
		  0, //     hMenu,
		  Instance, // hInstance,
		  0 //    lpParam
					);
    
    if(WindowHandle)
      {
	// message loop (windows does not provide one)
	for(;;)
	  {
	    MSG message;
	    // retrieves a message from this thread's message queue
	    BOOL message_result = GetMessage(& message, // lpMsg
			    nullptr, // retrieve messages from any window belonging to this thread  hWnd,
			    0, //  wMsgFilterMin,
			    0 //  wMsgFilterMax // get all messages. Don't filter
			);
	    if (message_result > 0)
	      {
		TranslateMessage(& message); // translates keyboard input to proper message
		DispatchMessage(& message);
	      }
	    else
	      {
		break;
	      }
	  }
      }
    else
      {
      }
  } else
  {
  }

  // Day 1 - simple windows window
  // MessageBox(
  // 	     0,
  // 	     "This is handmade hero",
  // 	     "Handmade hero",
  // 	     MB_OK|MB_ICONINFORMATION);
  return(0);
}


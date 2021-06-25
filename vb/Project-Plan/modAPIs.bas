Attribute VB_Name = "modAPIs"
Option Explicit

Private Declare Function timeGetTime Lib "winmm.dll" () As Long
Private Declare Function GetTickCount Lib "kernel32" () As Long

Private Declare Function QueryPerformanceCounter Lib "kernel32 " (lpPerformanceCount As Any) As Long
Private Declare Function QueryPerformanceFrequency Lib "kernel32 " (lpFrequency As Any) As Long

Private Declare Sub Sleep Lib "KERNEL32.dll" (ByVal dwMilliseconds As Long)

Private Declare Function LoadImage Lib "user32" Alias "LoadImageA" (ByVal hInst As Long, ByVal lpsz As String, ByVal dwImageType As Long, ByVal dwDesiredWidth As Long, ByVal dwDesiredHeight As Long, ByVal dwFlags As Long) As Long
Private Const LR_LOADFROMFILE = &H10
Private Const LR_LOADMAP3DCOLORS = &H1000
Private Const IMAGE_ICON = 1



Public Const MAX_TOOLTIP As Integer = 64
Private Const NIF_MESSAGE = &H1
Private Const NIF_ICON = &H2
Private Const NIF_TIP = &H4
Private Const NIF_STATE = &H8
Private Const NIF_INFO = &H10
Private Const NIM_ADD = &H0
Private Const NIM_MODIFY = &H1
Private Const NIM_DELETE = &H2
Private Const NIM_SETFOCUS = &H3
Private Const NIM_SETVERSION = &H4
Private Const NIM_VERSION = &H5
Private Const WM_USER As Long = &H400
Private Const NIN_BALLOONSHOW = (WM_USER + 2)
Private Const NIN_BALLOONHIDE = (WM_USER + 3)
Private Const NIN_BALLOONTIMEOUT = (WM_USER + 4)
Private Const NIN_BALLOONUSERCLICK = (WM_USER + 5)
Private Const NOTIFYICON_VERSION = 3
Private Const NIS_HIDDEN = &H1
Private Const NIS_SHAREDICON = &H2
Private Const WM_NOTIFY As Long = &H4E
Private Const WM_COMMAND As Long = &H111
Private Const WM_CLOSE As Long = &H10
Private Const WM_MOUSEMOVE As Long = &H200
Private Const WM_LBUTTONDOWN As Long = &H201
Private Const WM_LBUTTONUP As Long = &H202
Private Const WM_LBUTTONDBLCLK As Long = &H203
Private Const WM_MBUTTONDOWN As Long = &H207
Private Const WM_MBUTTONUP As Long = &H208
Private Const WM_MBUTTONDBLCLK As Long = &H209
Private Const WM_RBUTTONDOWN As Long = &H204
Private Const WM_RBUTTONUP As Long = &H205
Private Const WM_RBUTTONDBLCLK As Long = &H206
Public Enum bFlag
  NIIF_NONE = &H0
  NIIF_INFO = &H1
  NIIF_WARNING = &H2
  NIIF_ERROR = &H3
  NIIF_GUID = &H5
  NIIF_ICON_MASK = &HF
  NIIF_NOSOUND = &H10 '关闭提示音标志
End Enum
Public Const SW_RESTORE = 9
Public Const SW_HIDE = 0
Public nfIconData As NOTIFYICONDATA
Public Type NOTIFYICONDATA
  cbSize As Long
  Hwnd As Long
  uID As Long
  uFlags As Long
  uCallbackMessage As Long
  hIcon As Long
  szTip As String * 128
  dwState As Long
  dwStateMask As Long
  szInfo As String * 256
  uTimeoutAndVersion As Long
  szInfoTitle As String * 64
  dwInfoFlags As Long
End Type
Public Declare Function ShowWindow Lib "user32" (ByVal Hwnd As Long, ByVal nCmdShow As Long) As Long
Public Declare Function Shell_NotifyIcon Lib "shell32.dll" Alias "Shell_NotifyIconA" (ByVal dwMessage As Long, lpData As NOTIFYICONDATA) As Long
Public rc As Long


Public Declare Function Beep Lib "kernel32" (ByVal dwFreq As Long, ByVal dwDuration As Long) As Long

Public Declare Function sndPlaySound Lib "winmm.dll" Alias "sndPlaySoundA" _
(ByVal lpszSoundName As String, ByVal uFlags As Long) As Long
Public Declare Function PlaySound Lib "winmm.dll" Alias "PlaySoundA" (ByVal lpszName As String, ByVal hModule As Long, ByVal dwFlags As Long) As Long


Public Declare Function SetWindowPos Lib "user32" (ByVal Hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Public Const SWP_NOMOVE = &H2
Public Const SWP_NOSIZE = &H1
Public Const FLAG = SWP_NOMOVE Or SWP_NOSIZE
Public Const HWND_TOPMOST = -1
Public Const HWND_NOTOPMOST = -2
Public Const HWND_TOP = 0
Public Const HWND_BOTTOM = 1

Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" _
    (ByVal Hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
 
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" _
    (ByVal lpPrevWndFunc As Long, ByVal Hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
 
Public Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" (ByVal Hwnd As Long, ByVal nIndex As Long) As Long
 Public Const TRAY_CALLBACK = (&H400 + 1001&)
Public Const GWL_WNDPROC = -4
 
 
Public Const MIIM_STATE = &H1
Public Const MIIM_ID = &H2
Public Const MIIM_SUBMENU = &H4
Public Const MIIM_CHECKMARKS = &H8
Public Const MIIM_TYPE = &H10
Public Const MIIM_DATA = &H20
Public Const MFT_RADIOCHECK = &H200&
Public Type MENUITEMINFO
  cbSize As Long
  fMask As Long
  fType As Long
  fState As Long
  wID As Long
  hSubMenu As Long
  hbmpChecked As Long
  hbmpUnchecked As Long
  dwItemData As Long
  dwTypeData As String
  cch As Long
End Type

Public Declare Function SetMenuItemInfo Lib "user32" Alias "SetMenuItemInfoA" _
(ByVal hMenu As Long, ByVal uItem As Long, _
ByVal fByPosition As Long, lpmii As MENUITEMINFO) As Long

Public Declare Function GetMenu Lib "user32" _
(ByVal Hwnd As Long) As Long

Public Declare Function GetSubMenu Lib "user32" _
(ByVal hMenu As Long, _
ByVal nPos As Long) As Long
Public Declare Function SHFileExists Lib "shell32" Alias "#45" (ByVal szPath As String) As Long
Public Declare Function GetFileAttributes Lib "kernel32" Alias "GetFileAttributesA" (ByVal lpFileName As String) As Long
Public Declare Function SystemParametersInfo Lib "user32" Alias "SystemParametersInfoA" (ByVal uAction As Long, ByVal uParam As Long, ByRef lpvParam As Any, ByVal fuWinIni As Long) As Long
Public Const SPI_GETWORKAREA = 48
Public Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type
 
 
 
 
 
 
 
 
 

Dim glWinRet As Long
Dim OrgWinRet As Long
Dim MyForm_Bak As Form

Dim curFrequency As Currency
Dim curStart As Currency
Dim curEnd As Currency
Public Sub InitTimeCount()
  QueryPerformanceFrequency curFrequency
End Sub

Public Sub SaveLastTime()
  QueryPerformanceCounter curStart
End Sub

Public Function GetDifferenceTime() 'MS returned
  QueryPerformanceCounter curEnd
  GetDifferenceTime = (curEnd - curStart) / curFrequency * 1000
End Function

'----------Control the mouse on the tray !!!!!
Public Function CallbackMsgs(ByVal wHwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
  On Error Resume Next
    If wMsg = TRAY_CALLBACK Then
        With MyForm_Bak
            Select Case lParam
                Case WM_RBUTTONUP
                    .PopupMenu .trayMenu
                Case WM_LBUTTONUP
                    If .WindowState = vbMinimized Then
                      .Visible = True
                        .WindowState = vbNormal
                    Else
                        .WindowState = vbMinimized
                        .Visible = False
                    End If
            End Select
        End With
    End If
    CallbackMsgs = CallWindowProc(glWinRet, wHwnd, wMsg, wParam, lParam)
End Function

Public Sub AsyncSleep(lngInterval As Long)
   Dim lngEnd As Long, lngNow As Long, count1 As Long
   count1 = GetTickCount()
   lngEnd = count1 + (lngInterval * 1000)
   Do
     DoEvents
     lngNow = GetTickCount()
   Loop Until lngNow >= lngEnd
End Sub
Public Sub TrayAddIcon(ByVal MyForm As Form, ByVal MyIcon, ByVal ToolTip As String, Optional ByVal bFlag As bFlag)
'Add
  Set MyForm_Bak = MyForm
  OrgWinRet = GetWindowLong(MyForm.Hwnd, GWL_WNDPROC)
  With nfIconData
    .cbSize = Len(nfIconData)
    .Hwnd = MyForm.Hwnd
    .uID = vbNull
    .uFlags = NIF_ICON Or NIF_TIP Or NIF_MESSAGE
    .uCallbackMessage = TRAY_CALLBACK
    If TypeName(MyIcon) = "String" Then
      .hIcon = LoadImage(App.hInstance, MyIcon, IMAGE_ICON, 16, 16, LR_LOADFROMFILE Or LR_LOADMAP3DCOLORS)
    Else
      If InStr(1, "Picture,Image", TypeName(MyIcon), vbTextCompare) > 0 Then
        .hIcon = MyIcon
      End If
    End If
    .szTip = ToolTip & vbNullChar
  End With
  Call Shell_NotifyIcon(NIM_ADD, nfIconData)
  glWinRet = SetWindowLong(MyForm.Hwnd, GWL_WNDPROC, AddressOf CallbackMsgs)
End Sub
Public Sub TrayBalloon(ByVal sBalloonText As String, sBalloonTitle As String, Optional ByVal bFlag As bFlag)
'Balloon
  PlaySound TipSoundFilePath, 0, 1
  
  frmBalloon.Show
  frmBalloon.Caption = sBalloonTitle
  frmBalloon.lbl_msg.Caption = sBalloonText
  
  Dim lRes As Long
  Dim rectVal As RECT
  Dim TaskbarHeight As Integer
  lRes = SystemParametersInfo(SPI_GETWORKAREA, 0, rectVal, 0)
  TaskbarHeight = Screen.Height - rectVal.Bottom * Screen.TwipsPerPixelY
  frmBalloon.Move Screen.Width - frmBalloon.Width, Screen.Height - frmBalloon.Height - TaskbarHeight, frmBalloon.Width, frmBalloon.Height
  SetWindowPos frmBalloon.Hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE Or SWP_NOMOVE

Exit Sub

  With nfIconData
    .cbSize = Len(nfIconData)
    .Hwnd = MyForm_Bak.Hwnd
    .uID = vbNull
    .uFlags = NIF_INFO
    .dwInfoFlags = bFlag
    .szInfoTitle = sBalloonTitle & vbNullChar
    .szInfo = sBalloonText & vbNullChar
  End With
  Shell_NotifyIcon NIM_MODIFY, nfIconData
End Sub
Public Sub TrayRemoveIcon()
'Delete
  Shell_NotifyIcon NIM_DELETE, nfIconData
  Call SetWindowLong(MyForm_Bak.Hwnd, GWL_WNDPROC, OrgWinRet)
End Sub
Public Sub TrayTip(ByVal sTipText As String)
'Reset Tip
  With nfIconData
    .cbSize = Len(nfIconData)
    .Hwnd = MyForm_Bak.Hwnd
    .uID = vbNull
    .uFlags = NIF_ICON Or NIF_TIP Or NIF_MESSAGE
    .szTip = sTipText & vbNullChar
  End With
  Shell_NotifyIcon NIM_MODIFY, nfIconData
End Sub
Public Sub TrayIcon(MyIcon)
'Reset icon
  With nfIconData
    If TypeName(MyIcon) = "String" Then
      .hIcon = LoadImage(App.hInstance, MyIcon, IMAGE_ICON, 16, 16, LR_LOADFROMFILE Or LR_LOADMAP3DCOLORS)
    Else
      If TypeName(MyIcon) = "Picture" Then
        .hIcon = MyIcon
      End If
    End If
    .uFlags = NIF_ICON
  End With
  Shell_NotifyIcon NIM_MODIFY, nfIconData
End Sub



Public Function FileExists(Path$) As Boolean
  FileExists = Not CBool(GetFileAttributes(Path) = &HFFFFFFFF)
End Function


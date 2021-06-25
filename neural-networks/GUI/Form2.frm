VERSION 5.00
Begin VB.Form Form2 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Result"
   ClientHeight    =   3600
   ClientLeft      =   3255
   ClientTop       =   3405
   ClientWidth     =   4560
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3600
   ScaleWidth      =   4560
   Begin VB.CommandButton Command1 
      Caption         =   "Exit"
      BeginProperty Font 
         Name            =   "ËÎÌו"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   1560
      TabIndex        =   4
      Top             =   3000
      Width           =   1335
   End
   Begin VB.Timer Timer2 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   3960
      Top             =   360
   End
   Begin VB.Timer Timer1 
      Interval        =   250
      Left            =   4200
      Top             =   1200
   End
   Begin VB.TextBox Text1 
      Height          =   1215
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      TabIndex        =   2
      Top             =   1680
      Width           =   4335
   End
   Begin VB.Label Label3 
      Caption         =   "Output:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   1320
      Width           =   615
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "Executing..."
      BeginProperty Font 
         Name            =   "ËÎÌו"
         Size            =   18
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   840
      TabIndex        =   1
      Top             =   600
      Width           =   3015
   End
   Begin VB.Label Label1 
      Caption         =   "The number is:  "
      BeginProperty Font 
         Name            =   "ËÎÌו"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1815
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function GetLastError Lib "kernel32" () As Long
Private Declare Function CreateProcess Lib "kernel32" Alias "CreateProcessA" (ByVal lpApplicationName As String, ByVal lpCommandLine As String, lpProcessAttributes As SECURITY_ATTRIBUTES, lpThreadAttributes As SECURITY_ATTRIBUTES, ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, lpEnvironment As Any, ByVal lpCurrentDirectory As String, lpStartupInfo As STARTUPINFO, lpProcessInformation As PROCESS_INFORMATION) As Long
Private Declare Function CloseHandle Lib "kernel32.dll" (ByVal hObject As Long) As Long
Private Declare Function ReadFile Lib "kernel32" (ByVal hFile As Long, lpBuffer As Any, ByVal nNumberOfBytesToRead As Long, lpNumberOfBytesRead As Long, lpOverlapped As Long) As Long
Private Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function CreatePipe Lib "kernel32" (phReadPipe As Long, phWritePipe As Long, lpPipeAttributes As SECURITY_ATTRIBUTES, ByVal nSize As Long) As Long
Private Type STARTUPINFO
    cb                              As Long
    lpReserved                      As String
    lpDesktop                       As String
    lpTitle                         As String
    dwX                             As Long
    dwY                             As Long
    dwXSize                         As Long
    dwYSize                         As Long
    dwXCountChars                   As Long
    dwYCountChars                   As Long
    dwFillAttribute                 As Long
    dwFlags                         As Long
    wShowWindow                     As Integer
    cbReserved2                     As Integer
    lpReserved2                     As Long
    hStdInput                       As Long
    hStdOutput                      As Long
    hStdError                       As Long
End Type
Private Type PROCESS_INFORMATION
    hProcess                        As Long
    hThread                         As Long
    dwProcessId                     As Long
    dwThreadId                      As Long
End Type
Private Type SECURITY_ATTRIBUTES
    nLength                         As Long
    lpSecurityDescriptor            As Long
    bInheritHandle                  As Long
End Type
Private Const NORMAL_PRIORITY_CLASS  As Long = &H20&
Private Const STARTF_USESTDHANDLES   As Long = &H100&
Private Const STARTF_USESHOWWINDOW   As Long = &H1&
Private Const SW_HIDE                As Long = 0&
Private Const INFINITE               As Long = &HFFFF&



Private Sub Command1_Click()
  Unload Me
End Sub

Private Sub Timer1_Timer()
  Label2.Caption = Label2.Caption & "."
  If Label2.Caption = "Executing......" Then: Label2.Caption = "Executing"
  Timer2.Enabled = True
End Sub

Private Sub Timer2_Timer()
  Dim si As STARTUPINFO
  Dim pi As PROCESS_INFORMATION
  Dim retval As Long
  Dim hRead As Long
  Dim hWrite As Long
  Dim sBuffer(0 To 63) As Byte
  Dim lgSize As Long
  Dim sa As SECURITY_ATTRIBUTES
  Dim strResult As String

  With sa
    .nLength = Len(sa)
    .bInheritHandle = 1&
    .lpSecurityDescriptor = 0&
  End With

  retval = CreatePipe(hRead, hWrite, sa, 0&)
  If retval = 0 Then
    MsgBox "Error: FAILED to CREATE a PIPE ! (CreatePipe with " & Str(GetLastError()) & ")", vbCritical, "FAULT"
    Unload Me
  End If

  With si
    .cb = Len(si)
    .dwFlags = STARTF_USESTDHANDLES Or STARTF_USESHOWWINDOW
    .wShowWindow = SW_HIDE
    .hStdOutput = hWrite
  End With

  Dim commandline$
  commandline = App.Path & "\neural-networks.exe """ & App.Path & "\tmp.bmp"""

  'retval = CreateProcess(vbNullString, commandline & vbNullChar, sa, sa, 1&, NORMAL_PRIORITY_CLASS, ByVal 0&, vbNullString, si, pi)
  retval = CreateProcess(vbNullString, commandline & vbNullChar, sa, sa, 1&, NORMAL_PRIORITY_CLASS, ByVal 0&, App.Path & vbNullString, si, pi)
  If retval Then
    WaitForSingleObject pi.hProcess, INFINITE
    Do While ReadFile(hRead, sBuffer(0), 64, lgSize, ByVal 0&)
      strResult = strResult & StrConv(sBuffer(), vbUnicode)
      Erase sBuffer()
      If lgSize <> 64 Then Exit Do
      DoEvents
    Loop
    CloseHandle pi.hProcess
    CloseHandle pi.hThread
  Else
    MsgBox "Error: FAILED to CREATE a PROCESS ! (CreateProcess with " & Str(GetLastError()) & ")", vbCritical, "FAULT"
    Unload Me
  End If

  CloseHandle hRead
  CloseHandle hWrite
  
  strResult = Trim(strResult)
  
  Timer1.Enabled = False
  Timer2.Enabled = False
  Text1.Text = strResult
  Text1.SelStart = Len(Text1.Text)
  
  Dim pos&
  pos = InStr(1, strResult, "The number is: ")
  pos = pos + Len("The number is: ")
  Label2.Caption = Mid(strResult, pos, 1)
  
End Sub

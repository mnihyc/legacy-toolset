VERSION 5.00
Begin VB.UserControl clsColorfulButton 
   AutoRedraw      =   -1  'True
   BackColor       =   &H00FFFFFF&
   ClientHeight    =   945
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   2010
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   HasDC           =   0   'False
   ScaleHeight     =   63
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   134
   Begin VB.Timer Timer_MouseCheck 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1680
      Top             =   600
   End
   Begin VB.Image Image_Command 
      Height          =   375
      Left            =   240
      Stretch         =   -1  'True
      Top             =   120
      Width           =   615
   End
End
Attribute VB_Name = "clsColorfulButton"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event Click() 'Set user's custom function

Private Type POINTAPI
  X As Long
  Y As Long
End Type
Private Type RECT
  Left As Long
  Top As Long
  Right As Long
  Bottom As Long
End Type
Private Declare Function GetCursorPos Lib "user32" (ByRef lpPoint As POINTAPI) As Long
Private Declare Function GetWindowRect Lib "user32" (ByVal Hwnd As Long, ByRef lpRect As RECT) As Long


Dim Image_Normal As IPictureDisp, Image_Move As IPictureDisp, Image_Down As IPictureDisp



'Set an image to Image_Command
Private Sub SetImage(Img As IPictureDisp)
  Image_Command.Picture = Img
End Sub



Private Sub Image_Command_Click()
  RaiseEvent Click
End Sub

'Check if the mouse leave the control
Private Sub Timer_MouseCheck_Timer()
  Dim p As POINTAPI
  If GetCursorPos(p) = 0 Then: MsgBox "Failed in clsButton.Timer_MouseCheck_Timer at GetCursorPos() with a zero returned !", vbCritical, "Critical"
  Dim r As RECT
  If GetWindowRect(UserControl.Hwnd, r) = 0 Then: MsgBox "Failed in clsButton.Timer_MouseCheck_Timer at GetWindowRect() with a zero returned !", vbCritical, "Critical"
  If Not (p.X >= r.Left And p.X <= r.Right And p.Y >= r.Top And p.Y <= r.Bottom) Then
    Call SetImage(Image_Normal)
    Timer_MouseCheck.Enabled = False
  End If
End Sub

Private Sub UserControl_Initialize()
  UserControl.BackStyle = 0
  UserControl.AutoRedraw = True
  UserControl.ScaleMode = vbPixels
  Image_Command.Move 0, 0, UserControl.ScaleWidth, UserControl.ScaleHeight
  Set Image_Normal = Nothing
  Set Image_Move = Nothing
  Set Image_Down = Nothing
  Image_Command.Stretch = True
  Timer_MouseCheck.Enabled = False
End Sub




Private Sub Image_Command_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If Not (Button And vbLeftButton) > 0 Then
    Call SetImage(Image_Move) 'Event move
    Timer_MouseCheck.Enabled = True
  End If
End Sub

Private Sub Image_Command_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If (Button And vbLeftButton) > 0 Then
    Call SetImage(Image_Down) 'Event down
    Timer_MouseCheck.Enabled = True
  End If
End Sub

Private Sub Image_Command_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
  Call SetImage(Image_Normal) 'Event up
End Sub






Private Sub UserControl_Resize()
  Image_Command.Move 0, 0, UserControl.ScaleWidth, UserControl.ScaleHeight
End Sub


Public Property Get ImageNormal() As IPictureDisp
  Set ImageNormal = Image_Normal
End Property

Public Property Set ImageNormal(ByVal vNewValue As IPictureDisp)
  Set Image_Normal = vNewValue
  PropertyChanged ("ImageNormal")
  Call SetImage(Image_Normal)
End Property

Public Sub ImageNormalClear()
  Set Image_Normal = Nothing
End Sub

Public Property Get ImageMove() As IPictureDisp
  Set ImageMove = Image_Move
End Property

Public Property Set ImageMove(ByVal vNewValue As IPictureDisp)
  Set Image_Move = vNewValue
  PropertyChanged ("ImageMove")
End Property

Public Sub ImageMoveClear()
  Set Image_Move = Nothing
End Sub

Public Property Get ImageDown() As IPictureDisp
  Set ImageDown = Image_Down
End Property

Public Property Set ImageDown(ByVal vNewValue As IPictureDisp)
  Set Image_Down = vNewValue
  PropertyChanged ("ImageDown")
End Property

Public Sub ImageDownClear()
  Set Image_Down = Nothing
End Sub








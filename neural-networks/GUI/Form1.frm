VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "ComDlg32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "GUI"
   ClientHeight    =   5325
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   4725
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   5325
   ScaleWidth      =   4725
   StartUpPosition =   3  '窗口缺省
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   480
      Top             =   3480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
   End
   Begin VB.CommandButton Command5 
      Caption         =   "LoadFile"
      Height          =   330
      Left            =   840
      TabIndex        =   14
      Top             =   120
      Width           =   1095
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Unmake"
      Height          =   255
      Left            =   2760
      TabIndex        =   13
      Top             =   2880
      Visible         =   0   'False
      Width           =   735
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Left            =   120
      Max             =   6
      Min             =   1
      TabIndex        =   12
      Top             =   3120
      Value           =   6
      Width           =   2655
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Optimized"
      Height          =   180
      Left            =   0
      TabIndex        =   10
      Top             =   4800
      Width           =   1215
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Origional"
      Height          =   180
      Left            =   0
      TabIndex        =   9
      Top             =   4560
      Value           =   -1  'True
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   3360
      MaxLength       =   1
      TabIndex        =   6
      Text            =   "0"
      Top             =   120
      Width           =   495
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Load"
      Height          =   255
      Left            =   3960
      TabIndex        =   4
      Top             =   120
      Width           =   615
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Start"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   15
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   1440
      TabIndex        =   3
      Top             =   4320
      Width           =   2175
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Clear"
      Height          =   495
      Left            =   3240
      TabIndex        =   2
      Top             =   2400
      Width           =   1095
   End
   Begin VB.PictureBox Picture2 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FFFFFF&
      Height          =   480
      Left            =   1200
      ScaleHeight     =   28
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   28
      TabIndex        =   1
      Top             =   3480
      Width           =   480
   End
   Begin VB.PictureBox Picture1 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FFFFFF&
      Height          =   2580
      Left            =   165
      ScaleHeight     =   168
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   168
      TabIndex        =   0
      Top             =   480
      Width           =   2580
   End
   Begin VB.Label Label4 
      Caption         =   "Database:"
      Height          =   255
      Left            =   0
      TabIndex        =   11
      Top             =   4200
      Width           =   975
   End
   Begin VB.Label Label3 
      Caption         =   "Result <-------Notice:        Write in the middle with a nice size!"
      Height          =   615
      Left            =   2040
      TabIndex        =   8
      Top             =   3480
      Width           =   2415
   End
   Begin VB.Label Label2 
      Caption         =   $"Form1.frx":0000
      Height          =   1695
      Left            =   3000
      TabIndex        =   7
      Top             =   600
      Width           =   1695
   End
   Begin VB.Label Label1 
      Caption         =   "Sample:"
      Height          =   255
      Left            =   2520
      TabIndex        =   5
      Top             =   120
      Width           =   735
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function StretchBlt Lib "gdi32" _
( _
ByVal hdc As Long, _
ByVal X As Long, _
ByVal Y As Long, _
ByVal nWidth As Long, _
ByVal nHeight As Long, _
ByVal hSrcDC As Long, _
ByVal xSrc As Long, _
ByVal ySrc As Long, _
ByVal nSrcWidth As Long, _
ByVal nSrcHeight As Long, _
ByVal dwRop As Long _
) As Long

Private Declare Function DrawFocusRect Lib "user32" _
(ByVal hdc As Long, lpRect As RECT) As Long
Private Type RECT
  X1 As Long
  Y1 As Long
  X2 As Long
  Y2 As Long
End Type
Dim lpRect As RECT

Dim isnowCapturing As Boolean


Private Sub LitUp(X As Single, Y As Single, Optional color As Long = 0)
  X = Abs(X): Y = Abs(Y)
  Picture1.PSet (X, Y), color
End Sub

Private Function GetRValue&(ByVal rgbColor&)
    GetRValue = rgbColor And &HFF
End Function
  
Private Function GetGValue&(ByVal rgbColor&)
    GetGValue = (rgbColor And &HFF00&) / &HFF&
End Function
  
Private Function GetBValue&(ByVal rgbColor&)
    GetBValue = (rgbColor& And &HFF0000) / &HFF00&
End Function

Private Sub ChangePicGray(ByRef pic As PictureBox, ByVal nx&, ByVal ny&, Optional ByVal nMaskColor& = -1)
    Dim rgbColor&, Gray&
    Dim RValue&, GValue&, BValue&
    Dim dl&
    rgbColor = pic.Point(nx, ny)
  
    If rgbColor = nMaskColor Then GoTo Release
    RValue = GetRValue(rgbColor)
    GValue = GetGValue(rgbColor)
    BValue = GetBValue(rgbColor)
    Gray = (9798 * RValue + 19235 * GValue + 3735 * BValue) / 32768
    rgbColor = RGB(Gray, Gray, Gray)
    pic.PSet (nx, ny), rgbColor
Release:
    rgbColor = 0
    Gray = 0
    RValue = 0
    GValue = 0
    BValue = 0
    dl = 0
End Sub

Private Sub Command1_Click()
  Set Picture1.Picture = Nothing
  With lpRect
    .X1 = -1
    .X2 = 0
    .Y1 = 0
    .Y2 = 0
  End With
  Picture1.Cls
  Picture2.Cls
  Command4.Visible = False
End Sub

Private Sub Command2_Click()
  Command2.Caption = "Saving..."
  On Error Resume Next
  Kill App.Path & "\tmp.bmp"
  On Error GoTo 0
  SavePicture Picture2.Image, App.Path & "\tmp.bmp"
  'SavePicture Picture1.Image, App.Path & "\t.bmp"
  Command2.Caption = "Executing..."
  Form2.Show vbModal
  Command2.Caption = "Start"
End Sub

Private Function GetColor(ByVal i As Long, ByVal j As Long) As Long
  Dim dig As Integer
  dig = (Abs(i) + 2) * (Abs(j) + 2) * 2 'Int(Rnd * ((Abs(i) + Abs(j)) * 50))
  If Abs(i) <= 0 And Abs(j) <= 0 Then: dig = RGB(0, 0, 0)
  
  GetColor = RGB(Abs(dig), Abs(dig), Abs(dig))
End Function

Private Sub ResetColor()
  Dim X, Y As Integer
  For X = 0 To Picture2.ScaleWidth - 1
    For Y = 0 To Picture2.ScaleHeight - 1
    
    
    
    Next Y
  Next X
  
End Sub

Private Sub Command3_Click()
  On Error GoTo err
  Dim s$
  's = App.Path & "\samples\" & Text1.Text & ".bmp"
  'Picture1.Picture = LoadPicture(s)
  s = "10" & Text1.Text
  'Picture1.Picture = LoadResPicture(Val(s), vbResBitmap)
  '简直要上天，垃圾360这里竟然给我报毒
  
  
  With lpRect
    .X1 = -1
    .X2 = 0
    .Y1 = 0
    .Y2 = 0
  End With
  Command4.Visible = False
  
  Dim a&
  a = Val(s)
  Picture1.Picture = LoadResPicture(a, vbResBitmap)
  
  
  StretchBlt Picture2.hdc, 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight, Picture1.hdc, 0, 0, Picture1.ScaleWidth, Picture1.ScaleHeight, vbNotSrcCopy
  Picture2.Refresh
  Exit Sub
err:
  MsgBox "Error : Couldn't load """ & s & """", vbExclamation, "ERROR"
End Sub

Private Sub Command4_Click()
  Command4.Visible = False
  Call DrawFocusRect(Picture1.hdc, lpRect)
  With lpRect
    .X1 = -1
    .X2 = 0
    .Y1 = 0
    .Y2 = 0
  End With
  Picture1.Refresh
  StretchBlt Picture2.hdc, 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight, Picture1.hdc, 0, 0, Picture1.ScaleWidth, Picture1.ScaleHeight, vbNotSrcCopy
  Picture2.Refresh
End Sub

Private Sub Command5_Click()
  With lpRect
    .X1 = -1
    .X2 = 0
    .Y1 = 0
    .Y2 = 0
  End With
  Command4.Visible = False
  
  On Error GoTo errHandler
  CommonDialog1.ShowOpen
  Picture1.Picture = LoadPicture(CommonDialog1.FileName)
  'Picture1.Refresh
  StretchBlt Picture2.hdc, 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight, Picture1.hdc, 0, 0, Picture1.ScaleWidth, Picture1.ScaleHeight, vbNotSrcCopy
  Picture2.Refresh
  Exit Sub
errHandler:
  
End Sub

Private Sub Form_Load()
  Form3.Show vbModal
  Command3.ToolTipText = "Load a sample which I saved from the file." & vbCrLf & "You may take it as a reference."
  Text1.ToolTipText = "Input a number which is from 0 to 9."
  Picture1.ToolTipText = "Hand-writing screen." & vbCrLf & "Left-click for writing and right-click for capturing." & vbCrLf & "Notice: hold the click on when using."
  Picture2.ToolTipText = "The result picture which will be sent to the final program."
  Label4.ToolTipText = "Choose which database to use." & vbCrLf & "Different databases may get different results."
  Option1.ToolTipText = "Origional database." & vbCrLf & "Use the origional MNIST training data."
  Option2.ToolTipText = "Optimized database." & vbCrLf & "Use the optimized training data for especially this program.(A short training time)"
  Command2.ToolTipText = "Go on to find what this number is."
  Command1.ToolTipText = "Clear what you wrote or edited"
  Command4.ToolTipText = "Unmake the rectangle you drew"
  Command5.ToolTipText = "Load a picture from file"
  
  lpRect.X1 = -1
  CommonDialog1.CancelError = True
  CommonDialog1.Flags = cdlOFNHideReadOnly
  CommonDialog1.Filter = "BMP Pictures(*.bmp)|*.bmp"
  CommonDialog1.FilterIndex = 0
  
  On Error Resume Next
  Kill App.Path & "\tmp.bmp"
  DoEvents
  On Error GoTo 0
  
  OutputResBin 1
  
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
  On Error Resume Next
  Kill App.Path & "\tmp.bmp"
  DoEvents
  
  End
End Sub

Private Sub OutputResBin(num&)
  Dim Datas() As Byte
  Dim FileNum As Long
  
  Datas = LoadResData(Val("10" + Str(num)), "CUSTOM")
  FileNum = FreeFile
  Open App.Path & "\svf.bin" For Binary As #FileNum
  Put #1, , Datas
  Close #FileNum
  
  
End Sub

Private Sub HScroll1_Change()
  Dim cw, ch As Single
  cw = 2580 / 6 * HScroll1.Value
  ch = 2580 / 6 * HScroll1.Value
  Picture1.Move 165 + (2580 - cw) / 2, 480 + (2580 - ch) / 2, cw, ch
  Picture1.SetFocus
End Sub

Private Sub Option1_Click()
  OutputResBin 1
End Sub

Private Sub Option2_Click()
  OutputResBin 2
End Sub

Private Function gMax&(a&, b&)
  gMax = a
  If b > a Then: gMax = b
End Function

Private Sub gSwap(ByRef a&, ByRef b&)
  Dim c&
  c = a
  a = b
  b = c
End Sub

Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If Button = vbRightButton And Not isnowCapturing And lpRect.X1 = -1 Then
     isnowCapturing = True
     lpRect.X1 = X
     lpRect.Y1 = Y
  End If
End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If Button = vbLeftButton Then
    
    Randomize
    
    Dim i, j As Long
    For i = -Int(Rnd * gMax(HScroll1.Value, 2)) To Int(Rnd * gMax(HScroll1.Value, 2))
      For j = -Int(Rnd * gMax(HScroll1.Value, 2)) To Int(Rnd * gMax(HScroll1.Value, 2))
        Dim nx, ny As Long
        nx = X: ny = Y
        
        LitUp X - i, Y - j, GetColor(i, j)
        LitUp X - i, Y + j, GetColor(i, j)
        LitUp X + i, Y - j, GetColor(i, j)
        LitUp X + i, Y + j, GetColor(i, j)
        
      Next j
    Next i
    
  End If
  If Button = vbRightButton And isnowCapturing Then
    With lpRect
      If .X2 <> 0 Then: Call DrawFocusRect(Picture1.hdc, lpRect)
      .X2 = X
      .Y2 = Y
      If .X2 < .X1 Then: gSwap .X1, .X2
      If .Y2 < .Y1 Then: gSwap .Y1, .Y2
      
      Call DrawFocusRect(Picture1.hdc, lpRect)
      Picture1.Refresh
      
      'StretchBlt Picture2.hdc, 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight, Picture1.hdc, .X1 \ Screen.TwipsPerPixelX, .Y1 \ Screen.TwipsPerPixelY, .X2 \ Screen.TwipsPerPixelX, .Y2 \ Screen.TwipsPerPixelY, vbNotSrcCopy
      Call DrawFocusRect(Picture1.hdc, lpRect)
      StretchBlt Picture2.hdc, 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight, Picture1.hdc, .X1, .Y1, .X2 - .X1, .Y2 - .Y1, vbNotSrcCopy
      Call DrawFocusRect(Picture1.hdc, lpRect)
    End With
    Picture2.Refresh
  End If
End Sub

Private Sub Picture1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If Button = vbRightButton Then
    isnowCapturing = False
    Command4.Visible = True
    Exit Sub
  End If
  StretchBlt Picture2.hdc, 0, 0, Picture2.ScaleWidth, Picture2.ScaleHeight, Picture1.hdc, 0, 0, Picture1.ScaleWidth, Picture1.ScaleHeight, vbNotSrcCopy
  Picture2.Refresh
  'ResetColor
  'Picture2.Refresh
End Sub

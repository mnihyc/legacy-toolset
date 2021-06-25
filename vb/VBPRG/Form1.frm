VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "ComDlg32.OCX"
Begin VB.Form Form1 
   Caption         =   "文件加密"
   ClientHeight    =   4275
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4245
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   4275
   ScaleWidth      =   4245
   StartUpPosition =   3  '窗口缺省
   Begin MSComDlg.CommonDialog openf 
      Left            =   3720
      Top             =   3600
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.ComboBox slt 
      Height          =   300
      ItemData        =   "Form1.frx":0000
      Left            =   2160
      List            =   "Form1.frx":000A
      Style           =   2  'Dropdown List
      TabIndex        =   6
      Top             =   240
      Width           =   1455
   End
   Begin VB.CommandButton comd 
      Caption         =   "开始"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   1200
      TabIndex        =   5
      Top             =   3480
      Width           =   1575
   End
   Begin VB.CommandButton seld 
      Caption         =   "保存"
      Height          =   375
      Left            =   2880
      TabIndex        =   4
      Top             =   2280
      Width           =   735
   End
   Begin VB.CommandButton self 
      Caption         =   "打开"
      Height          =   375
      Left            =   2880
      TabIndex        =   3
      Top             =   1200
      Width           =   735
   End
   Begin VB.TextBox psw 
      Height          =   375
      IMEMode         =   3  'DISABLE
      Left            =   2160
      MaxLength       =   20
      PasswordChar    =   "*"
      TabIndex        =   0
      Top             =   720
      Width           =   1935
   End
   Begin VB.Label Label4 
      Caption         =   "保存的文件:"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   600
      TabIndex        =   10
      Top             =   2280
      Width           =   1455
   End
   Begin VB.Label Label3 
      Caption         =   "打开的文件:"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   600
      TabIndex        =   9
      Top             =   1200
      Width           =   1455
   End
   Begin VB.Label Label2 
      Caption         =   "在这里输入密码:"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   720
      Width           =   1935
   End
   Begin VB.Label Label1 
      Caption         =   "选择需要的操作:"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   240
      Width           =   1935
   End
   Begin VB.Label swd 
      Height          =   735
      Left            =   0
      TabIndex        =   2
      Top             =   2640
      Width           =   4215
   End
   Begin VB.Label sws 
      Height          =   615
      Left            =   0
      TabIndex        =   1
      Top             =   1560
      Width           =   4215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Text:psw Lable:sws,swd Command:self,sels,comd
'ComboBox:slt CommonDialog:openf
Dim lastslt As Long
Private Sub Form1_Load()
Me.Show
lastslt = -1
openf.Filter = "All Files (*.*)|*.*"
openf.FileName = ""
End Sub


Private Sub self_Click()
openf.ShowOpen
Dim fs
Set fs = CreateObject("scripting.filesystemobject")
If fs.fileexists(openf.FileName) Then
sws.Caption = openf.FileName
swd.Caption = openf.FileName
Select Case slt.ListIndex
Case -1
swd.Caption = swd.Caption & ".xmd"
Case 0
swd.Caption = swd.Caption & ".enc"
Case 1
If Right(sws.Caption, 4) = ".enc" And Left(Right(sws.Caption, 8), 1) = "." Then
swd.Caption = Left(sws.Caption, Len(sws.Caption) - 4)
Else
swd.Caption = swd.Caption & ".dec"
End If
End Select
Else
sws.Caption = "文件未找到"
End If
End Sub
Private Sub seld_Click()
openf.ShowSave
swd.Caption = openf.FileName
End Sub
Private Sub slt_Click()
If Right(swd.Caption, 4) = ".xmd" Or Right(swd.Caption, 4) = ".enc" Or Right(swd.Caption, 4) = ".dec" Then
Select Case slt.ListIndex
Case -1
swd.Caption = Left(swd.Caption, Len(swd.Caption) - 4) & ".xmd"
Case 0
swd.Caption = Left(swd.Caption, Len(swd.Caption) - 4) & ".enc"
Case 1
If Right(sws.Caption, 4) = ".enc" And Left(Right(sws.Caption, 8), 1) = "." Then
swd.Caption = Left(sws.Caption, Len(sws.Caption) - 4)
Else
swd.Caption = Left(swd.Caption, Len(swd.Caption) - 4) & ".dec"
End If
End Select
End If
If lastslt <> slt.ListIndex Then
lastslt = slt.ListIndex
psw.Text = ""
End If
End Sub
Private Sub comd_Click()
If sws.Caption = "文件未找到" Or sws.Caption = "" Or swd.Caption = "" Then
MsgBox "没有设置好文件", vbExclamation, ""
Exit Sub
End If
If psw.Text = "" Then
If slt.ListIndex = -1 Or slt.ListIndex = 0 Then: MsgBox "没有设置好密码", vbExclamation, ""
If slt.ListIndex = 1 Then: MsgBox "密码为空", vbExclamation, ""
Exit Sub
End If
psw.Enabled = False
comd.Enabled = False
slt.Enabled = False
self.Enabled = False
seld.Enabled = False
Dim i As Long, j As Long
Dim ndn As Long
Dim fs, selec As Long
Set fs = CreateObject("scripting.filesystemobject")
Dim ydt() As Byte, edt() As Byte
Open sws.Caption For Binary As #1
If slt.ListIndex = 0 Then
If fs.fileexists(swd.Caption) Then
selec = MsgBox("文件已存在,是否覆盖", vbYesNo Or vbInformation, "")
If selec = vbNo Then
Close #1
psw.Enabled = True
comd.Enabled = True
slt.Enabled = True
self.Enabled = True
seld.Enabled = True
Exit Sub
End If
Kill swd.Caption
End If
ReDim ydt(LOF(1) - 1)
ReDim edt(LOF(1) + Len(psw.Text) + 2)
Get #1, , ydt()
Randomize
ndn = Int(Rnd * (9 - 1 + 1)) + 1
edt(0) = ndn + 15
If Len(psw.Text) < 10 Then
edt(1) = 0
Else
edt(1) = Len(psw.Text) \ 10 + Asc("L")
End If
edt(2) = Len(psw.Text) Mod 10 + Asc("f")
For i = 1 To Len(psw.Text)
edt(i + 2) = (Asc(Mid(psw.Text, i, 1)) Xor ndn) + i
Next i
j = 0
For i = 1 To LOF(1)
If j < Len(psw.Text) Then
j = j + 1
Else
j = 1
End If
edt(i + Len(psw.Text) + 2) = ydt(i - 1) Xor Asc(Mid(psw.Text, j, 1))
Next i
End If

If slt.ListIndex = 1 Then
ReDim ydt(LOF(1) - 1)
Get #1, , ydt()
ndn = ydt(0) - 15
Dim pswl As Long
If ydt(1) <> 0 Then: pswl = (ydt(1) - Asc("L")) * 10 + ydt(2) - Asc("f")
If ydt(1) = 0 Then: pswl = ydt(2) - Asc("f")
Dim Npsw As String
Npsw = ""
For i = 1 To pswl
Npsw = Npsw + Chr((ydt(i + 2) - i) Xor ndn)
Next i
If Npsw <> psw.Text Then
MsgBox "密码错误", vbCritical, ""
Close #1
psw.Enabled = True
comd.Enabled = True
slt.Enabled = True
self.Enabled = True
seld.Enabled = True
Exit Sub
End If
If fs.fileexists(swd.Caption) Then
selec = MsgBox("文件已存在,是否覆盖", vbYesNo Or vbInformation, "")
If selec = vbNo Then
Close #1
psw.Enabled = True
comd.Enabled = True
slt.Enabled = True
self.Enabled = True
seld.Enabled = True
Exit Sub
End If
Kill swd.Caption
End If
ReDim edt(LOF(1) - Len(psw.Text) - 4)
j = 0
For i = 1 To LOF(1) - Len(psw.Text) - 3
If j < Len(psw.Text) Then
j = j + 1
Else
j = 1
End If
edt(i - 1) = ydt(i + 2 + Len(psw.Text)) Xor Asc(Mid(psw.Text, j, 1))
Next i
End If
Close #1
Open swd.Caption For Binary As #2
Put #2, , edt()
Close #2
MsgBox "成功", vbInformation, ""
psw.Enabled = True
comd.Enabled = True
slt.Enabled = True
self.Enabled = True
seld.Enabled = True
End Sub


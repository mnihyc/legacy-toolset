VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Begin VB.Form frmDateAdd 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "添加计划"
   ClientHeight    =   5160
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   4305
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5160
   ScaleWidth      =   4305
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox Text3 
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   120
      TabIndex        =   10
      Text            =   "Null"
      Top             =   3840
      Width           =   3975
   End
   Begin VB.CommandButton Command2 
      Caption         =   "取消"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   2280
      TabIndex        =   8
      Top             =   4320
      Width           =   1695
   End
   Begin VB.CommandButton Command1 
      Caption         =   "确认"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   480
      TabIndex        =   7
      Top             =   4320
      Width           =   1695
   End
   Begin VB.TextBox Text2 
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1095
      Left            =   960
      MaxLength       =   50
      MultiLine       =   -1  'True
      TabIndex        =   6
      Top             =   2160
      Width           =   3255
   End
   Begin VB.TextBox Text1 
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   960
      MaxLength       =   10
      TabIndex        =   4
      Top             =   1320
      Width           =   3135
   End
   Begin MSComCtl2.DTPicker DTTime 
      Height          =   495
      Left            =   1920
      TabIndex        =   2
      Top             =   720
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   873
      _Version        =   393216
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Format          =   166658050
      UpDown          =   -1  'True
      CurrentDate     =   36494
   End
   Begin MSComCtl2.DTPicker DTDate 
      Height          =   495
      Left            =   1920
      TabIndex        =   0
      Top             =   120
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   873
      _Version        =   393216
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Format          =   166658049
      UpDown          =   -1  'True
      CurrentDate     =   42841
   End
   Begin VB.Label Label3 
      Caption         =   "执行的程序:(如shutdown -s -t 0) 无留空或Null"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   120
      TabIndex        =   9
      Top             =   3360
      Width           =   3975
   End
   Begin VB.Label Label2 
      Caption         =   "内容："
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   1
      Left            =   120
      TabIndex        =   5
      Top             =   2640
      Width           =   855
   End
   Begin VB.Label Label2 
      Caption         =   "标题："
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   0
      Left            =   240
      TabIndex        =   3
      Top             =   1320
      Width           =   855
   End
   Begin VB.Label Label1 
      Caption         =   "计划时间："
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   360
      TabIndex        =   1
      Top             =   240
      Width           =   1335
   End
End
Attribute VB_Name = "frmDateAdd"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub Command1_Click()
  If Trim(Text1.Text) = "" Or Trim(Text2.Text) = "" Then
    MsgBox "信息不能为空！", vbInformation, "提示"
    Exit Sub
  End If
  If Trim(Text3.Text) = "" Then: Text3.Text = "Null"
  Text3.Text = Replace(Text3.Text, " ", SPACESavedStr)
  On Error Resume Next
  Dim i&, Index&
  Index = 0
  Dim DTDateValue$, DTTimeValue$, Text1Text$, Text2Text$
  Text1Text = Replace(Text1.Text, " ", SPACESavedStr)
  Text2Text = Replace(Text2.Text, " ", SPACESavedStr)
  Text2Text = Replace(Text2Text, vbCrLf, CRLFSavedStr)
  DTDateValue = Replace(DTDate.Value, "/", "-")
  DTTimeValue = DTTime.Value
  SetLocalDate DTDateValue: SetLocalTime DTTimeValue
  If CallDateAddType = 1 Then 'Delete and Re-add ,prevent from
    Dim p&
    For p = CallDateAddIndex + 1 To UBound(RCData)
      RCData(p - 1) = RCData(p)
    Next p
    If UBound(RCData) - 1 <> 0 Then
      ReDim Preserve RCData(1 To UBound(RCData) - 1)
    Else
      SetRCDataNull
    End If
  End If
  If IsRCDataNull Then
    With RCData(1)
    .Date = DTDateValue
    .Time = DTTimeValue
    .Title = Text1Text
    .Detail = Text2Text
    .ToDo = Text3.Text
    End With
   GoTo EndCheck
  End If
  For i = UBound(RCData) To 1 Step -1
    If (SplitYMD(DTDateValue, 1) > SplitYMD(RCData(i).Date, 1)) Or _
    (SplitYMD(DTDateValue, 1) = SplitYMD(RCData(i).Date, 1) And SplitYMD(DTDateValue, 2) > SplitYMD(RCData(i).Date, 2)) Or _
    (SplitYMD(DTDateValue, 1) = SplitYMD(RCData(i).Date, 1) And SplitYMD(DTDateValue, 2) = SplitYMD(RCData(i).Date, 2) And SplitYMD(DTDateValue, 3) > SplitYMD(RCData(i).Date, 3)) Or _
    (SplitYMD(DTDateValue, 1) = SplitYMD(RCData(i).Date, 1) And SplitYMD(DTDateValue, 2) = SplitYMD(RCData(i).Date, 2) And SplitYMD(DTDateValue, 3) = SplitYMD(RCData(i).Date, 3) And SplitHMS(DTTimeValue, 1) > SplitHMS(RCData(i).Time, 1)) Or _
    (SplitYMD(DTDateValue, 1) = SplitYMD(RCData(i).Date, 1) And SplitYMD(DTDateValue, 2) = SplitYMD(RCData(i).Date, 2) And SplitYMD(DTDateValue, 3) = SplitYMD(RCData(i).Date, 3) And SplitHMS(DTTimeValue, 1) = SplitHMS(RCData(i).Time, 1) And SplitHMS(DTTimeValue, 2) > SplitHMS(RCData(i).Time, 2)) Or _
    (SplitYMD(DTDateValue, 1) = SplitYMD(RCData(i).Date, 1) And SplitYMD(DTDateValue, 2) = SplitYMD(RCData(i).Date, 2) And SplitYMD(DTDateValue, 3) = SplitYMD(RCData(i).Date, 3) And SplitHMS(DTTimeValue, 1) = SplitHMS(RCData(i).Time, 1) And SplitHMS(DTTimeValue, 2) = SplitHMS(RCData(i).Time, 2) And SplitHMS(DTTimeValue, 3) > SplitHMS(RCData(i).Time, 3)) Then
      Index = i
      Exit For
    End If
  Next i
  ReDim Preserve RCData(1 To UBound(RCData) + 1)
  Dim k&
  For k = UBound(RCData) To Index + 2 Step -1
    RCData(k) = RCData(k - 1)
  Next k
  With RCData(Index + 1)
    .Date = DTDateValue
    .Time = DTTimeValue
    .Title = Text1Text
    .Detail = Text2Text
    .ToDo = Text3.Text
  End With
EndCheck:
  RewriteRCDataFile
  If Not CheckIfErr() Then
    MsgBox "成功！", vbInformation, "!"
  Else
    MsgBox "失败！", vbExclamation, "!!!"
  End If
  Call frmStart.FindRCToPaint
  Unload Me
End Sub

Private Sub Command2_Click()
  Unload Me
End Sub

Private Sub Form_Load()
  DTDate.Value = Format$(Now, "yyyy/mm/dd")
  DTTime.Value = Format$(Now, "hh:mm:ss")
  If CallDateAddType = 1 Then
    'Was sure that this form must be UNLOADED when it disapperaed
    Me.Caption = "修改计划"
    DTDate.Value = Replace(CallDateAddRC.Date, "-", "/")
    DTTime.Value = CallDateAddRC.Time
    Text1.Text = Replace(CallDateAddRC.Title, SPACESavedStr, " ")
    Text2.Text = Replace(Replace(CallDateAddRC.Detail, CRLFSavedStr, vbCrLf), SPACESavedStr, " ")
    Text3.Text = Replace(CallDateAddRC.ToDo, SPACESavedStr, " ")
  End If
End Sub

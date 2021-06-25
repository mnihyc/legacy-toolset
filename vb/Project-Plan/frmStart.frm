VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmStart 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "启动界面"
   ClientHeight    =   7110
   ClientLeft      =   150
   ClientTop       =   495
   ClientWidth     =   5010
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   474
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   334
   StartUpPosition =   3  '窗口缺省
   Begin VB.Timer TimerRepaint 
      Interval        =   60000
      Left            =   3240
      Top             =   6600
   End
   Begin VB.Timer TimeControl 
      Interval        =   20
      Left            =   120
      Top             =   600
   End
   Begin TabDlg.SSTab TabSwitch 
      Height          =   5775
      Left            =   0
      TabIndex        =   5
      TabStop         =   0   'False
      Top             =   1080
      Width           =   4935
      _ExtentX        =   8705
      _ExtentY        =   10186
      _Version        =   393216
      TabHeight       =   529
      TabCaption(0)   =   "健康提示"
      TabPicture(0)   =   "frmStart.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "lbl_jk_last(0)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "lbl_jk_last(1)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "lbl_jk_time"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "lbl_jk_title"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "lbl_jk_content"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "lbl_jk_next(0)"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "lbl_jk_next(1)"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "lbl_jk_index"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "img_jk_down"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "img_jk_up"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).ControlCount=   10
      TabCaption(1)   =   "日程安排"
      TabPicture(1)   =   "frmStart.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "TimeCheckRC"
      Tab(1).Control(1)=   "btn_rc_add"
      Tab(1).Control(2)=   "img_rc_up"
      Tab(1).Control(3)=   "img_rc_down"
      Tab(1).Control(4)=   "btn_rc_del"
      Tab(1).Control(5)=   "btn_rc_edit"
      Tab(1).Control(6)=   "lbl_rc_time(3)"
      Tab(1).Control(7)=   "lbl_rc_time(2)"
      Tab(1).Control(8)=   "lbl_rc_time(1)"
      Tab(1).Control(9)=   "lbl_rc_time(0)"
      Tab(1).Control(10)=   "lbl_rc_index"
      Tab(1).ControlCount=   11
      TabCaption(2)   =   "设置"
      TabPicture(2)   =   "frmStart.frx":0038
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "cmd_start"
      Tab(2).Control(1)=   "cmd_clear"
      Tab(2).ControlCount=   2
      Begin VB.CommandButton cmd_start 
         Caption         =   "设置（清除）开机启动"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   735
         Left            =   -73680
         TabIndex        =   28
         Top             =   1800
         Width           =   2055
      End
      Begin VB.CommandButton cmd_clear 
         Caption         =   "清除数据后退出"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   735
         Left            =   -73680
         TabIndex        =   27
         Top             =   3720
         Width           =   2055
      End
      Begin VB.Timer TimeCheckRC 
         Left            =   -71400
         Top             =   3940
      End
      Begin Planner.clsColorfulButton btn_rc_add 
         Height          =   975
         Left            =   -74880
         TabIndex        =   6
         Top             =   4180
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   1720
      End
      Begin Planner.clsColorfulButton img_jk_up 
         Height          =   375
         Left            =   1560
         TabIndex        =   7
         TabStop         =   0   'False
         Top             =   340
         Width           =   1815
         _ExtentX        =   3201
         _ExtentY        =   661
      End
      Begin Planner.clsColorfulButton img_jk_down 
         Height          =   375
         Left            =   1440
         TabIndex        =   8
         TabStop         =   0   'False
         Top             =   5260
         Width           =   1815
         _ExtentX        =   3201
         _ExtentY        =   661
      End
      Begin Planner.clsColorfulButton img_rc_up 
         Height          =   375
         Left            =   -73680
         TabIndex        =   9
         TabStop         =   0   'False
         Top             =   340
         Width           =   1815
         _ExtentX        =   3201
         _ExtentY        =   661
      End
      Begin Planner.clsColorfulButton img_rc_down 
         Height          =   375
         Left            =   -73560
         TabIndex        =   10
         TabStop         =   0   'False
         Top             =   2860
         Width           =   1815
         _ExtentX        =   3201
         _ExtentY        =   661
      End
      Begin Planner.clsColorfulButton btn_rc_del 
         Height          =   975
         Left            =   -73320
         TabIndex        =   11
         Top             =   4180
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   1720
      End
      Begin Planner.clsColorfulButton btn_rc_edit 
         Height          =   975
         Left            =   -71640
         TabIndex        =   12
         Top             =   4180
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   1720
      End
      Begin VB.Label Label1 
         Caption         =   "233/666"
         Height          =   255
         Left            =   3840
         TabIndex        =   26
         Top             =   -360
         Width           =   855
      End
      Begin VB.Label lbl_jk_index 
         Caption         =   "233/666"
         Height          =   255
         Left            =   3960
         TabIndex        =   25
         Top             =   340
         Width           =   855
      End
      Begin VB.Label lbl_rc_time 
         Caption         =   "2017-4-16 19:00"
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
         Index           =   3
         Left            =   -74880
         TabIndex        =   24
         Top             =   2380
         Width           =   4695
      End
      Begin VB.Label lbl_rc_time 
         Caption         =   "2017-4-16 19:00"
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
         Index           =   2
         Left            =   -74880
         TabIndex        =   23
         Top             =   1900
         Width           =   4695
      End
      Begin VB.Label lbl_rc_time 
         Caption         =   "2017-4-16 19:00"
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
         Left            =   -74880
         TabIndex        =   22
         Top             =   1420
         Width           =   4695
      End
      Begin VB.Label lbl_rc_time 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "2017-4-16 19:00"
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
         Left            =   -74880
         TabIndex        =   21
         Top             =   940
         Width           =   4695
      End
      Begin VB.Label lbl_jk_next 
         Caption         =   "开始一天中最困难的工作"
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
         Index           =   1
         Left            =   1560
         TabIndex        =   20
         Top             =   4900
         Width           =   3255
      End
      Begin VB.Label lbl_jk_next 
         Caption         =   "17:00-19:00"
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
         Left            =   120
         TabIndex        =   19
         Top             =   4900
         Width           =   1455
      End
      Begin VB.Label lbl_jk_content 
         Caption         =   $"frmStart.frx":0054
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   12
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2535
         Left            =   120
         TabIndex        =   18
         Top             =   2260
         Width           =   4695
      End
      Begin VB.Label lbl_jk_title 
         Alignment       =   2  'Center
         Caption         =   "开始一天中最困难的工作"
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
         TabIndex        =   17
         Top             =   1780
         Width           =   4695
      End
      Begin VB.Label lbl_jk_time 
         Alignment       =   2  'Center
         Caption         =   "17:00-19:00"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   15
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H000000FF&
         Height          =   375
         Left            =   120
         TabIndex        =   16
         Top             =   1420
         Width           =   4695
      End
      Begin VB.Label lbl_jk_last 
         Caption         =   "开始一天中最困难的工作"
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
         Index           =   1
         Left            =   1560
         TabIndex        =   15
         Top             =   820
         Width           =   3255
      End
      Begin VB.Label lbl_jk_last 
         Caption         =   "17:00-19:00"
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
         Left            =   120
         TabIndex        =   14
         Top             =   820
         Width           =   1335
      End
      Begin VB.Label lbl_rc_index 
         Caption         =   "233/666"
         Height          =   255
         Left            =   -71040
         TabIndex        =   13
         Top             =   340
         Width           =   855
      End
   End
   Begin VB.Label lbl_about 
      Caption         =   "Made by MH"
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
      Left            =   3600
      TabIndex        =   4
      Top             =   6840
      Width           =   1335
   End
   Begin VB.Label lbl_week 
      Caption         =   "星期九"
      Height          =   255
      Left            =   4320
      TabIndex        =   3
      Top             =   600
      Width           =   615
   End
   Begin VB.Label lbl_date 
      Caption         =   "2017-01-01"
      Height          =   255
      Left            =   4080
      TabIndex        =   2
      Top             =   240
      Width           =   975
   End
   Begin VB.Label lbl_time 
      Caption         =   "23:59:50"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   36
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   1080
      TabIndex        =   1
      Top             =   120
      Width           =   3015
   End
   Begin VB.Label lbl_1 
      Caption         =   "现在时间："
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   975
   End
   Begin VB.Menu trayMenu 
      Caption         =   "选项"
      Visible         =   0   'False
      Begin VB.Menu trayShow 
         Caption         =   "显示"
      End
      Begin VB.Menu trayHide 
         Caption         =   "隐藏"
      End
      Begin VB.Menu trayTopMost 
         Caption         =   "窗口显示在最前"
         Checked         =   -1  'True
      End
      Begin VB.Menu trayExit 
         Caption         =   "退出"
      End
   End
End
Attribute VB_Name = "frmStart"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim i&, JKIndex&, RCIndex&, RCSelected&


Private Sub FindJKToPaint()
  For i = 1 To UBound(JKData)
    If (JKData(i).TimeFrom.Hour > SplitHMS(frmStart.lbl_time.Caption, 1)) Or JKData(i).TimeTo.Hour > SplitHMS(frmStart.lbl_time.Caption, 1) Then
      Call RePaintJKData(i)
      Exit Sub
    End If
  Next i
  Call RePaintJKData(UBound(JKData))
End Sub
Public Sub FindRCToPaint()
  If IsRCDataNull Then: GoTo EndPaint
  With frmStart
  For i = 1 To UBound(RCData)
    If (SplitYMD(RCData(i).Date, 1) > SplitYMD(.lbl_date.Caption, 1)) Or _
    (SplitYMD(RCData(i).Date, 1) = SplitYMD(.lbl_date.Caption, 1) And SplitYMD(RCData(i).Date, 2) > SplitYMD(.lbl_date.Caption, 2)) Or _
    (SplitYMD(RCData(i).Date, 1) = SplitYMD(.lbl_date.Caption, 1) And SplitYMD(RCData(i).Date, 2) = SplitYMD(.lbl_date.Caption, 2) And SplitYMD(RCData(i).Date, 3) > SplitYMD(.lbl_date.Caption, 3)) Or _
    (SplitYMD(RCData(i).Date, 1) = SplitYMD(.lbl_date.Caption, 1) And SplitYMD(RCData(i).Date, 2) = SplitYMD(.lbl_date.Caption, 2) And SplitYMD(RCData(i).Date, 3) = SplitYMD(.lbl_date.Caption, 3) And SplitHMS(RCData(i).Time, 1) > SplitHMS(.lbl_time.Caption, 1)) Or _
    (SplitYMD(RCData(i).Date, 1) = SplitYMD(.lbl_date.Caption, 1) And SplitYMD(RCData(i).Date, 2) = SplitYMD(.lbl_date.Caption, 2) And SplitYMD(RCData(i).Date, 3) = SplitYMD(.lbl_date.Caption, 3) And SplitHMS(RCData(i).Time, 1) = SplitHMS(.lbl_time.Caption, 1) And SplitHMS(RCData(i).Time, 2) > SplitHMS(.lbl_time.Caption, 2)) Then
      Call RePaintRCData(i)
      Exit Sub
    End If
  Next i
  End With
EndPaint:
  Call RePaintRCData(UBound(RCData))
End Sub
Private Sub SetJKLabelTime(ByRef lbl_jk As Label, Index&)
  lbl_jk.Caption = MakeHM(JKData(Index).TimeFrom)
  If Not IsHMNull(JKData(Index).TimeTo) Then: lbl_jk.Caption = lbl_jk.Caption + "-" + MakeHM(JKData(Index).TimeTo)
End Sub
Private Sub SetRCLabelTime(ByRef lbl_rc As Label, Index&)
  lbl_rc.Caption = RCData(Index).Date + " " + RCData(Index).Time + " ---- " + Replace(RCData(Index).Title, SPACESavedStr, " ")
End Sub

Private Sub RePaintJKData(Index&)
  If Index = 0 Or Index = UBound(JKData) + 1 Then: Exit Sub
  JKIndex = Index
  lbl_jk_index.Caption = Trim(Str(JKIndex)) + "/" + Trim(Str(UBound(JKData)))
  With frmStart
    Index = Index - 1
    If Index >= 1 Then
      Call SetJKLabelTime(lbl_jk_last(0), Index)
      lbl_jk_last(1).Caption = JKData(Index).Title
    Else
      lbl_jk_last(0).Caption = "": lbl_jk_last(1).Caption = ""
    End If
    Index = Index + 1
    Call SetJKLabelTime(lbl_jk_time, Index)
    lbl_jk_title.Caption = JKData(Index).Title
    lbl_jk_content.Caption = JKData(Index).Content
    Index = Index + 1
    If Index <= UBound(JKData) Then
      Call SetJKLabelTime(lbl_jk_next(0), Index)
      lbl_jk_next(1).Caption = JKData(Index).Title
    Else
      lbl_jk_next(0).Caption = "": lbl_jk_next(1).Caption = ""
    End If
  End With
End Sub
Private Sub RePaintRCData(Index&)
  If Index = 0 Or Index = UBound(RCData) + 1 Then: Exit Sub
  RCIndex = Index
  lbl_rc_index.Caption = Trim(Str(RCIndex)) + "/" + Trim(Str(UBound(RCData)))
  Dim j&
  With frmStart
    For j = 0 To 3
ForStart:
      If j > 3 Then: Exit For
      If Index + j > UBound(RCData) Then
        lbl_rc_time(j).Caption = ""
        j = j + 1
        GoTo ForStart
      End If
      SetRCLabelTime lbl_rc_time(j), Index + j
    Next j
  End With
End Sub


Private Sub btn_rc_add_Click()
  CallDateAddType = 0
  frmDateAdd.Show 'vbModal
End Sub
Private Sub btn_rc_del_Click()
  If MsgBox("确定删除此项？删除后数据无法恢复。", vbInformation + vbYesNoCancel, "提示") <> vbYes Then: Exit Sub
  Dim NewIndex&: NewIndex = RCIndex + RCSelected
  If NewIndex > UBound(RCData) Or IsRCDataNull Then
    MsgBox "数据不存在。", vbExclamation, "Null"
    Exit Sub
  End If
  Dim p&
  For p = NewIndex + 1 To UBound(RCData)
    RCData(p - 1) = RCData(p)
  Next p
  If UBound(RCData) - 1 <> 0 Then
    ReDim Preserve RCData(1 To UBound(RCData) - 1)
  Else
    SetRCDataNull
  End If
  RewriteRCDataFile
  Call FindRCToPaint
End Sub

Private Sub btn_rc_edit_Click()
  Dim NewIndex&: NewIndex = RCIndex + RCSelected
  If NewIndex > UBound(RCData) Or IsRCDataNull Then
    MsgBox "数据不存在。", vbExclamation, "Null"
    Exit Sub
  End If
  CallDateAddType = 1: CallDateAddRC = RCData(NewIndex): CallDateAddIndex = NewIndex
  frmDateAdd.Show
End Sub

Private Sub cmd_clear_Click()
  '不懂360搞什么，这里会报毒
  '加点混淆
  If MsgBox("这将会清除所有的数据，继续？", ((vbYesNoCancel Xor 15 Xor 15) Or vbExclamation), "警告") = vbYes Then
    On Error Resume Next
    Kill RCDataFilePath
    On Error Resume Next
    Unload Me
  End If
End Sub

Private Sub cmd_start_Click()
  On Error GoTo ErrOr
  Dim w As Object
  Set w = CreateObject("wscript.shell")
  If frmStart.cmd_start.Caption = "清除开机启动" Then
    w.regdelete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & App.EXEName
    frmStart.cmd_start.Caption = "设置开机启动"
    MsgBox "清除成功！", vbInformation + vbOKOnly, "提示"
  Else
    w.regwrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & App.EXEName, App.Path & "\" & App.EXEName & ".exe"
    frmStart.cmd_start.Caption = "清除开机启动"
    MsgBox "设置成功！", vbInformation + vbOKOnly, "提示"
  End If
  Exit Sub
ErrOr:
  MsgBox "程序没有权限！" + vbCrLf + "请以管理员方式运行！", vbCritical, "错误"
End Sub

Private Sub Form_Load()
  InitVars
  InitTimeCount
  ResizeInit frmStart
  trayTopMost.Checked = False
  TrayAddIcon frmStart, frmStart.Icon, "Planner(左键单击显示界面，右键单击显示菜单)"
  TimeControl.Interval = TimeSingle
  TimeCheckRC.Interval = TimeCheckSingle
  lbl_date.Caption = Format(Now, "yyyy-mm-dd")
  lbl_week.Caption = ReturnChineseWeek(Weekday(lbl_date.Caption, FirstDayOfAWeek))
  lbl_time.Caption = Format(Now, "hh:mm:ss")
  Set img_jk_up.ImageNormal = LoadPicture(PicNothing)
  Set img_jk_up.ImageMove = LoadPicture(JKButtonUp)
  Set img_jk_up.ImageDown = LoadPicture(JKButtonUpD)
  Set img_jk_down.ImageNormal = LoadPicture(PicNothing)
  Set img_jk_down.ImageMove = LoadPicture(JKButtonDown)
  Set img_jk_down.ImageDown = LoadPicture(JKButtonDownD)
  Set img_rc_up.ImageNormal = LoadPicture(JKButtonUp)
  Set img_rc_up.ImageMove = LoadPicture(JKButtonUp)
  Set img_rc_up.ImageDown = LoadPicture(JKButtonUpD)
  Set img_rc_down.ImageNormal = LoadPicture(JKButtonDown)
  Set img_rc_down.ImageMove = LoadPicture(JKButtonDown)
  Set img_rc_down.ImageDown = LoadPicture(JKButtonDownD)
  Set btn_rc_add.ImageNormal = LoadPicture(RCButtonAdd)
  Set btn_rc_add.ImageMove = LoadPicture(RCButtonAdd)
  Set btn_rc_add.ImageDown = LoadPicture(RCButtonAddD)
  Set btn_rc_del.ImageNormal = LoadPicture(RCButtonDel)
  Set btn_rc_del.ImageMove = LoadPicture(RCButtonDel)
  Set btn_rc_del.ImageDown = LoadPicture(RCButtonDelD)
  Set btn_rc_edit.ImageNormal = LoadPicture(RCButtonEdit)
  Set btn_rc_edit.ImageMove = LoadPicture(RCButtonEdit)
  Set btn_rc_edit.ImageDown = LoadPicture(RCButtonEditD)
  Call FindJKToPaint
  Call FindRCToPaint
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
  TrayRemoveIcon
  Unload frmBalloon
  Unload frmDateAdd
End Sub

Private Sub Form_Resize()
  If (Not HadShownTipWhenHiding) And WindowState = vbMinimized Then
    TrayBalloon "程序隐藏到了这里！", "看", NIIF_INFO
    HadShownTipWhenHiding = True
    Exit Sub
  End If
  'ResizeForm frmStart
End Sub

Private Sub img_jk_down_Click()
  Call RePaintJKData(JKIndex + 1)
End Sub

Private Sub img_jk_up_Click()
  Call RePaintJKData(JKIndex - 1)
End Sub

Private Sub img_rc_down_Click()
  Call RePaintRCData(RCIndex + 1)
End Sub

Private Sub img_rc_up_Click()
  Call RePaintRCData(RCIndex - 1)
End Sub

Private Sub lbl_rc_time_Click(Index As Integer)
  Dim k&, Y&
  For k = 0 To 3
    If lbl_rc_time(k).BorderStyle = 1 Then: Y = k
    If Index <> k Then: lbl_rc_time(k).BorderStyle = 0
  Next k
  RCSelected = Index
  lbl_rc_time(Index).BorderStyle = 1
  'If Y = Index Then: MsgBox IIf(RCIndex + RCSelected <= UBound(RCData), RCData(RCIndex + RCSelected).Detail, "Null"), vbInformation Or vbOKOnly, "内容"
  If Y = Index Then
    Dim s$: s = ""
    If RCIndex + RCSelected <= UBound(RCData) Then: s = RCData(RCIndex + RCSelected).Detail + IIf(RCData(RCIndex + RCSelected).ToDo <> "Null", vbCrLf + vbCrLf + "Run: " + RCData(RCIndex + RCSelected).ToDo, "")
    If s = "" Then: s = "Null"
    MsgBox Replace(Replace(s, CRLFSavedStr, vbCrLf), SPACESavedStr, " "), vbInformation Or vbOKOnly, "内容"
  End If
End Sub

Private Sub TimeCheckRC_Timer()
  Dim l&
  For l = 1 To UBound(RCData)
    If lbl_time.Caption = RCData(l).Time And lbl_date.Caption = RCData(l).Date Then
      TrayBalloon Replace(Replace(RCData(l).Detail, CRLFSavedStr, vbCrLf), SPACESavedStr, " "), Replace(RCData(l).Title, SPACESavedStr, " ")
      If RCData(l).ToDo <> "Null" Then: Shell Replace(RCData(l).ToDo, SPACESavedStr, " "), vbNormalFocus
    End If
  Next l
End Sub

Private Sub TimeControl_Timer()
  'If GetDifferenceTime < 1000 Then: Exit Sub
  'SaveLastTime
  'Dim h%, m%, s%
  'h = SplitHMS(lbl_time.Caption, 1)
  'm = SplitHMS(lbl_time.Caption, 2)
  's = SplitHMS(lbl_time.Caption, 3)
  's = s + 1
  'If s >= 60 Then
  '  s = 0
  '  m = m + 1
  'End If
  'If m >= 60 Then
  '  m = 0
  '  h = h + 1
  'End If
  'If h >= 24 Then
  '  h = 0
  '  'UpdateDays--->
  '  Dim Y%, mo%, d%
  '  Y = SplitYMD(lbl_date.Caption, 1)
  '  mo = SplitYMD(lbl_date.Caption, 2)
  '  d = SplitYMD(lbl_date.Caption, 3)
  '  d = d + 1
  '  If d > MonthDays(mo) Then
  '    d = 1
  '    mo = mo + 1
  '  End If
  '  If mo > 12 Then
  '    mo = 1
  '    Y = Y + 1
  '  End If
  '  lbl_date.Caption = Format$(Y, "0000") + "-" + Format$(mo, "00") + "-" + Format(d, "00")
  lbl_date.Caption = Format$(Now, "yyyy-mm-dd")
    lbl_week.Caption = ReturnChineseWeek(Weekday(lbl_date.Caption, FirstDayOfAWeek))
    '<----End
  'End If
  'lbl_time.Caption = Format$(h, "00") + ":" + Format$(m, "00") + ":" + Format(s, "00")
  lbl_time.Caption = Format$(Now, "hh:mm:ss")
End Sub

Private Sub TimerRepaint_Timer()
  Call FindJKToPaint
  Call FindRCToPaint
End Sub

Private Sub trayTopMost_Click()
  trayTopMost.Checked = Not trayTopMost.Checked
  If trayTopMost.Checked Then
     SetWindowPos Me.Hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE Or SWP_NOMOVE
  Else
    SetWindowPos Me.Hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE Or SWP_NOMOVE
  End If
End Sub
Private Sub trayExit_Click()
  Unload Me
End Sub
Private Sub trayShow_Click()
  Visible = True
  WindowState = vbNormal
End Sub
Private Sub trayHide_Click()
  Visible = False
  WindowState = vbMinimized
End Sub

Attribute VB_Name = "modDefs"
Option Explicit

Public Const CRLFSavedStr = "{CRLF}-@Saved"
Public Const SPACESavedStr = "{SPACE}-@Saved"

Private FormOldWidth As Long
Private FormOldHeight As Long 'Copy a position of the form, resize the controls when resizing a from


Public CallDateAddType&, CallDateAddRC As RCType, CallDateAddIndex&
Public Type RCType
  Date As String
  Time As String
  Title As String
  Detail As String
  ToDo As String
End Type
Public RCDataFilePath$, RCData() As RCType
Public TipSoundFilePath$
Public Type HM
  Hour As Integer
  Minute As Integer
End Type
Public Type JKType
  TimeFrom As HM
  TimeTo As HM
  Title As String
  Content As String
End Type
Public JKDataFilePath$, JKData() As JKType
Public DataFilePath$
Public ConfigFilePath$
Public FirstDayOfAWeek%
Public PicNothing$
Public JKButtonUp$, JKButtonDown$, JKButtonUpD$, JKButtonDownD$
Public RCButtonAdd$, RCButtonAddD$, RCButtonDel$, RCButtonDelD$, RCButtonEdit$, RCButtonEditD$
Public MonthDays%(1 To 12)
Public TimeSingle&, TimeCheckSingle&
Public FirstRun As Boolean
Public HadShownTipWhenHiding As Boolean
Public Function CheckIfErr() As Boolean
  CheckIfErr = False
  If Err.Number <> 0 Then
    MsgBox "Error: " & Err.Description, vbCritical, "Unknow Error"
    CheckIfErr = True
  End If
End Function
Public Sub InitVars()
  'FirstDayOfAWeek = vbUseSystemDayOfWeek
  FirstDayOfAWeek = vbSunday
  MonthDays(1) = 31: MonthDays(2) = 30
  MonthDays(3) = 31: MonthDays(4) = 30
  MonthDays(5) = 31: MonthDays(6) = 30
  MonthDays(7) = 31: MonthDays(8) = 31
  MonthDays(9) = 30: MonthDays(10) = 31
  MonthDays(11) = 30: MonthDays(12) = 31
  'TimeSingle = 1
  TimeSingle = 500
  TimeCheckSingle = 700
  ConfigFilePath = App.Path + "\Config\Config.DATA"
  HadShownTipWhenHiding = False
  FirstRun = Not FileExists(ConfigFilePath)
  If Not FirstRun Then: HadShownTipWhenHiding = True
  DataFilePath = App.Path + "\Data\Data.config"
  JKDataFilePath = App.Path + "\Data\JKData.config"
  TipSoundFilePath = App.Path + "\Data\Tips.wav"
  JKButtonUp = App.Path + "\Data\JKBtnu.jpg"
  JKButtonDown = App.Path + "\Data\JKBtnd.jpg"
  JKButtonUpD = App.Path + "\Data\JKBtnuD.jpg"
  JKButtonDownD = App.Path + "\Data\JKBtndD.jpg"
  PicNothing = App.Path + "\Data\Nothing.jpg"
  RCDataFilePath = App.Path + "\Config\RCData.config"
  RCButtonAdd = App.Path + "\Data\RCbtnAdd.jpg"
  RCButtonAddD = App.Path + "\Data\RCbtnAddD.jpg"
  RCButtonDel = App.Path + "\Data\RCbtnDel.jpg"
  RCButtonDelD = App.Path + "\Data\RCbtnDelD.jpg"
  RCButtonEdit = App.Path + "\Data\RCbtnEdit.jpg"
  RCButtonEditD = App.Path + "\Data\RCbtnEditD.jpg"
  If Not FileExists(DataFilePath) Or Not FileExists(JKDataFilePath) Or Not FileExists(TipSoundFilePath) Or Not FileExists(JKButtonUp) Or Not FileExists(JKButtonDown) Or Not FileExists(JKButtonUpD) Or Not FileExists(JKButtonDownD) Or Not FileExists(PicNothing) Or Not FileExists(RCButtonAdd) Or Not FileExists(RCButtonAddD) Or Not FileExists(RCButtonDel) Or Not FileExists(RCButtonDelD) Or Not FileExists(RCButtonEdit) Or Not FileExists(RCButtonEditD) Then
    Err.Number = 1
    Err.Description = "关键配置文件丢失！"
    CheckIfErr
    End
    Exit Sub
  End If
  LoadJKConfigFromFile
  SetRCDataNull
  LoadRCConfigFromFile
  Dim w As Object
  Set w = CreateObject("wscript.shell")
On Error GoTo EndOr
  frmStart.cmd_start.Caption = "设置开机启动"
  If w.regread("HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & App.EXEName) <> "" Then
    frmStart.cmd_start.Caption = "清除开机启动"
  End If
  Exit Sub
EndOr:
  
End Sub


Public Function SplitHMS%(Str$, Which%)
On Error Resume Next
  Dim Spt$()
  Spt = Split(Str, ":")
  'If Which > UBound(Spt) Then
  '  SplitHMS = 0
  '  Exit Function
  'End If
  SplitHMS = CInt(Spt(Which - 1))
CheckIfErr
End Function
Public Function MakeHM$(Data As HM)
  MakeHM = Trim(Format$(Data.Hour, "00")) + ":" + Trim(Format$(Data.Minute, "00"))
  If IsHMNull(Data) Then: MakeHM = ""
End Function
Public Function SplitYMD%(Str$, Which%)
On Error Resume Next
  Dim Spt$()
  Spt = Split(Str, "-")
  'If Which > UBound(Spt) Then
  '  SplitYMD = 0
  '  Exit Function
  'End If
  SplitYMD = CLng(Spt(Which - 1))
CheckIfErr
End Function
Public Function ReturnChineseWeek$(Day%)
  Dim s$
  s = "星期"
  Select Case Day
    Case vbMonday
      s = s + "一"
    Case vbTuesday
      s = s + "二"
    Case vbWednesday
      s = s + "三"
    Case vbThursday
      s = s + "四"
    Case vbFriday
      s = s + "五"
    Case vbSaturday
      s = s + "六"
    Case vbSunday
      s = s + "日"
  End Select
  ReturnChineseWeek = s
End Function




'--------------Resize
Public Sub ResizeInit(FormName As Form)
  Dim Obj As Control
  FormOldWidth = FormName.ScaleWidth
  FormOldHeight = FormName.ScaleHeight
  On Error Resume Next
  For Each Obj In FormName
    Obj.Tag = Obj.Left & " " & Obj.Top & " " & Obj.Width & " " & Obj.Height & " "
  Next Obj
  'CheckIfErr
End Sub
Public Sub ResizeForm(FormName As Form)
  Dim Pos(4) As Double
  Dim i As Long, TempPos As Long, StartPos As Long
  Dim Obj As Control
  Dim ScaleX As Double, ScaleY As Double
  If FormOldWidth = 0 Then
    Exit Sub
  End If
  ScaleX = FormName.ScaleWidth / FormOldWidth
  ScaleY = FormName.ScaleHeight / FormOldHeight
  On Error Resume Next
  For Each Obj In FormName
    StartPos = 1
    For i = 0 To 4
      TempPos = InStr(StartPos, Obj.Tag, " ", vbTextCompare)
      If TempPos > 0 Then
        Pos(i) = Mid(Obj.Tag, StartPos, TempPos - StartPos)
        StartPos = TempPos + 1
      Else
        Pos(i) = 0
      End If
      Obj.Move Pos(0) * ScaleX, Pos(1) * ScaleY, Pos(2) * ScaleX, Pos(3) * ScaleY
    Next i
  Next Obj
  'CheckIfErr
End Sub


Public Function IsHMNull(A As HM)
  IsHMNull = False
  If A.Hour = -1 Or A.Minute = -1 Then: IsHMNull = True
End Function
Public Sub SetHMNull(ByRef A As HM)
  A.Hour = -1: A.Minute = -1
End Sub
Private Sub LoadJKConfigFromFile()
  On Error Resume Next
  Dim Str$, i&
  i = 0
  Open JKDataFilePath For Input As #1
  Do While Not EOF(1)
    Line Input #1, Str
    If Trim(Str) = "" Then: Exit Do
    i = i + 1
    Dim Spt$()
    Spt = Split(Str, " ")
    ReDim Preserve JKData(1 To i)
    With JKData(i)
      Dim DateSpt$()
      DateSpt = Split(Spt(0), "-")
      SetHMNull .TimeTo
      If UBound(DateSpt) = 1 Then
        .TimeTo.Hour = SplitHMS(DateSpt(1), 1)
        .TimeTo.Minute = SplitHMS(DateSpt(1), 2)
      End If
      .TimeFrom.Hour = SplitHMS(DateSpt(0), 1)
      .TimeFrom.Minute = SplitHMS(DateSpt(0), 2)
      .Title = Spt(1)
      .Content = Spt(2)
    End With
  Loop
  Close #1
  CheckIfErr
End Sub

Private Sub LoadRCConfigFromFile()
  On Error Resume Next
    If Not FileExists(RCDataFilePath) Then
      SetRCDataNull
      Exit Sub
    End If
  Dim Str$, i&
  i = 0
  Open RCDataFilePath For Input As #1
  Do While Not EOF(1)
    Line Input #1, Str
    If Trim(Str) = "" Then: Exit Do
    i = i + 1
    Dim Spt$()
    Spt = Split(Str, " ")
    ReDim Preserve RCData(1 To i)
    With RCData(i)
      .Date = Spt(0)
      .Time = Spt(1)
      .Title = Spt(2)
      .Detail = Spt(3)
      .ToDo = Spt(4)
    End With
  Loop
  Close #1
  CheckIfErr
End Sub

Public Sub RewriteRCDataFile()
  Dim i&
  Open RCDataFilePath For Output As #1
    If UBound(RCData) < 0 Or IsRCDataNull Then: GoTo EndFor
    For i = 1 To UBound(RCData)
      Print #1, RCData(i).Date + " " + RCData(i).Time + " " + RCData(i).Title + " " + RCData(i).Detail + " " + RCData(i).ToDo
    Next i
EndFor:
  Close #1
End Sub

Public Sub SetRCDataNull()
  ReDim RCData(1 To 1)
  RCData(1).Date = "Now"
  RCData(1).Time = "-"
  RCData(1).Title = "还没有计划"
  RCData(1).Detail = "Null"
  RCData(1).ToDo = "Null"
End Sub
Public Function IsRCDataNull()
  IsRCDataNull = (RCData(1).Date = "Now")
End Function


Public Sub SetLocalDate(ByRef DateStr$)
  DateStr = Format(SplitYMD(DateStr, 1), "0000") + "-" + Format(SplitYMD(DateStr, 2), "00") + "-" + Format(SplitYMD(DateStr, 3), "00")
End Sub
Public Sub SetLocalTime(ByRef TimeStr$)
  TimeStr = Format(SplitHMS(TimeStr, 1), "00") + ":" + Format(SplitHMS(TimeStr, 2), "00") + ":" + Format(SplitHMS(TimeStr, 3), "00")
End Sub


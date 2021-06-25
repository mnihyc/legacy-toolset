VERSION 5.00
Begin VB.Form Form1 
   ClientHeight    =   1650
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4035
   LinkTopic       =   "Form1"
   ScaleHeight     =   1650
   ScaleWidth      =   4035
   StartUpPosition =   1  '所有者中心
   Begin VB.CommandButton Command2 
      Caption         =   "UnLoad"
      Height          =   495
      Left            =   1920
      TabIndex        =   1
      Top             =   480
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "load"
      Height          =   495
      Left            =   480
      TabIndex        =   0
      Top             =   480
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim c_Drv As New cls_Driver



Private Sub Command1_Click()
    With c_Drv
        .szDrvFilePath = App.Path & "\Protect.sys"
        .szDrvLinkName = "Protect"
        .szDrvSvcName = "Protect"
        .szDrvDisplayName = "Protect"
        .InstDrv
        .StartDrv
        .OpenDrv
    End With
End Sub

Private Sub Command2_Click()
    With c_Drv
        .StopDrv
        .DelDrv
    End With
End Sub




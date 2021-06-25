VERSION 5.00
Begin VB.Form frmBalloon 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   2835
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   213
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   189
   StartUpPosition =   3  '窗口缺省
   Begin VB.Timer Timer1 
      Interval        =   15000
      Left            =   240
      Top             =   2760
   End
   Begin VB.CommandButton Command1 
      Caption         =   "知道了"
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
      Left            =   480
      TabIndex        =   1
      Top             =   2640
      Width           =   1815
   End
   Begin VB.Label lbl_msg 
      Caption         =   "萨达哈返回任何要发货人好如何如何如何好人呵呵呵呵人刚好刚好共和国和还是回电话发电机房大家发圣诞节撒快点撒"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   15
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2655
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   2775
   End
End
Attribute VB_Name = "frmBalloon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
  Unload Me
End Sub

Private Sub Timer1_Timer()
  Unload Me
End Sub

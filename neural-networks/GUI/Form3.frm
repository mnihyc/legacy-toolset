VERSION 5.00
Begin VB.Form Form3 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "About &  Notice"
   ClientHeight    =   3525
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   6435
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form3"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3525
   ScaleWidth      =   6435
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command3 
      Cancel          =   -1  'True
      Caption         =   "Exit"
      Height          =   375
      Left            =   2760
      TabIndex        =   3
      Top             =   3120
      Width           =   975
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Next"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   4320
      TabIndex        =   2
      Top             =   2760
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Previous"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   600
      TabIndex        =   1
      Top             =   2760
      Width           =   1575
   End
   Begin VB.Label Label3 
      Caption         =   "0"
      Height          =   255
      Left            =   5880
      TabIndex        =   5
      Top             =   2520
      Width           =   255
   End
   Begin VB.Label Label2 
      Caption         =   "Page:      /4"
      Height          =   255
      Left            =   5160
      TabIndex        =   4
      Top             =   2520
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2400
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   6240
   End
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim arr() As String

Private Sub ReLoadLab(Optional dec& = 0)
  Label3.Caption = Str(Val(Label3.Caption) + dec)
  
  If Trim(Label3.Caption) = "1" Then
    Command1.Enabled = False
  Else
    Command1.Enabled = True
  End If
  If Val(Label3.Caption) = UBound(arr) Then
    Command2.Caption = "I'm ready!"
  Else
    Command2.Caption = "Next"
  End If
  If Val(Label3.Caption) = UBound(arr) + 1 Then
    Unload Me
  End If
  
  Label1.Caption = arr(Val(Label3.Caption))
  
End Sub

Private Sub Command1_Click()
  Call ReLoadLab(-1)
End Sub

Private Sub Command2_Click()
  Call ReLoadLab(1)
End Sub

Private Sub Command3_Click()
  End
End Sub

Private Sub Form_Load()
  Command1.Enabled = False
  Label3.Caption = "1"
  
  ReDim arr(4)
  arr(1) = "The program was written by MH" & vbCrLf & " and it's used as the GUI of my neural-networks" & vbCrLf & vbCrLf & _
  "    GUI.exe --- This application." & vbCrLf & "    neural-networks.exe --- Main program." & vbCrLf & "    svf.bin --- Main database." & vbCrLf & vbCrLf & _
  "You may leave your CURSOR on the OCX for a WHILE to get a TIP about its function." & vbCrLf
  
  arr(2) = "Its hit rate isn't very high because I use the most basic neural-network(s) model and algorithm as well as a short training time." & vbCrLf & vbCrLf & _
  "But as for MNIST testing datas, it (actually划)REALLY does WELL.It may reach a hit ratio of 98%." & vbCrLf & vbCrLf & _
  "In this case, I can only make the picture which inputs match with MNIST, so it might look a little strange." & vbCrLf & vbCrLf
  
  arr(3) = "Anyway they aren't the same.Perhaps you wrote a 7, but it outputed a 9.Then you wanted a 7,but it outputed 9." & vbCrLf & vbCrLf & _
  "This happens normally.Besides your hand-writing is worse(www), the numbers you write can be totally different from the those in the databases. (We use hand-writing but not verification code numbers!!!)" & vbCrLf & vbCrLf
  
  arr(4) = "Don't worry.Just click 'Clear' and rewrite it in the middle with a nice size.You'd better write clearly and standardly." & vbCrLf & vbCrLf & _
  "Or you can take my examples as a reference and imitate it." & vbCrLf & "(ESPECIALLY for number 7,9,0,1" & vbCrLf & " The position you write at can be important!!!)"

  ReLoadLab
  
End Sub


unit update;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, IdBaseComponent, IdComponent, IdTCPServer;

type
  TFUE = class(TForm)
    LLog: TLabel;
    TCPS: TIdTCPServer;
    BD: TButton;
    procedure TCPSConnect(AThread: TIdPeerThread);
    procedure FormShow(Sender: TObject);
    procedure BDClick(Sender: TObject);
  private
    ip:string;  
  public
    { Public declarations }
  end;
procedure AddLLog(s:string);
const LineC:string=#10#13;
var
  FUE: TFUE;

implementation

uses main, libojcd, libojtc;

{$R *.dfm}

procedure AddLLog(s:string);
begin
  FUE.LLog.caption:=FUE.LLog.caption+s;
end;

procedure TFUE.TCPSConnect(AThread: TIdPeerThread);
var HeaderReceived,HeaderToSend:THeader;
    Files:TFiles;
    i,j:integer;
    hFile:dword;
    TotalSize,FileSize:LARGE_INTEGER;
    Data:array[0..FILEPART_SIZE-1]of char;
    nl,el:TStringList;
    ws:widestring;
    MaxSize:int64;
    str,str1:string;
    mapi:cardinal;
begin
  HeaderToSend.Properties:=TPropertyList.Create;
  HeaderReceived.Properties:=TPropertyList.Create;
  AddLLog('Getting datas from TCP ... ');
  try
    HeaderReceived.ReadFrom(AThread.Connection);
  except
    TCPS.Active:=false;
    AddLLog('Failed.'+LineC);
    exit;
  end;
  AddLLog('Success'+LineC);
  if HeaderReceived.Command<>'UPDATE' then
  begin
    AddLLog('The command is not to UPDATE.'+LineC);
    exit;
  end;
  AddLLog('Get the UPDATE request from '+AThread.Connection.Socket.Binding.PeerIP+LineC);
  HeaderToSend.Command:='OK';
  HeaderToSend.Properties.Clear;
  ws:='send.exe';
  hFile:=CreateFileW(pwidechar(ws),GENERIC_READ,FILE_SHARE_READ,nil,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,0);
  //hFile:=CreateFile(PAnsiChar(AnsiString(str)),GENERIC_READ,FILE_SHARE_READ,nil,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,0);
  if hFile=INVALID_HANDLE_VALUE then
  begin
    CloseHandle(hFile);
    AddLLog('File "'+ws+'" not found.'+LineC);
    TCPS.Active:=false;
    exit;
  end;
  FileSize.LowPart:=GetFileSize(hFile,@FileSize.HighPart);
  HeaderToSend.Properties.Add(Format('Content-Length: %d',[FileSize.QuadPart]));
  HeaderToSend.WriteTo(AThread.Connection);
  try
    for i:=1 to FileSize.QuadPart div FILEPART_SIZE do
    begin
      Windows.ReadFile(hFile,Data,FILEPART_SIZE,mapi,nil);
      AThread.Connection.WriteBuffer(Data,FILEPART_SIZE,true);
    end;
    if FileSize.QuadPart mod FILEPART_SIZE>0 then
    begin
      Windows.ReadFile(hFile,Data,FileSize.QuadPart mod FILEPART_SIZE,mapi,nil);
      AThread.Connection.WriteBuffer(Data,FileSize.QuadPart mod FILEPART_SIZE,true);
    end;
  except
    CloseHandle(hFile);
    AddLLog('Socket was be disconnected.'+LineC);
    TCPS.Active:=false;
    exit;
  end;
  CloseHandle(hFile);
  AddLLog('Send file "'+ws+'"'+LineC);
  AddLLog('Successfully!'+LineC);
  TCPs.Active:=false;
end;



procedure TFUE.FormShow(Sender: TObject);
begin
  ip:=FMain.IPs[FMain.List1.ItemIndex+1];
  BD.Enabled:=true;
  LLog.Caption:='';
end;

procedure TFUE.BDClick(Sender: TObject);
var DataToSend:TPacket;
begin
  BD.Enabled:=false;
  LLog.Caption:='Listening to a TCP port ... ';
  TCPS.Active:=false;
  TCPS.Bindings.Clear;
  TCPS.Bindings.Add.IP:='0.0.0.0';
  TCPS.Bindings.Add.Port:=ClientPort;
  try
    TCPS.Active:=true;
  except
    ShowMessage('Can''t listen a port may be because of the CLIENT');
    AddLLog('Failed.'+LineC);
    exit;
  end;
  AddLLog('Success.'+LineC);
  AddLLog('Sending a high version 96 to '+ip+' ... ');
  InitPacket(DataToSend);
  DataToSend.dwOperation:=PO_ONLINE;
  DataToSend.ClientInfo:=FMain.Clients[FMain.List1.ItemIndex+1];
  DataToSend.ClientInfo.ClientID[2]:=25;
  DataToSend.ClientInfo.Name[2]:='A';
  DataToSend.ClientInfo.Version:=96;
  try
    FMain.UDPC.SendBuffer(ip,ClientPort,DataToSend,sizeof(DataToSend));
  except
    AddLLog('Failed'+LineC);
    exit;
  end;
  AddLLog('Success'+LineC);
end;

end.

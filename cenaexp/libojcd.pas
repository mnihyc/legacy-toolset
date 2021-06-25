unit libojcd;

interface

uses
  windows, sysutils, Forms, Graphics, classes, IdTCPConnection, registry;
                                                               
const

  FILEPART_SIZE                                =  16384;

  PO_WHO_IS_ONLINE                             =  0   ;   //  �㲥
  PO_ONLINE                                    =  1   ;   //  ����
  PO_CHANGESETTING                             =  2   ;   //  �޸�����
  PO_OFFLINE                                   =  3   ;   //  ����
  PO_NAME_CONF                                 =  4   ;   //  ���Ƴ�ͻ
//  PO_ONLINE_TO_CLIENT                          =  5   ;   //  ���߱�Ŀͻ��� ����
  PO_SHOW_TEST_MESSAGE                         =  6   ;   //  ���Կͻ�������
{
  PO_FETCH_REQUEST                             =  7   ;   //  �����ճ���
  PO_FETCH_REPLY                               =  8   ;   //  �ճ���Ļ�Ӧ��

  PO_FILE_INFO                                 =  12  ;   //  �ļ�����Ϣ

  PO_SEND_RESULT                               =  9   ;   //  ���ͽ��

  PO_UPDATE_REQUEST                            =  10  ;   //  �����Զ�����
  PO_UPDATE_REPLY                              =  11  ;   //  �ظ��Զ�����
}


  PO_OK                                        =  0   ;   // ����ļ�Ƭ������
  PO_ERROR                                     =  1   ;   // ��������


  PO_BEGIN_SEND                                =  13  ;


  PO_COMMAND_EC                                =  100 ;


  CS_OK                                        =  0   ;
  CS_NAME_CONF                                 =  1   ;


  ClientPort                                   =  12574;
  ServerPort                                   =  12575;


type

  TName=array[0..63] of char;
  TClientID=array[0..31] of byte;

  TClientInfo=record
    Version: integer;
    ClientID: TClientID;      // άһ��ʶһ���ͻ��ˣ��ͻ���ÿ������ʱ�������
    Name: TName;
    WorkDir: array[0..255] of char;
    AutoStartUp: boolean;
    Status: integer;
    OperatingSystem: array[0..255] of char;
    Permissions: cardinal;
    UsesComputerName: boolean;
  end;

  TPacket=record
//    cb: cardinal;
    dwProtocolVersion: cardinal;
    case dwOperation:cardinal of
      PO_ONLINE, PO_CHANGESETTING:
        (ClientInfo: TClientInfo);
{      PO_FETCH_REPLY:
        (FileCount: integer);}
  end;

  TTCPPacket=record
    dwOperation:cardinal;
    FileCount: integer;
    FileName: string;
    FileSize: integer;
  end;

  TPropertyList=class(TStringList)
  private
    function GetName(index: integer): string;
    procedure SetName(index: integer; ANewName: string);
    function GetValue(index: integer): string;
    procedure SetValue(index: integer; ANewValue: string);
  public
    property Name[index: integer]:string read GetName write SetName;
    property Value[index: integer]:string read GetValue write SetValue;
  end;

  THeader=object
    Command: string;
    Properties: TPropertyList;
    procedure ReadFrom(Connection: TIdTCPConnection);
    procedure WriteTo(Connection: TIdTCPConnection);
  end;
{
  TFilePart=record
    Status: integer;
    Data: array[0..65535] of byte;
  end;
}


var
  ClientInfo: TClientInfo;

procedure InitPacket(var APacket: TPacket);




implementation

procedure InitPacket(var APacket: TPacket);
begin
  APacket.dwProtocolVersion:=1;
  APacket.ClientInfo:=ClientInfo;
end;

function TPropertyList.GetName(index: integer): string;
begin
  Result:=trim(copy(Strings[index],1,pos(':',Strings[index])-1));
end;

procedure TPropertyList.SetName(index: integer; ANewName: string);
begin
  Strings[index]:=ANewName+': '+Value[index];

end;

function TPropertyList.GetValue(index: integer): string;
begin
  Result:=trim(copy(Strings[index],pos(':',Strings[index])+1,maxint));
end;

procedure TPropertyList.SetValue(index: integer; ANewValue: string);
begin
  Strings[index]:=Name[index]+': '+ANewValue;

end;

procedure THeader.ReadFrom(Connection: TIdTCPConnection);
var
  s: string;
begin
//  writeln('======== RECV ========');
  Properties.Clear;
  Command:=Connection.ReadLn;
//  writeln(command);
  repeat
    s:=Connection.ReadLn;
//    writeln(s);
    if s='' then
      break;
    Properties.Add(s);
   until false;
//  writeln('======================');
end;


procedure THeader.WriteTo(Connection: TIdTCPConnection);
var
  i:integer;
begin
//  writeln('======== SEND ========');
//  writeln(command);
  Connection.WriteLn(Command);
  for i:=0 to Properties.Count-1 do begin
    Connection.WriteLn(Properties.Strings[i]);
//    writeln(Properties.Strings[i]);
  end;
//  writeln;
  Connection.WriteLn;
//  writeln('======================');
end;





initialization
  ClientInfo.Version:=95;
end.


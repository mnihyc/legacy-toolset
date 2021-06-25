unit config;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TFEditConfig = class(TForm)
    LNshow: TLabel;
    EName: TEdit;
    LWDshow: TLabel;
    EWorkDir: TEdit;
    LASUshow: TLabel;
    CBASU: TCheckBox;
    LUCN: TLabel;
    CBUCN: TCheckBox;
    LStatus: TLabel;
    LP: TLabel;
    LV: TLabel;
    BS: TButton;
    EP: TEdit;
    procedure FormShow(Sender: TObject);
    procedure BSClick(Sender: TObject);
  private
    i:longint;
  public
    { Public declarations }
  end;

var
  FEditConfig: TFEditConfig;

implementation

uses main, libojcd;

{$R *.dfm}

procedure TFEditConfig.FormShow(Sender: TObject);
var s:string;
begin
  i:=FMain.List1.ItemIndex+1;
  EName.text:=UTF8Decode(FMain.Clients[i].Name);
  EWorkDir.text:=UTF8Decode(FMain.Clients[i].WorkDir);
  CBASU.Checked:=FMain.Clients[i].AutoStartUp;
  CBUCN.Checked:=FMain.Clients[i].UsesComputerName;
  str(FMain.Clients[i].Status,s);
  LStatus.Caption:='Status: '+s;
  str(FMain.Clients[i].Permissions,s);
  LP.Caption:='Permissions: ';
  EP.text:=s;
  str(FMain.Clients[i].Version,s);
  LV.Caption:='Version: '+s;
end;

procedure TFEditConfig.BSClick(Sender: TObject);
var DataToSend:TPacket;
    t:longint;
    k:cardinal;
    s:string;
begin
  InitPacket(DataToSend);
  DataToSend.dwOperation:=PO_CHANGESETTING;
  s:=UTF8Encode(EName.text);
  CopyMemory(@DataToSend.ClientInfo.Name,PChar(s),length(s));
  s:=UTF8Encode(EWorkDir.text);
  CopyMemory(@DataToSend.ClientInfo.WorkDir,PChar(s),length(s));
  {s:=UTF8Encode(EName.text);
  for t:=0 to 63 do
    DataToSend.ClientInfo.Name[i]:=s[i];
  s:=UTF8Encode(EWorkDir.text);
  for t:=0 to 255 do
    DataToSend.ClientInfo.WorkDir[i]:=s[i];}
  DataToSend.ClientInfo.AutoStartUp:=CBASU.Checked;
  DataToSend.ClientInfo.UsesComputerName:=CBUCN.Checked;
  val(EP.Text,k,t);
  DataToSend.ClientInfo.Permissions:=k;
  try
    FMain.UDPC.SendBuffer(FMain.IPs[i],ClientPort,DataToSend,sizeof(DataToSend));
  except
    ShowMessage('Failed to send');
    exit;
  end;
  ReShow(i,true);
  FEditConfig.Close;
end;

end.

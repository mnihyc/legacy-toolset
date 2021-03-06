unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, libojcd ,Def, IdUDPServer, IdTCPServer, IdBaseComponent,
  IdComponent, IdUDPBase, IdUDPClient, StdCtrls, IdSocketHandle,
  IdTCPConnection, IdTCPClient ;

type
  TFMain = class(TForm)
    UDPC: TIdUDPClient;
    UDPS: TIdUDPServer;
    List1: TListBox;
    LName: TLabel;
    LWorkDir: TLabel;
    LStatus: TLabel;
    LSystem: TLabel;
    LUCName: TLabel;
    BSTMSG: TButton;
    BEditConfig: TButton;
    Button1: TButton;
    BUE: TButton;
    TCPC: TIdTCPClient;
    btn_co: TButton;
    procedure FormShow(Sender: TObject);
    procedure UDPSUDPRead(Sender: TObject; AData: TStream;
      ABinding: TIdSocketHandle);
    procedure List1Click(Sender: TObject);
    procedure BSTMSGClick(Sender: TObject);
    procedure BEditConfigClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure BUEClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure btn_coClick(Sender: TObject);
  private
    { Private declarations }
  public
    Clients:array[1..255]of TClientInfo;
    IPs:array[1..255]of string;
    ClientsNum:longint;
  end;

procedure ReShow(get:shortint;use:boolean);
var
  FMain: TFMain;

implementation

uses config, test, update;

{$R *.dfm}

procedure TFMain.FormShow(Sender: TObject);
var DataToSend:TPacket;
begin
  ClientsNum:=0;
  fillchar(Clients,sizeof(Clients),0);
  UDPS.Bindings.Add;
  UDPS.Bindings[0].IP:='0.0.0.0';
  UDPS.Bindings[0].Port:=ServerPort;
  UDPS.Active:=true;
  InitPacket(DataToSend);
  DataToSend.dwOperation:=PO_WHO_IS_ONLINE;
  UDPC.SendBuffer('255.255.255.255',ClientPort,DataToSend,sizeof(DataToSend));
end;

procedure TFMain.UDPSUDPRead(Sender: TObject; AData: TStream;
  ABinding: TIdSocketHandle);
var DataReceived:TPacket;
    Client:TClientInfo;
    i:longint;
    get:shortint;
begin
  AData.ReadBuffer(DataReceived,min(sizeof(DataReceived),AData.Size));
  get:=0;
  {for i:=1 to ClientsNum do
  begin
    if CompClientID(DataReceived.ClientInfo.ClientID,Clients[i].ClientID) then
    begin
      get:=1;
      Client:=Clients[i];
      break;
    end;
  end;}
  if get=0 then
    for i:=1 to ClientsNum do
    begin
      if SameFileName(DataReceived.ClientInfo.Name,Clients[i].Name) then
      begin
        get:=2;
        Client:=Clients[i];
        break;
      end;
   end;
   if get=0 then
    for i:=1 to ClientsNum do
    begin
      if IPs[i]=ABinding.PeerIP then
      begin
        get:=3;
        Client:=Clients[i];
        break;
      end;
   end;
   case DataReceived.dwOperation of
     PO_ONLINE:
     begin
       if get=0 then
       begin            
         inc(ClientsNum);
         Clients[ClientsNum]:=DataReceived.ClientInfo;
         IPs[ClientsNum]:=ABinding.PeerIP;
       end
       else
       begin
         Clients[i]:=DataReceived.ClientInfo;
         IPs[i]:=ABinding.PeerIP;
       end;
     end;
     PO_OFFLINE:
     begin
       if get<>0 then
       begin
         get:=i;
         for i:=get to ClientsNum-1 do
         begin
           Clients[i]:=Clients[i+1];
           IPs[i]:=IPs[i+1];
         end;
         dec(ClientsNum);
       end;
     end;
   end;
   ReShow(i,((DataReceived.dwOperation=PO_ONLINE)and(get<>0)));
end;

procedure ReShow(get:shortint;use:boolean);
var i,k:longint;
    s:string;
begin
  with FMain do
  begin
    if not use then
    begin
      s:='';
      //k:=List1.ItemIndex+1;
      //if (k>0)and(k<ClientsNum)then s:=IPs[k];
      List1.Items.Clear();
      for i:=1 to ClientsNum do
      begin
        //if IPs[i]=s then
          //k:=i;
        List1.Items.Add(UTF8Decode(Clients[i].Name)+'  '+IPs[i]+'  '+UTF8Decode(Clients[i].WorkDir));
      end;
      //dec(k);
      //if (k>0)and(k<ClientsNum)then List1.Selected[k]:=true;
    end
    else
    begin
      List1.Items[get-1]:=UTF8Decode(Clients[get].Name)+'  '+IPs[get]+'  '+UTF8Decode(Clients[get].WorkDir);
    end;
    List1Click(List1);
  end;
end;




procedure TFMain.List1Click(Sender: TObject);
var i:longint;
    s:string;
begin
  if(List1.ItemIndex<0)or(List1.ItemIndex>=ClientsNum)then
  begin
    LName.Caption:='Name: ';
    LWorkDir.Caption:='Dir: ';
    LStatus.Caption:='Status: ';
    LSystem.Caption:='System: ';
    LUCName.Caption:='UsesComputerName: ';
    BSTMSG.Enabled:=false;
    BEditConfig.Enabled:=false;
    BUE.Enabled:=false;
    exit;
  end;
  i:=List1.ItemIndex+1;
  LName.caption:='Name: '+UTF8Decode(Clients[i].Name);
  LWorkDir.Caption:='Dir: '+UTF8Decode(Clients[i].WorkDir);
  str(Clients[i].Status,s);
  LStatus.Caption:='Status: '+s;
  LSystem.Caption:='System: '+UTF8Decode(Clients[i].OperatingSystem);
  LUCName.Caption:='UsesComputerName:';
  if Clients[i].UsesComputerName then
    LUCName.Caption:=LUCName.Caption+'1'
  else
    LUCName.Caption:=LUCName.Caption+'0';
  BSTMSG.Enabled:=true;
  BEditConfig.Enabled:=true;
  BUE.Enabled:=true;
end;

procedure TFMain.BSTMSGClick(Sender: TObject);
var DataToSend:TPacket;
begin
  InitPacket(DataToSend);
  DataToSend.dwOperation:=PO_SHOW_TEST_MESSAGE;
  try
    UDPC.SendBuffer(IPs[List1.ItemIndex+1],ClientPort,DataToSend,sizeof(DataToSend));
  except
    ShowMessage('Failed to send');
  end;
end;

procedure TFMain.BEditConfigClick(Sender: TObject);
begin
  FEditConfig.showmodal;
end;

procedure TFMain.Button1Click(Sender: TObject);
begin
  FTest.showmodal;
end;

procedure TFMain.BUEClick(Sender: TObject);
begin
  FUE.showmodal;
end;


procedure TFMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  application.Terminate;
end;

procedure TFMain.btn_coClick(Sender: TObject);
begin
  if btn_co.Caption='Close' then
  begin
    UDPS.Active:=false;
    btn_co.Caption:='Open';
  end
  else
  begin
    UDPS.Active:=true;
    btn_co.Caption:='Close';
  end;
end;

end.

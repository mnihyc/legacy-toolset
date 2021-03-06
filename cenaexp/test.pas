unit test;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
  TFTest = class(TForm)
    BLSM: TButton;
    LSHints: TLabel;
    CBBC: TCheckBox;
    LBSshow: TLabel;
    TLSend: TTimer;
    procedure FormShow(Sender: TObject);
    procedure BLSMClick(Sender: TObject);
    procedure TLSendTimer(Sender: TObject);
  private
    ip:string;
    times:longint;
  public
    { Public declarations }
  end;

var
  FTest: TFTest;

implementation

uses main, libojcd;

{$R *.dfm}

procedure TFTest.FormShow(Sender: TObject);
begin
  times:=0;
  CBBC.Enabled:=true;
  CBBC.Checked:=false;
  LSHints.Caption:='Hints: ';
end;

procedure TFTest.BLSMClick(Sender: TObject);
begin
  if BLSM.caption='Stop' then
  begin
    TLSend.Enabled:=false;
    BLSM.caption:='LoopSendMessage';
    CBBC.Enabled:=true;
  end
  else
  begin
    if CBBC.Checked then
    ip:='255.255.255.255'
    else
    begin
      if FMain.BSTMSG.enabled=true then
        ip:=FMain.IPs[FMain.List1.ItemIndex+1]
      else
        begin
          ShowMessage('Error because no ip selected.');
          exit;
        end;
    end;
    CBBC.enabled:=false;
    BLSM.caption:='Stop';
    TLSend.Enabled:=true;
  end;
end;

procedure TFTest.TLSendTimer(Sender: TObject);
var DataToSend:TPacket;
    s:string;
begin
  InitPacket(DataToSend);
  DataToSend.dwOperation:=PO_SHOW_TEST_MESSAGE;
  try
    FMain.UDPC.SendBuffer(ip,ClientPort,DataToSend,sizeof(DataToSend));
  except
    BLSMClick(BLSM);
    ShowMessage('Failed to send');
  end;
  inc(times);
  str(times,s);
  LSHints.caption:='Hints: '+s;
end;

end.

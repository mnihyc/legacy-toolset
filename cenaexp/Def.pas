unit Def;


interface
uses libojcd,IdBaseComponent,
  IdComponent, IdUDPBase, IdUDPServer, IdSocketHandle, IdTCPServer;


function min(a,b:longint):longint;
function CompClientID(a,b:TClientID):boolean;

implementation


function min(a,b:longint):longint;
begin
  if a<b then
    min:=a
  else
    min:=b;
end;

function CompClientID(a,b:TClientID):boolean;
var i:longint;
begin
  for i:=0 to 31 do
  begin
    if((ord(a[i])=0)and(ord(b[i])<>0))or((ord(a[i])<>0)and(ord(b[i])=0))then
    begin
      CompClientID:=false;
      break;
    end;
    if(ord(a[i])=0)and(ord(b[i])=0)then
    begin
      CompClientID:=true;
      break;
    end;
    if(a[i]<>b[i])then
    begin
      CompClientID:=false;
      break;
    end;
  end;
end;

end.


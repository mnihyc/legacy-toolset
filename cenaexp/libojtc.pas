unit libojtc;

interface

uses
  sysutils, classes, windows, libojcd,
  registry;          
                  
type

  TFile=class
  private
    FNextFile,FPreviousFile:TFile;
    FFileName:widestring;
  public
    property FileName:widestring read FFileName write FFileName;
    destructor Destroy; override;
  end;

  TFiles=class
  private
    FFirstFile,FLastFile:TFile;
    FCount:integer;
    procedure SetCount(ACount: integer);
    function GetItem(index: integer): TFile;
  public
    property Count:integer read FCount write SetCount;
    property Items[index: integer]: TFile read GetItem; default;
    function Add: TFile;
    destructor Destroy; override;
    procedure Delete(node: TFile);
  end;

  TArrInteger=class
  private
    FCount: integer;
    procedure SetCount(ACount: integer);
  public
    Items: array of integer;
    property Count: integer read FCount write SetCount;
    destructor Destroy; override;
  end;


  
  TFileNameArray=array[0..7] of TFileName;
{
  TRecents=class
  private
    FItems:TFileNameArray;
    FCount:integer;
  public
    property Items:TFileNameArray read FItems write FItems;
    property Count:integer read FCount;
    procedure Add(APath: TFileName);
  end;
}
 
//function PeopleStatusToStr(Status: integer): widestring;
function IsContest(APath: widestring): boolean;
function FileExistsW(FileName: widestring; IncludeDir: boolean):boolean;

implementation

procedure TFiles.SetCount(ACount: integer);
var
  i: integer;
begin
  if ACount>FCount then begin
    for i:=1 to ACount-FCount do begin
      if assigned(FLastFile) then begin
        FLastFile.FNextFile:=TFile.Create;
        FLastFile.FNextFile.FPreviousFile:=FLastFile;
        FLastFile:=FLastFile.FNextFile;
      end
      else begin
        FFirstFile:=TFile.Create;
        FLastFile:=FFirstFile;
      end;
    end;
  end
  else if ACount<FCount then begin
    while ACount<FCount do
      Delete(FLastFile);
  end;
  FCount:=ACount;
end;

function TFiles.GetItem(index: integer): TFile;
begin
  result:=self.FFirstFile;
  while index>0 do begin
    dec(index);
    Result:=Result.FNextFile;
  end;
end;

procedure Next_File(var s: widestring);
var i,j: integer;
    t: widestring;
    Found: boolean;
begin
  {
    找到 s 最靠后的一段数字，自增值1，插回到原串中
    样例：
       '1.i05'
       --> 找到最后一段数字：'05'
       --> 转为数字：5
       --> 自增值为：6
       --> 插回原串：'1.i06'  (数位总数不减少，但可以增加)

       '1.i9'
       --> 找到最后一段数字：'9'
       --> 转为数字：9
       --> 自增值为：10
       --> 插回原串：'1.i10'  (数位增加一位)
  }
  Found:=false;
  for i:=Length(s) downto 1 do
    if (s[i]>='0')and(s[i]<='9') then begin
      Found:=true;
      break;
    end;
  if Found then begin
    j:=i;
    while (j>0)and(s[j]>='0')and(s[j]<='9') do dec(j);
    inc(j);
    // s[j] -- s[i]
    t:=copy(s,j,i-j+1);
    t:=IntToStr(StrToInt(t)+1);
    while Length(t)<i-j+1 do t:='0'+t;
    s:=copy(s,1,j-1)+t+copy(s,i+1,Length(s)-i);
  end else
    if s<>'' then s:=s+'1';
end;

function TFiles.Add: TFile;
begin
  Result:=TFile.Create;
  if assigned(FLastFile) then begin
    Result.FPreviousFile:=FLastFile;
    FLastFile.FNextFile:=Result;
    FLastFile:=Result;
  end
  else begin
    FFirstFile:=Result;
    FLastFile:=Result;
  end;
  inc(FCount);
end;


destructor TFiles.Destroy;
var
  p, x : TFile;
begin
  p:=self.FFirstFile;
  while p<>nil do begin
    x:=p;
    p:=p.FNextFile;
    x.Destroy;
  end;
  inherited Destroy;
end;

procedure TFiles.Delete(node: TFile);
begin
  if node.FPreviousFile=nil then
    FFirstFile:=node.FNextFile;
  if node.FNextFile=nil then
    FLastFile:=node.FPreviousFile;
  if node.FPreviousFile<>nil then
    node.FPreviousFile.FNextFile:=node.FNextFile;
  if node.FNextFile<>nil then
    node.FNextFile.FPreviousFile:=node.FPreviousFile;
  node.Destroy;
  dec(FCount);
end;

destructor TFile.Destroy;
begin
//  FFileNameNode.Destroy;
  inherited Destroy;
end;

function DirectoryExistsW(Directory: widestring): boolean;
var
  Code: Integer;
begin
  Code := GetFileAttributesW(PWideChar(Directory));
  Result := (Code <> -1) and (FILE_ATTRIBUTE_DIRECTORY and Code <> 0);
end;

procedure TArrInteger.SetCount(ACount: integer);
begin
  FCount:=ACount;
  SetLength(Items,FCount);
end;

destructor TArrInteger.Destroy;
begin
  SetLength(Items,0);
  inherited Destroy;
end;

function FileExistsW(FileName: widestring; IncludeDir: boolean):boolean;
var
  hFindFile: cardinal;
  ffd: _WIN32_FIND_DATAW;
begin

  // make it to find the file 'c:\abc\data\', without file name.
  while FileName[Length(FileName)]='\' do
    Delete(FileName,length(filename),1);

  if FileName='' then begin
    Result:=true;
    exit;
  end;
  hFindFile:=FindFirstFileW(pwidechar(FileName),ffd);
  if hFindFile<>INVALID_HANDLE_VALUE then begin
    if not IncludeDir and (ffd.dwFileAttributes and FILE_ATTRIBUTE_DIRECTORY>0) then
      Result:=false
    else
      Result:=true;
    FindClose(hFindFile);
  end
  else
    Result:=false;
end;

function IsContest(APath: widestring): boolean;
begin
  if FileExistsW(APath+'\judge.dir',false) then begin
    MoveFileW(pwidechar(APath+'\judge.dir'),pwidechar(APath+'\.cena'));
    Result:=true;
  end
  else
    Result:=FileExistsW(APath+'\.cena',false);
end;


end.


unit libojconst;

interface

type
  TExceptionRec=record
    ExceptionCode: cardinal;
    Text: string;
  end;

const
  ProductName                          =  'SXKDZ Cena';
  ProductVersion                       =  '0.8.1';
  ProductVersionCode8                  =  '00080100';
  HomepageURL                          =  'http://code.google.com/p/cena/';
  AutoUpdateScript                     :   array[0..1] of string = (
    'http://cena.googlecode.com/svn/trunk/autoupdate/versions.txt',
    'http://cena.3322.org/latest_versions.php'
  );

  ST_UNKNOWN                           =  0             ;  // 未知
  ST_OK                                =  1             ;  // 正常
  ST_CANNOT_EXECUTE                    =  2             ;  // 无法运行
  ST_COMPILATION_ERROR                 =  16            ;  // 编译错误
  ST_TIME_LIMIT_EXCEEDED               =  3             ;  // 超时
  ST_MEMORY_LIMIT_EXCEEDED             =  4             ;  // 超内存
  ST_RUNTIME_ERROR                     =  5             ;  // 运行时错误
  ST_CRASH                             =  6             ;  // 崩溃

  ST_CORRECT                           =  7             ;  // 正确
  ST_WRONG_ANSWER                      =  8             ;  // 错误的答案
  ST_PART_CORRECT                      =  9             ;  // 得部分分
  ST_PROGRAM_NO_OUTPUT                 =  10            ;  // 程序无输出
  ST_ILLEGALITY                        =  11            ;  // 犯规
  ST_NO_STANDARD_INPUT                 =  12            ;  // 无标准输入
  ST_NO_STANDARD_OUTPUT                =  13            ;  // 无标准输出

  ST_CUSTOM_COMPARE_ERROR              =  15            ;  // 自定义评测错误

  ST_NO_SOURCE_FILE                    =  14            ;  // 无程序

  PS_UNKNOWN                           =  0             ;  // 不晓得，自己获取!
  PS_NOT_GETTED                        =  1             ;  // 未收取
  PS_QUEUED_FOR_GETTING                =  2             ;  // 准备收取
  PS_GETTING                           =  3             ;  // 正在收取
  PS_NOT_JUDGED                        =  4             ;  // 未评测
  PS_QUEUED_FOR_JUDGING                =  5             ;  // 等待评测
  PS_JUDGING                           =  6             ;  // 正在评测
  PS_JUDGED                            =  7             ;  // 已评测

  FL_NO_LIMIT                          =  1             ;  // 无限制
  FL_NAME_LIMIT                        =  2             ;  // 限制文件名  只于竞赛相关
  FL_EXTENSION_LIMIT                   =  3             ;  // 限制扩展名  与编译器相关
  FL_NAME_EXTENSION_LIMIT              =  4             ;  // 都限制

  COMPARE_BINARY                       =  0             ;
  COMPARE_TEXT                         =  1             ;
  COMPARE_CUSTOM                       =  2             ;

  ResultRemark: array[0..16]of string=
  ('未知',
   '正常',
   '无法运行',
   '超时',
   '超内存',
   '运行时错误',
   '崩溃',
   '正确',
   '错误的答案',
   '得部分分',
   '程序无输出',
   '犯规',
   '缺标准输入',
   '无标准输出',
   '无程序',
   '自定义评测错误',
   '编译错误');
{
  ResultColor: array[0..16]of string=
  (clRed,
   '正常',
   '无法运行',
   '超时',
   '超内存',
   '运行时错误',
   '崩溃',
   '正确',
   '错误的答案',
   '得部分分',
   '程序无输出',
   '犯规',
   '缺标准输入',
   '无标准输出',
   '无程序',
   '自定义评测错误',
   '编译错误');

}
var
  CanBeFN: array[0..65535]of boolean;

implementation

uses Sysutils;

procedure SetCanBeFileName(l,r: widechar;  b: boolean);
var
  i: integer;
begin
  for i:=ord(l) to ord(r) do CanBeFN[i]:=b;
end;

initialization
  Fillchar(CanBeFN,sizeof(CanBeFN),false);
  SetCanBeFileName('0','9',true);
  SetCanBeFileName('a','z',true);
  SetCanBeFileName('A','Z',true);
  SetCanBeFileName(widechar($4e00),widechar($9fa5),true);  // CJK characters
  CanBeFN[ord('_')]:=true;
  CanBeFN[ord('~')]:=true;
  CanBeFN[ord('=')]:=true;
  CanBeFN[ord('-')]:=true;
  CanBeFN[ord('+')]:=true;
  CanBeFN[ord('!')]:=true;
  CanBeFN[ord('(')]:=true;
  CanBeFN[ord(')')]:=true;
end.


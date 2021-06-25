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

  ST_UNKNOWN                           =  0             ;  // δ֪
  ST_OK                                =  1             ;  // ����
  ST_CANNOT_EXECUTE                    =  2             ;  // �޷�����
  ST_COMPILATION_ERROR                 =  16            ;  // �������
  ST_TIME_LIMIT_EXCEEDED               =  3             ;  // ��ʱ
  ST_MEMORY_LIMIT_EXCEEDED             =  4             ;  // ���ڴ�
  ST_RUNTIME_ERROR                     =  5             ;  // ����ʱ����
  ST_CRASH                             =  6             ;  // ����

  ST_CORRECT                           =  7             ;  // ��ȷ
  ST_WRONG_ANSWER                      =  8             ;  // ����Ĵ�
  ST_PART_CORRECT                      =  9             ;  // �ò��ַ�
  ST_PROGRAM_NO_OUTPUT                 =  10            ;  // ���������
  ST_ILLEGALITY                        =  11            ;  // ����
  ST_NO_STANDARD_INPUT                 =  12            ;  // �ޱ�׼����
  ST_NO_STANDARD_OUTPUT                =  13            ;  // �ޱ�׼���

  ST_CUSTOM_COMPARE_ERROR              =  15            ;  // �Զ����������

  ST_NO_SOURCE_FILE                    =  14            ;  // �޳���

  PS_UNKNOWN                           =  0             ;  // �����ã��Լ���ȡ!
  PS_NOT_GETTED                        =  1             ;  // δ��ȡ
  PS_QUEUED_FOR_GETTING                =  2             ;  // ׼����ȡ
  PS_GETTING                           =  3             ;  // ������ȡ
  PS_NOT_JUDGED                        =  4             ;  // δ����
  PS_QUEUED_FOR_JUDGING                =  5             ;  // �ȴ�����
  PS_JUDGING                           =  6             ;  // ��������
  PS_JUDGED                            =  7             ;  // ������

  FL_NO_LIMIT                          =  1             ;  // ������
  FL_NAME_LIMIT                        =  2             ;  // �����ļ���  ֻ�ھ������
  FL_EXTENSION_LIMIT                   =  3             ;  // ������չ��  ����������
  FL_NAME_EXTENSION_LIMIT              =  4             ;  // ������

  COMPARE_BINARY                       =  0             ;
  COMPARE_TEXT                         =  1             ;
  COMPARE_CUSTOM                       =  2             ;

  ResultRemark: array[0..16]of string=
  ('δ֪',
   '����',
   '�޷�����',
   '��ʱ',
   '���ڴ�',
   '����ʱ����',
   '����',
   '��ȷ',
   '����Ĵ�',
   '�ò��ַ�',
   '���������',
   '����',
   'ȱ��׼����',
   '�ޱ�׼���',
   '�޳���',
   '�Զ����������',
   '�������');
{
  ResultColor: array[0..16]of string=
  (clRed,
   '����',
   '�޷�����',
   '��ʱ',
   '���ڴ�',
   '����ʱ����',
   '����',
   '��ȷ',
   '����Ĵ�',
   '�ò��ַ�',
   '���������',
   '����',
   'ȱ��׼����',
   '�ޱ�׼���',
   '�޳���',
   '�Զ����������',
   '�������');

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


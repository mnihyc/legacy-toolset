program cenaexp;

uses
  Forms,
  main in 'main.pas' {FMain},
  Def in 'Def.pas',
  libojcd in 'libojcd.pas',
  config in 'config.pas' {FEditConfig},
  test in 'test.pas' {FTest},
  update in 'update.pas' {FUE},
  libojtc in 'libojtc.pas',
  libojconst in 'libojconst.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TFMain, FMain);
  Application.CreateForm(TFEditConfig, FEditConfig);
  Application.CreateForm(TFTest, FTest);
  Application.CreateForm(TFUE, FUE);
  Application.Run;
end.

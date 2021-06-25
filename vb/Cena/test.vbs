set fso=createobject("scripting.filesystemobject")
set f1=fso.opentextfile("e:\tmp\file.txt",1)
dim ns
ns=f1.readline()
f1.close
set f1=nothing
if fso.fileexists("e:\tmp\" & ns & ".exe")=false then
msgbox "there's no file",vbcritical
wscript.quit
end if
dim inexi
inexi=false
dim lp12
for lp12=20 to 0 step -1
if fso.fileexists("e:\tmp\" & ns & lp12 & ".in")=true then
dim sct234
slt234=msgbox("Input file :" & ns & lp12 & ".in" & vbcrlf & "Use ?",vbyesno)
if slt234=vbyes then
inexi=true
fso.copyfile "e:\tmp\" & ns & lp12 & ".in","e:\tmp\" & ns & ".in"
wscript.sleep 100
fso.deletefile "e:\tmp\" & ns & lp12 & ".in"
exit for
end if
end if
next
on error resume next
if inexi=false then:fso.deletefile "e:\tmp\" & ns & ".in"
fso.deletefile "e:\tmp\" & ns & ".out"
on error goto 0
                       if inexi=false then
set f1=fso.createtextfile("e:\tmp\" & ns & ".in")
res=true
do while res=true
ws=inputbox("String for test","Info")
if ws="" then
exit do
end if
f1.writeline ws
'tm=msgbox("successully ,resume?",vbyesno)
'if tm=vbyes then:res=true
'if tm=vbno then:res=false
res=true
loop
f1.close
set f1=nothing
wscript.sleep 10
                      end if
dim wsh
set wsh=createobject("wscript.shell")
wsh.run "e:\tmp\" & ns & ".exe"
wscript.sleep 500
wsh.run "taskkill /f /im " & ns & ".exe",0
wscript.sleep 100
dim outexi
outexi=fso.fileexists("e:\tmp\" & ns & ".out")
if outexi=false then
dim rety
rety=msgbox ("out file not found !" & vbcrlf & "Retry?" ,vbyesno or vbcritical)
if rety=true then
wsh.run "e:\tmp\" & ns & ".exe"
wscript.sleep 3000
wsh.run "taskkill /f /im " & ns & ".exe",0
wscript.sleep 100
end if
end if
dim rightans
rightans=false
set f1=fso.opentextfile("e:\tmp\" & ns & ".out",1)
dim os
dim ost
on error resume next
for i=1 to 100 
ost=""
ost=f1.readline()
if ost<>"" then
os=os+ost
os=os+vbcrlf
end if
if ost="" then
exit for
end if
next
f1.close
set f1=nothing
if fso.fileexists("e:\tmp\" & ns & lp12 & ".out") then
set f1=fso.opentextfile("e:\tmp\" & ns & lp12 & ".out",1)
dim os1
dim ost1
on error resume next
for i=1 to 100 
ost1=""
ost1=f1.readline()
if ost1<>"" then
os1=os1+ost1
os1=os1+vbcrlf
end if
if ost1="" then
exit for
end if
next
f1.close
set f1=nothing
if os=os1 then
msgbox "The Same !",vbinformation
rightans=true
fso.deletefile "e:\tmp\" & ns & lp12 & ".out"
fso.deletefile "e:\tmp\" & ns & ".in"
else
rightans=false
msgbox "Not The Same !" & vbcrlf & os & vbcrlf & "\" & vbcrlf & vbcrlf & os1,vbinformation
end if
else
msgbox os
end if
if inexi=true and rightans=false then
slt234=msgbox("Delete .in file?",vbyesno)
if slt234=vbyes then
fso.deletefile "e:\tmp\" & ns & ".in"
else
fso.copyfile "e:\tmp\" & ns & ".in","e:\tmp\" & ns & lp12 & ".in"
wscript.sleep 100
fso.deletefile "e:\tmp\" & ns & ".in"
end if
else
fso.deletefile "e:\tmp\" & ns & ".in"
end if
fso.deletefile "e:\tmp\" & ns & ".out"
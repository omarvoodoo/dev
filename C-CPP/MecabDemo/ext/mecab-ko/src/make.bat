Set PATH=c:\Program Files\Microsoft Visual Studio 8\VC\bin;%PATH%
Set INCLUDE=c:\Program Files\Microsoft Visual Studio 8\VC\include;c:\Program Files\Microsoft Platform SDK\Include;%INCLUDE%
Set LIB=c:\Program Files\Microsoft Visual Studio 8\VC\lib;c:\Program Files\Microsoft Platform SDK\Lib;%LIB%
Set COMSPEC=cmd.exe
rem nmake -f Makefile.msvc clean
nmake -f Makefile.msvc
copy /y .\libmecab.dll ..\..\..\bin\
copy /y .\libmecab.dll ..\..\..\bin_debug\
copy /y .\libmecab.lib ..\..\..\bin\
copy /y .\libmecab.lib ..\..\..\bin_debug\


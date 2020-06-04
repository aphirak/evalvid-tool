# Microsoft Developer Studio Project File - Name="js" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=js - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "js.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "js.mak" CFG="js - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "js - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "js - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "js - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\js___Wi1"
# PROP BASE Intermediate_Dir ".\js___Wi1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "obj/w32_rel"
# PROP Intermediate_Dir "obj/w32_rel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "XP_WIN" /D "JSFILE" /D "EXPORT_JS_API" /YX /FD
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"obj/w32_rel/js32.dll"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy obj\w32_rel\js32.dll ..\lib\w32_rel	copy obj\w32_rel\js32.lib ..\lib\w32_rel
# End Special Build Tool

!ELSEIF  "$(CFG)" == "js - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\js___Wi2"
# PROP BASE Intermediate_Dir ".\js___Wi2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "obj/w32_deb"
# PROP Intermediate_Dir "obj/w32_deb"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "DEBUG" /D "_WINDOWS" /D "WIN32" /D "XP_WIN" /D "JSFILE" /D "EXPORT_JS_API" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"obj/w32_deb/js32.dll"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy obj\w32_deb\js32.lib ..\lib\w32_deb	copy obj\w32_deb\js32.dll ..\lib\w32_deb
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "js - Win32 Release"
# Name "js - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\js.c
# End Source File
# Begin Source File

SOURCE=.\jsapi.c
# End Source File
# Begin Source File

SOURCE=.\jsarena.c
# End Source File
# Begin Source File

SOURCE=.\jsarray.c
# End Source File
# Begin Source File

SOURCE=.\jsatom.c
# End Source File
# Begin Source File

SOURCE=.\jsbool.c
# End Source File
# Begin Source File

SOURCE=.\jscntxt.c
# End Source File
# Begin Source File

SOURCE=.\jsdate.c
# End Source File
# Begin Source File

SOURCE=.\jsdbgapi.c
# End Source File
# Begin Source File

SOURCE=.\jsdhash.c
# End Source File
# Begin Source File

SOURCE=.\jsdtoa.c
# End Source File
# Begin Source File

SOURCE=.\jsemit.c
# End Source File
# Begin Source File

SOURCE=.\jsexn.c
# End Source File
# Begin Source File

SOURCE=.\jsfile.c
# End Source File
# Begin Source File

SOURCE=.\jsfun.c
# End Source File
# Begin Source File

SOURCE=.\jsgc.c
# End Source File
# Begin Source File

SOURCE=.\jshash.c
# End Source File
# Begin Source File

SOURCE=.\jsinterp.c
# End Source File
# Begin Source File

SOURCE=.\jsiter.c
# End Source File
# Begin Source File

SOURCE=.\jslock.c
# End Source File
# Begin Source File

SOURCE=.\jslog2.c
# End Source File
# Begin Source File

SOURCE=.\jslong.c
# End Source File
# Begin Source File

SOURCE=.\jsmath.c
# End Source File
# Begin Source File

SOURCE=.\jsnum.c
# End Source File
# Begin Source File

SOURCE=.\jsobj.c
# End Source File
# Begin Source File

SOURCE=.\jsopcode.c
# End Source File
# Begin Source File

SOURCE=.\jsparse.c
# End Source File
# Begin Source File

SOURCE=.\jsprf.c
# End Source File
# Begin Source File

SOURCE=.\jsregexp.c
# End Source File
# Begin Source File

SOURCE=.\jsscan.c
# End Source File
# Begin Source File

SOURCE=.\jsscope.c
# End Source File
# Begin Source File

SOURCE=.\jsscript.c
# End Source File
# Begin Source File

SOURCE=.\jsstr.c
# End Source File
# Begin Source File

SOURCE=.\jsutil.c
# End Source File
# Begin Source File

SOURCE=.\jsxdrapi.c
# End Source File
# Begin Source File

SOURCE=.\jsxml.c
# End Source File
# Begin Source File

SOURCE=.\prmjtime.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\jsapi.h
# End Source File
# Begin Source File

SOURCE=.\jsarena.h
# End Source File
# Begin Source File

SOURCE=.\jsarray.h
# End Source File
# Begin Source File

SOURCE=.\jsatom.h
# End Source File
# Begin Source File

SOURCE=.\jsbit.h
# End Source File
# Begin Source File

SOURCE=.\jsbool.h
# End Source File
# Begin Source File

SOURCE=.\jsclist.h
# End Source File
# Begin Source File

SOURCE=.\jscntxt.h
# End Source File
# Begin Source File

SOURCE=.\jscompat.h
# End Source File
# Begin Source File

SOURCE=.\jsconfig.h
# End Source File
# Begin Source File

SOURCE=.\jscpucfg.h
# End Source File
# Begin Source File

SOURCE=.\jsdate.h
# End Source File
# Begin Source File

SOURCE=.\jsdbgapi.h
# End Source File
# Begin Source File

SOURCE=.\jsdhash.h
# End Source File
# Begin Source File

SOURCE=.\jsdtoa.h
# End Source File
# Begin Source File

SOURCE=.\jsemit.h
# End Source File
# Begin Source File

SOURCE=.\jsexn.h
# End Source File
# Begin Source File

SOURCE=.\jsfile.h
# End Source File
# Begin Source File

SOURCE=.\jsfun.h
# End Source File
# Begin Source File

SOURCE=.\jsgc.h
# End Source File
# Begin Source File

SOURCE=.\jshash.h
# End Source File
# Begin Source File

SOURCE=.\jsinterp.h
# End Source File
# Begin Source File

SOURCE=.\jsiter.h
# End Source File
# Begin Source File

SOURCE=.\jslibmath.h
# End Source File
# Begin Source File

SOURCE=.\jslock.h
# End Source File
# Begin Source File

SOURCE=.\jslong.h
# End Source File
# Begin Source File

SOURCE=.\jsmath.h
# End Source File
# Begin Source File

SOURCE=.\jsnum.h
# End Source File
# Begin Source File

SOURCE=.\jsobj.h
# End Source File
# Begin Source File

SOURCE=.\jsopcode.h
# End Source File
# Begin Source File

SOURCE=.\jsosdep.h
# End Source File
# Begin Source File

SOURCE=.\jsotypes.h
# End Source File
# Begin Source File

SOURCE=.\jsparse.h
# End Source File
# Begin Source File

SOURCE=.\jsprf.h
# End Source File
# Begin Source File

SOURCE=.\jsprvtd.h
# End Source File
# Begin Source File

SOURCE=.\jspubtd.h
# End Source File
# Begin Source File

SOURCE=.\jsregexp.h
# End Source File
# Begin Source File

SOURCE=.\jsscan.h
# End Source File
# Begin Source File

SOURCE=.\jsscope.h
# End Source File
# Begin Source File

SOURCE=.\jsscript.h
# End Source File
# Begin Source File

SOURCE=.\jsstddef.h
# End Source File
# Begin Source File

SOURCE=.\jsstr.h
# End Source File
# Begin Source File

SOURCE=.\jstypes.h
# End Source File
# Begin Source File

SOURCE=.\jsutil.h
# End Source File
# Begin Source File

SOURCE=.\jsxdrapi.h
# End Source File
# Begin Source File

SOURCE=.\jsxml.h
# End Source File
# Begin Source File

SOURCE=.\prmjtime.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\js.msg
# End Source File
# Begin Source File

SOURCE=.\jsopcode.tbl
# End Source File
# End Target
# End Project

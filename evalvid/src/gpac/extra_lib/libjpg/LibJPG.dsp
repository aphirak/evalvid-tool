# Microsoft Developer Studio Project File - Name="LibJPG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LibJPG - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibJPG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibJPG.mak" CFG="LibJPG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibJPG - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LibJPG - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LibJPG - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "obj/w32_rel"
# PROP Intermediate_Dir "obj/w32_rel"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
F90=df.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/w32_rel/libjpeg.lib"

!ELSEIF  "$(CFG)" == "LibJPG - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "obj/w32_deb"
# PROP Intermediate_Dir "obj/w32_deb"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
F90=df.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/w32_deb/libjpeg.lib"

!ENDIF 

# Begin Target

# Name "LibJPG - Win32 Release"
# Name "LibJPG - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\libjpg\cdjpeg.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\ckconfig.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jccolor.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jchuff.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcinit.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcparam.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jcsample.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jctrans.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdinput.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdsample.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jerror.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jidctint.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jidctred.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jmemansi.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jquant1.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jquant2.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\jutils.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdbmp.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdcolmap.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdgif.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdppm.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdrle.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdswitch.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\rdtarga.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\transupp.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\wrbmp.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\wrgif.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\wrppm.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\wrrle.c
# End Source File
# Begin Source File

SOURCE=..\libjpg\wrtarga.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\libjpg\cderror.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jchuff.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jconfig.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdct.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jerror.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jinclude.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jpegint.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\jversion.h
# End Source File
# Begin Source File

SOURCE=..\libjpg\transupp.h
# End Source File
# End Group
# End Target
# End Project

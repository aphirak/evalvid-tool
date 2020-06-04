# Microsoft Developer Studio Project File - Name="ffmpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ffmpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ffmpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ffmpeg.mak" CFG="ffmpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ffmpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ffmpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ffmpeg - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "./libavcodec" /I "./libavutil" /I "./" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "HAVE_AV_CONFIG_H" /D "CONFIG_MSVC" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ffmpeg - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /Ob0 /I "./libavcodec" /I "./libavutil" /I "./" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "HAVE_AV_CONFIG_H" /D "CONFIG_MSVC" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ffmpeg - Win32 Release"
# Name "ffmpeg - Win32 Debug"
# Begin Group "libavcodec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libavcodec\4xm.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\8bps.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\aasc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\adpcm.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\adx.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\alac.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\allcodecs.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\asv1.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\avs.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\bitstream.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\bitstream_filter.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\bmp.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cabac.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cavs.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cavsdsp.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cinepak.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cljr.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cook.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cscd.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\cyuv.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dpcm.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dsicinav.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dsputil.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dvbsub.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dvbsubdec.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dvdsubdec.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\dvdsubenc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\error_resilience.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\faandct.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\fdctref.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\fft.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\flashsv.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\flashsvenc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\flicvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\fraps.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\g726.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\gif.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\gifdec.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\golomb.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h261.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h263.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h263dec.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h264.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h264dsp.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h264enc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\h264idct.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\huffyuv.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\idcinvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\imc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\indeo2.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\indeo3.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\interplayvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\jrevdct.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\kmvc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\lcl.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\loco.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\lzo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\lzw.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mace.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mdct.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mjpeg.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mmvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\motion_est.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mpeg12.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mpegaudio.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mpegaudiodec.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\mpegvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\msmpeg4.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\msrle.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\msvideo1.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\nuv.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\opt.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\parser.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\pcm.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\png.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\pnm.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\qdm2.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\qdrw.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\qpeg.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\qtrle.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\ra144.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\ra288.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\rangecoder.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\ratecontrol.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\raw.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\resample.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\resample2.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\roqvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\rpza.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\rtjpeg.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\rv10.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\shorten.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\simple_idct.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\smacker.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\smc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\sonic.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\svq1.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\targa.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\tiertexseqv.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\tiff.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\truemotion1.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\truemotion2.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\truespeech.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\tscc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\tta.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\ulti.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\utils.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vc1.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vc1dsp.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vcr1.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vmdav.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vmnc.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vorbis.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vorbis_data.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vp3.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vp3dsp.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vp5.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vp56.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vp56data.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vp6.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\vqavideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\wavpack.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\wmadec.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\wnv1.c
# End Source File
# Begin Source File

SOURCE=".\libavcodec\ws-snd1.c"
# End Source File
# Begin Source File

SOURCE=.\libavcodec\xan.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\xl.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\xvmcvideo.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\zmbv.c
# End Source File
# Begin Source File

SOURCE=.\libavcodec\zmbvenc.c
# End Source File
# End Group
# Begin Group "libavformat"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libavformat\4xm_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\adtsenc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\aiff.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\allformats.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\allformats.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\amr.c
# End Source File
# Begin Source File

SOURCE=".\libavformat\asf-enc.c"
# End Source File
# Begin Source File

SOURCE=.\libavformat\asf.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\asf.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\au.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\avformat.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\avi.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\avidec.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\avienc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\avio.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\avio.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\aviobuf.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\avisynth.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\avs_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\barpainet.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\base64.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\base64.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\crc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\cutils.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\daud.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\dsicin.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\electronicarts.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\file.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\flic.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\flv.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\flvdec.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\framehook.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\framehook.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\gif_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\gifdec_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\gxf.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\gxf.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\gxfenc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\idcin.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\idroq.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\img2.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\ipmovie.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\isom.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\isom.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\matroska.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mm.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mmf.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mov.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\movenc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mpc_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mpeg.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mpjpeg.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mtv.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\mxf.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\nsvdec.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\nuv_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\psxstr.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\qtpalette.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\raw_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\riff.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\riff.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\rm.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\segafilm.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\sierravmd.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\smacker_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\sol.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\swf.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\tiertexseq.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\tta_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\utils_fmt.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\voc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\voc.h
# End Source File
# Begin Source File

SOURCE=.\libavformat\vocdec.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\vocenc.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\wav.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\wc3movie.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\westwood.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\wv.c
# End Source File
# Begin Source File

SOURCE=.\libavformat\yuv4mpeg.c
# End Source File
# End Group
# Begin Group "libavutil"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libavutil\adler32.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\adler32.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\aes.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\aes.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\aes128.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\aes128.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\bswap.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\crc.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\crc_util.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\fifo.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\fifo.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\integer.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\integer.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\internal.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\intfloat_readwrite.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\intfloat_readwrite.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\intreadwrite.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\lls.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\lls.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\log.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\log.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\mathematics.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\mathematics.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\md5.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\md5.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\mem.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\rational.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\softfloat.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\tree.c
# End Source File
# Begin Source File

SOURCE=.\libavutil\tree.h
# End Source File
# Begin Source File

SOURCE=.\libavutil\x86_cpu.h
# End Source File
# End Group
# End Target
# End Project

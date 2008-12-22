<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: rlToday - Win32 (WCE emulator) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOKUME~1\andre\LOKALE~1\Temp\RSP406.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D "_i386_" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "_USRDLL" /FR"emulatorDbg/" /Fo"emulatorDbg/" /Fd"emulatorDbg/" /Gs8192 /GF /c 
"C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\rlToday.cpp"
]
Creating command line "cl.exe @C:\DOKUME~1\andre\LOKALE~1\Temp\RSP406.tmp" 
Creating temporary file "C:\DOKUME~1\andre\LOKALE~1\Temp\RSP407.tmp" with contents
[
aygshell.lib commdlg.lib commctrl.lib coredll.lib corelibc.lib /nologo /base:"0x00100000" /stack:0x10000,0x1000 /entry:"_DllMainCRTStartup" /dll /incremental:yes /pdb:"emulatorDbg/rlToday.pdb" /debug /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /def:".\rlToday.def" /out:"emulatorDbg/rlToday.dll" /implib:"emulatorDbg/rlToday.lib" /subsystem:windowsce,4.20 /MACHINE:IX86 
".\emulatorDbg\AnalogClock.obj"
".\emulatorDbg\ConvertUTF.obj"
".\emulatorDbg\Date.obj"
".\emulatorDbg\DigitalClock.obj"
".\emulatorDbg\Helper.obj"
".\emulatorDbg\Image.obj"
".\emulatorDbg\rlToday.obj"
".\emulatorDbg\Skin.obj"
".\emulatorDbg\Text.obj"
".\emulatorDbg\tinystr.obj"
".\emulatorDbg\tinyxml.obj"
".\emulatorDbg\tinyxmlerror.obj"
".\emulatorDbg\tinyxmlparser.obj"
".\emulatorDbg\rlToday.res"
]
Creating command line "link.exe @C:\DOKUME~1\andre\LOKALE~1\Temp\RSP407.tmp"
<h3>Output Window</h3>
Compiling...
rlToday.cpp
C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\rlToday.cpp(341) : warning C4101: 'lResult' : unreferenced local variable
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xstring(724) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xstring(720) : while compiling class-template member function 'void __thiscall std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >::_Copy(unsigned int)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xstring(724) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xstring(720) : while compiling class-template member function 'void __thiscall std::basic_string<unsigned short,struct std::char_traits<unsigned short>,class std::allocator<unsigned short> >::_Copy(unsigned int)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(30) : warning C4786: '?$pair@Viterator@?$_Tree@PBGU?$pair@QBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@std@@U_Kfn@?$map@PBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@2@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@std@@_N' : identifier was truncated to '255' characters in the browser information
        C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\SimpleIni.h(1410) : see reference to class template instantiation 'std::pair<class std::_Tree<unsigned short const *,struct std::pair<unsigned short const * const,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > >,struct std::map<unsigned short const *,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> >,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::_Kfn,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::iterator,bool>' being compiled
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xmemory(71) : while compiling class-template member function 'int __thiscall CSimpleIniTempl<unsigned short,struct SI_NoCase<unsigned short>,class SI_ConvertW<unsigned short> >::AddEntry(const unsigned short *,const unsigned short *,const unsigned short *,bool)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(34) : warning C4786: '??0?$pair@Viterator@?$_Tree@PBGU?$pair@QBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@std@@U_Kfn@?$map@PBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@2@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@std@@_N@std@@QAE@XZ' : identifier was truncated to '255' characters in the browser information
        C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\SimpleIni.h(1410) : see reference to class template instantiation 'std::pair<class std::_Tree<unsigned short const *,struct std::pair<unsigned short const * const,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > >,struct std::map<unsigned short const *,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> >,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::_Kfn,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::iterator,bool>' being compiled
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xmemory(71) : while compiling class-template member function 'int __thiscall CSimpleIniTempl<unsigned short,struct SI_NoCase<unsigned short>,class SI_ConvertW<unsigned short> >::AddEntry(const unsigned short *,const unsigned short *,const unsigned short *,bool)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(36) : warning C4786: '??0?$pair@Viterator@?$_Tree@PBGU?$pair@QBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@std@@U_Kfn@?$map@PBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@2@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@std@@_N@std@@QAE@ABViterator@?$_Tree@PBGU?$pair@QBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@std@@U_Kfn@?$map@PBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@2@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@1@AB_N@Z' : identifier was truncated to '255' characters in the browser information
        C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\SimpleIni.h(1410) : see reference to class template instantiation 'std::pair<class std::_Tree<unsigned short const *,struct std::pair<unsigned short const * const,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > >,struct std::map<unsigned short const *,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> >,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::_Kfn,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::iterator,bool>' being compiled
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xmemory(71) : while compiling class-template member function 'int __thiscall CSimpleIniTempl<unsigned short,struct SI_NoCase<unsigned short>,class SI_ConvertW<unsigned short> >::AddEntry(const unsigned short *,const unsigned short *,const unsigned short *,bool)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(38) : warning C4786: '??0?$pair@Viterator@?$_Tree@PBGU?$pair@QBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@std@@U_Kfn@?$map@PBGV?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@2@U?$SI_NoCase@G@@V?$allocator@V?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@std@@@2@@std@@_N@std@@QAE@ABU?$pair@$DCAB@$DCAC@@1@@Z' : identifier was truncated to '255' characters in the browser information
        C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\SimpleIni.h(1410) : see reference to class template instantiation 'std::pair<class std::_Tree<unsigned short const *,struct std::pair<unsigned short const * const,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > >,struct std::map<unsigned short const *,class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> >,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::_Kfn,struct SI_NoCase<unsigned short>,class std::allocator<class std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> > > >::iterator,bool>' being compiled
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xmemory(71) : while compiling class-template member function 'int __thiscall CSimpleIniTempl<unsigned short,struct SI_NoCase<unsigned short>,class SI_ConvertW<unsigned short> >::AddEntry(const unsigned short *,const unsigned short *,const unsigned short *,bool)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(36) : warning C4786: '??0?$pair@Viterator@?$_Tree@PBGU?$pair@QBGPBG@std@@U_Kfn@?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@2@U?$SI_NoCase@G@@V?$allocator@PBG@2@@std@@_N@std@@QAE@ABViterator@?$_Tree@PBGU?$pair@QBGPBG@std@@U_Kfn@?$map@PBGPBGU?$SI_NoCase@G@@V?$allocator@PBG@std@@@2@U?$SI_NoCase@G@@V?$allocator@PBG@2@@1@AB_N@Z' : identifier was truncated to '255' characters in the browser information
        C:\Dokumente und Einstellungen\andre\Eigene Dateien\Projects\rlToday\SimpleIni.h(1443) : see reference to class template instantiation 'std::pair<class std::_Tree<unsigned short const *,struct std::pair<unsigned short const * const,unsigned short const *>,struct std::map<unsigned short const *,unsigned short const *,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> >::_Kfn,struct SI_NoCase<unsigned short>,class std::allocator<unsigned short const *> >::iterator,bool>' being compiled
        C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\xmemory(71) : while compiling class-template member function 'int __thiscall CSimpleIniTempl<unsigned short,struct SI_NoCase<unsigned short>,class SI_ConvertW<unsigned short> >::AddEntry(const unsigned short *,const unsigned short *,const unsigned short *,bool)'
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(34) : warning C4786: 'std::pair<unsigned short const * const,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > >::pair<unsigned short const * const,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > >' : identifier was truncated to '255' characters in the debug information
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(34) : warning C4786: 'std::pair<std::_Tree<unsigned short const *,std::pair<unsigned short const * const,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > >,std::map<unsigned short const *,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> >,SI_NoCase<unsigned short>,std::allocator<std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > > >::_Kfn,SI_NoCase<unsigned short>,std::allocator<std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > > >::iterator,bool>::pair<std::_Tree<unsigned short const *,std::pair<unsigned short const * const,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > >,std::map<unsigned short const *,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> >,SI_NoCase<unsigned short>,std::allocator<std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > > >::_Kfn,SI_NoCase<unsigned short>,std::allocator<std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> > > >::iterator,bool>' : identifier was truncated to '255' characters in the debug information
C:\Programme\Windows CE Tools\wce420\POCKET PC 2003\Include\emulator\utility(34) : warning C4786: 'std::pair<std::_Tree<unsigned short const *,std::pair<unsigned short const * const,unsigned short const *>,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> >::_Kfn,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> >::iterator,bool>::pair<std::_Tree<unsigned short const *,std::pair<unsigned short const * const,unsigned short const *>,std::map<unsigned short const *,unsigned short const *,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> >::_Kfn,SI_NoCase<unsigned short>,std::allocator<unsigned short const *> >::iterator,bool>' : identifier was truncated to '255' characters in the debug information
Linking...
   Creating library emulatorDbg/rlToday.lib and object emulatorDbg/rlToday.exp
rlToday.exp : warning LNK4070: /OUT:rlCalendar.dll directive in .EXP differs from output filename 'emulatorDbg/rlToday.dll'; ignoring directive
Creating temporary file "C:\DOKUME~1\andre\LOKALE~1\Temp\RSP40D.tmp" with contents
[
/nologo /o"emulatorDbg/rlToday.bsc" 
".\emulatorDbg\AnalogClock.sbr"
".\emulatorDbg\ConvertUTF.sbr"
".\emulatorDbg\Date.sbr"
".\emulatorDbg\DigitalClock.sbr"
".\emulatorDbg\Helper.sbr"
".\emulatorDbg\Image.sbr"
".\emulatorDbg\rlToday.sbr"
".\emulatorDbg\Skin.sbr"
".\emulatorDbg\Text.sbr"
".\emulatorDbg\tinystr.sbr"
".\emulatorDbg\tinyxml.sbr"
".\emulatorDbg\tinyxmlerror.sbr"
".\emulatorDbg\tinyxmlparser.sbr"]
Creating command line "bscmake.exe @C:\DOKUME~1\andre\LOKALE~1\Temp\RSP40D.tmp"
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
rlToday.dll - 0 error(s), 12 warning(s)
</pre>
</body>
</html>

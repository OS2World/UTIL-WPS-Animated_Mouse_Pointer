.SUFFIXES: .c .obj .dll .idl .h .ih .rc .res .iph .hlp .src .ipf .inf

DEBUG = 1

WPSCUR       = 0

SC           = sc
SCFLAGS      = -maddstar -S128000 -C128000 -mnoint -v
SCLIST       = -s"ih;h;c;def"

HC           = ipfc

WIC          = wic

RC           = rc

CC           = icc
CFLAGS_ALL   = -Sp4 -Ss+ -C+ -Q+ -Gm+ -Gl+ -DWPSCUR=$(WPSCUR)
CFLAGS_STD   = $(CFLAGS) -W3 -Wuse+ -Wcnd-
CFLAGS_EXE   = -Ge+
CFLAGS_DLL   = -Ge-
CFLAGS_SOM   = $(CFLAGS) -W1

LINK         = ilink
LFLAGS_ALL   = /MAP /NOL /NOI /PACKCODE /PACKDATA
LFLAGS_DLL   = /DLL
LFLAGS_EXE   = /STACK:0x100000 /EXE /PM:PM

!ifdef DEBUG
CFLAGS      = $(CFLAGS_ALL) -Ti+ -O- -DDEBUG=1
LFLAGS      = $(LFLAGS_ALL) /DEB
PRINTF      = printf.obj
!else
CFLAGS      = $(CFLAGS_ALL) -Ti- -O+
LFLAGS      = $(LFLAGS_ALL) /OPTF
!endif

!if $(WPSCUR) == 1
OBJS_SOM     = sominit.obj wpwcur.obj wpwani.obj wpand.obj wpoptr.obj
DLLS_DEF     = wpampdll_wpscur.def
!else
OBJS_SOM     = sominit.obj
DLLS_DEF     = wpampdll.def
!endif

OBJS_CONVERT = cursor.obj dll.obj dllbin.obj pointer.obj script.obj eas.obj
OBJS_ANIM    = mptranim.obj mptrcnr.obj mptredit.obj mptrlset.obj mptrpag1.obj mptrppl.obj mptrprop.obj mptrptr.obj mptrset.obj mptrutil.obj mptrfile.obj nls.obj

MHEADER      = macros.h mmacros.h tmacros.h fmacros.h
HHEADER      = mptrhook.h
HEADERS      = $(MHEADER) title.h cursor.h dll.h eas.h info.h mptranim.h mptredit.h mptrfile.h script.h wpamptr.rch eas.h

NLS          = amptreng.dll amptrdeu.dll wpamptrs.dll
HLP          = amptreng.hlp amptrdeu.hlp readme.inf

all:      wpamptr.dll wpamptrh.dll $(NLS) $(HLP) aniedit2.exe wpamptr.wpi

viewhelp: $(HLP)
  start viewhelp $(HLP)

wpamptrs.dll: amptreng.dll
  if exist wpamptrs.dll del wpamptrs.dll
  copy amptreng.dll wpamptrs.dll

wpamptrh.dll: mptrhook.obj wpamptrh.def
  $(LINK) /NOFREE $(LFLAGS) $(LFLAGS_DLL) @<<
  mptrhook.obj
  wpamptrh.dll
  wpamptrh.map

  wpamptrh.def
<<

wpamptrh.lib: wpamptrh.dll
  @implib /NOLOGO wpamptrh.lib wpamptrh.dll

mptrhook.obj: mptrhook.c wmuser.h
  $(CC) $(CFLAGS_STD) $(CFLAGS_DLL) $*.c

wpamptr.dll: wpamptr.obj wpamptrh.lib $(OBJS_SOM) $(OBJS_ANIM) $(OBJS_CONVERT) $(PRINTF) wpamptr.def
  $(LINK) /NOFREE $(LFLAGS) $(LFLAGS_DLL) @<<
  $*.obj $(OBJS_SOM) $(OBJS_ANIM) $(OBJS_CONVERT) $(PRINTF)
  wpamptr.dll
  wpamptr.map
  somtk.lib wpamptrh.lib ecolange.lib
  $(DLLS_DEF)
<<

wpamptr.lib: wpamptr.dll
  @implib /NOLOGO wpamptr.lib wpamptr.dll

!if $(WPSCUR) == 1
sominit.obj: sominit.c wpamptr.h wpwcur.h
!else
sominit.obj: sominit.c wpamptr.h
!endif
  $(CC) $(CFLAGS_STD) $(CFLAGS_DLL) $*.c


wpamptr.obj: wpamptr.c wpamptr.h wpamptr.ih wpamptr.rch $(HEADERS)
  $(CC) $(CFLAGS_SOM) $(CFLAGS_DLL) $*.c

wpamptr.h:   wpamptr.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl
wpamptr.ih:  wpamptr.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl
wpamptr.c:   wpamptr.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl
wpamptr.def: wpamptr.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl

wpwcur.obj: wpwcur.ih wpwcur.c wpwcur.h wpamptr.rch $(HEADERS)
  $(CC) $(CFLAGS_SOM) $(CFLAGS_DLL) $*.c

wpwcur.ih:  wpwcur.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl
wpwcur.c:   wpwcur.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl
wpwcur.h:   wpwcur.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl
wpwcur.def: wpwcur.idl
  $(SC) $(SCFLAGS) $(SCLIST) $(@B).idl

wpwani.obj: wpwani.ih wpwani.c wpwani.h wpamptr.rch $(HEADERS)
  $(CC) $(CFLAGS_SOM) $(CFLAGS_DLL) $*.c

wpwani.ih:  wpwcur.ih
  sed -f wpwani.sed < $** > $@
wpwani.c:   wpwcur.c
  sed -f wpwani.sed < $** > $@
wpwani.h:   wpwcur.h
  sed -f wpwani.sed < $** > $@
wpwani.def: wpwcur.def
  sed -f wpwani.sed < $** > $@

wpand.obj: wpand.ih wpand.c wpand.h wpamptr.rch $(HEADERS)
  $(CC) $(CFLAGS_SOM) $(CFLAGS_DLL) $*.c

wpand.ih:  wpwcur.ih
  sed -f wpand.sed < $** > $@
wpand.c:   wpwcur.c
  sed -f wpand.sed < $** > $@
wpand.h:   wpwcur.h
  sed -f wpand.sed < $** > $@
wpand.def: wpwcur.def
  sed -f wpand.sed < $** > $@

wpoptr.obj: wpoptr.ih wpoptr.c wpoptr.h wpamptr.rch $(HEADERS)
  $(CC) $(CFLAGS_SOM) $(CFLAGS_DLL) $*.c

wpoptr.ih:  wpwcur.ih
  sed -f wpoptr.sed < $** > $@
wpoptr.c:   wpwcur.c
  sed -f wpoptr.sed < $** > $@
wpoptr.h:   wpwcur.h
  sed -f wpoptr.sed < $** > $@
wpoptr.def: wpwcur.def
  sed -f wpoptr.sed < $** > $@

aniedit2.exe: aniedit2.obj aniedit2.def aniedit2.res wpamptr.lib
  $(LINK) /NOFREE $(LFLAGS) $(LFLAGS_EXE) @<<
  $*.obj $(PRINTF)
  aniedit2.exe
  aniedit2.map
  wpamptr.lib
  aniedit2.def
<<
  @$(RC) -n -x2 $*.res $@

aniedit2.obj: aniedit2.c title.h
  $(CC) $(CFLAGS_STD) $(CFLAGS_EXE) $*.c

aniedit2.res: aniedit2.rc wpamptr.rch
  $(RC) -n -r $*.rc

mptrcnr.obj: mptrcnr.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptredit.obj: mptredit.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptrpag1.obj: mptrpag1.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptrppl.obj: mptrppl.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptrset.obj: mptrset.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptranim.obj: mptranim.c $(HEADERS) $(HHEADER)
  $(CC) $(CFLAGS_STD) $*.c
mptrptr.obj: mptrptr.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptrutil.obj: mptrutil.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptrfile.obj: mptrfile.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
mptrlset.obj: mptrlset.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
cursor.obj: cursor.c cursor.h $(MHEADER)
  $(CC) $(CFLAGS_STD) $*.c
pointer.obj: pointer.c pointer.h $(MHEADER)
  $(CC) $(CFLAGS_STD) $*.c
dll.obj: dll.c dll.h lx.h mptrppl.h $(MHEADER)
  $(CC) $(CFLAGS_STD) $*.c
script.obj: script.c script.h mptrppl.h $(MHEADER)
  $(CC) $(CFLAGS_STD) $*.c
mptrprop.obj: mptrprop.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c
eas.obj: eas.c eas.h
  $(CC) $(CFLAGS_STD) $*.c
printf.obj: printf.c $(HEADERS)
  $(CC) $(CFLAGS_STD) $*.c

dllbin.obj: and\template.dll
  tools\xbin2obj -n and\template.dll -cCONST bindataDllTemplate dllbin.obj

wpamptrs.obj: wpamptrs.c
  $(CC) $(CFLAGS_STD) $*.c

amptreng.dll: amptreng.res wpamptrs.def wpamptrs.obj
  $(LINK) /NOFREE /DLL $(LFLAGS) wpamptrs.obj,$*.dll,,,wpamptrs.def
  $(RC) -n -x2 $*.res $@

amptrdeu.dll: amptrdeu.res wpamptrs.def wpamptrs.obj
  $(LINK) /NOFREE /DLL $(LFLAGS) wpamptrs.obj,$*.dll,,,wpamptrs.def
  $(RC) -n -x2 $*.res $@

amptreng.res: nls\amptreng.rc nls\amptreng.rch nls\amptreng.dlg wpamptr.dlg wpamptr.rch wpamptr.rc title.h about.ico
  $(RC) -n -r -i nls nls\amptreng.rc amptreng.res
amptrdeu.res: nls\amptrdeu.rc nls\amptrdeu.rch nls\amptrdeu.dlg wpamptr.dlg wpamptr.rch wpamptr.rc title.h about.ico
  $(RC) -n -r -i nls nls\amptrdeu.rc amptrdeu.res

amptreng.hlp: amptreng.iph
  $(HC) /L:eng $*.iph $*.hlp
amptreng.iph: nls\amptreng.ips nls\amptreng.rch nls\macros.iph wpamptr.rch title.h about.bmp
  tools\ipfcprep -L nls\amptreng.ips amptreng.iph -Inls -DHLP

amptrdeu.hlp: amptrdeu.iph
  $(HC) /L:deu $*.iph $*.hlp
amptrdeu.iph: nls\amptrdeu.ips nls\amptrdeu.rch nls\macros.iph wpamptr.rch title.h about.bmp
  tools\ipfcprep -L nls\amptrdeu.ips amptrdeu.iph -Inls -DHLP

readme.inf: readme.iph
  $(HC) /INF $*.iph $*.inf

readme.iph: readme.ips title.h
  tools\ipfcprep -L $*.ips $*.iph -Inls -DINF

wpamptr.wpi: wpamptr.dll wpamptrh.dll $(NLS) $(HLP) aniedit2.exe wpamptr.wis file_id.diz whatsnew.txt
  $(WIC) -a $*.wpi 1 wpamptrh.dll 2 wpamptr.dll wpamptrs.dll *.inf wpamptr*.hlp *.exe file_id.diz whatsnew.txt 3 -cpointers -r * 4 -cnls *.nls -s  $*.wis

clean:
    -@del *.obj *.res *.map *.exe *.dll *.lib    > nul
    -@del wpamptr.h wpamptr.ih wpamptr.def       > nul
    -@del wpwcur.h wpwcur.ih wpwcur.def          > nul
    -@del wpwani.h wpwani.ih wpwani.def wpwani.c > nul
    -@del wpand.h wpand.ih wpand.def wpand.c     > nul
    -@del wpoptr.h wpoptr.ih wpoptr.def wpoptr.c > nul
    -@del *.iph *.hlp *.inf                      > nul

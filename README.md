***AlfaCAD was conceived as a simple, universal program for creating technical drawings. However, "universal" does not mean "almighty". It is a Swiss Army knife rather than a box full of tools most of which are a secret to the user and probably will never be used.***

<p align="center" width="100%">
  <img width="25%" src="alfalogosaktrans2.png" />
</p>

AlfaCAD was designed as a program for creating any drawing, but mostly engineering drawings in virtually any industry, which are limited rather by access to an existing or user-created block library for fast drawing creation, than the set of functions available in the program.
This approach has changed significantly in version 2.6, where a new feature called "Applications" has been implemented, with the first item on the list being the "Static and Dynamic Analysis" feature.

*Following this path,AlfaCAD has been gradually equipped with tools and modules for static and dynamic analysis of frames, trusses and beam grids, as well as static analysis of plates and shields (shells) using the finite element method (FEM), complementing its capabilities for creating any type of technical drawings with very specialized calculations and a way of presenting their results.*

*Recently, in version 3.0, the "Profiles" application continues this development path. It is a suite of functions that enables quick and relatively easy development of utility profile drawings. It plots essential elements such as ground levels, road elevations, pipe crowns/inverts, utility cross-sections, and geological borehole logs.*

*There is also the "Utility Map" interface containing tools for tracing utility alignments over georeferenced backgrounds.*

***Compilation of AlfaCAD v. 3.x and all necessary libraries***

***AlfaCAD***<br>
***Linux***

From AlfaCAD-CC2.zip file extract CC2 folder to the main home folder, or any other folder which from now on we will call “CC2 root folder”.

CC2 stands for “Cooking CAD too”. We are cooking CAD.

From AlfaCAD-Linux-Pack.tar extract all zip files to any temporary folder, then extract each of them to the folder where CC2 root folder was created. So eventually you will get such structure of folders:

allegro5-4.4.3<br>
CavalierContours<br>
CC2<br>
clip<br>
Frame3DDa<br>
glyph-keeper<br>
kdialog-master<br>
libharu-master<br>
libpng1637<br>
libredwg-master<br>
potrace-1.16<br>
zlib

***Compiling AlfaCAD main program*** (AlfaCAD, AlfaCADPL, AlfaCADUA, AlfaCADES)

CC2/Source contains CmakeLists.txt file.<br>
This file is used by cmake to compile the entire program.<br>
So install cmake of your Linux, you will need it anyway.<br>
The author is using CLion IDE by Jetbrains. CLion is a commercial tool, but is cheap (a few USD per month) and it’s the best IDE I’ve ever seen. However, you can use any IDE of your choice.<br>
AlfaCAD should be compiled using cland and clang++ compilers, that’s for compatibility with Windows, which is also using that implementation of C and C++ as default in Visual Studio 2022. This is what I use for AlfaCAD compilation in Windows.

Use CC2/Source/***CmakeLists.txt*** file as a project configuration file.

CC2/Source/***CmakeLists.txt*** contains 8 compilation setups:

(English version)

- AlfaCAD Debug
- AlfaCAD Release

(Polish version)

- AlfaCADPL Debug
- AlfaCADPL Release

(Ukrainian version)

- AlfaCADUA Debug
- AlfaCADUA Release

(Spanish version)

- AlfaCADES Debug
- AlfaCADES Release

If successively compiled, binary file will be stored in folder accordingly to the setup :<br>
CC2/Source/cmake-build-debug<br>
CC2/Source/cmake-build-release<br>
CC2/Source/cmake-build-debug\_pl<br>
CC2/Source/cmake-build-release\_pl<br>
and so on.

You probably can compile it straight away, without re-compiling another libraries, because necessary static or dynamic link libraries were left in associated folders.<br>
However, you will also need the following third-party or system libraries:

- X11
- Xrandr
- ncurses
- Xm
- Xt
- fltk
- cups
- xcb
- ruby
- curl
- "/usr/lib/libQt5Core.so"
- "/usr/lib/libQt5Widgets.so"
- "/usr/lib/libQt5Gui.so"
- "/usr/lib/libQt5InputSupport.a"
- "/usr/lib/libQt5PrintSupport.so"
- "/usr/lib/libQt5Positioning.so"
- "/usr/lib/libKF5WidgetsAddons.so"
- "/usr/lib/libKF5TextWidgets.so"
- "/usr/lib/libKF5I18n.so"

Please install necessary packages, if you have not installed them together with entire system. Next to those libraries, the following ones will be linked too:

clip/cmake-build-release/libclip.a<br>
glyph-keeper/obj/libglyph-allegs.a<br>
libharu-master/cmake-build-release/src/libhpdfs.a<br>
libpng1637/cmake-build-release/libpng.a<br>
allegro5-4.4.3/lib/liballeg.so  //default<br>
allegro5-4.4.3/lib/liballeg.a  //optional if you like<br>
allegro5-4.4.3/lib/libjpgalleg.so  //default<br>
allegro5-4.4.3/lib/libjpgalleg.a //optional if you like<br>
zlib/libz.a

Position of those folders has to be parallel to CC2 folder, due to the setup in CmakeLists.txt:<br>
"${CMAKE\_CURRENT\_SOURCE\_DIR}/../../clip/cmake-build-release/libclip.a"<br>
and so on.

***AlfaCAD***<br>
***Windows***

From AlfaCAD-CC2.zip file extract CC2 folder to the main home folder, or any other folder which from now we will call “CC2 root folder”.

CC2 stands for “Cooking CAD 2”, where 2 is just the version. We are cooking CAD.

From AlfaCAD-Windows-Pack.tar extract all zip files to any temporary folder using command:<br>
tar -xvzf AlfaCAD-Windsows-Pack.tar -C New-Location<br>
where New-Location has to be replaced with any temporary folder name, then each of them extract to the folder where CC2 root folder was created, so eventually you will get such structure of folders:

allegro-4.4.3.1<br>
allegro5-4.4.4<br>
CavalierContours<br>
CC2<br>
Frame3DDa<br>
glyph-keeper<br>
libharu-master<br>
libpng1637<br>
potrace-1.16<br>
pyw<br>
qt5<br>
zlib

libredwg-master does not exist in the Windows version. This folder contains source to DWG/DXF conversion, and because it comes to AlfaCAD without any change, it’s easier to upgrade those tools directly from <https://www.gnu.org/software/libredwg/>

AlfaCAD installation packages contain dwg2dxf.exe and dxf2dwg.exe programs for both 32 and 64bit versions.

***Compiling AlfaCAD main program*** (AlfaCAD4Win, AlfaCAD4Win64, AlfaCADPL4Win, AlfaCADPL4Win64, AlfaCADUA4Win, AlfaCADUA4Win64, AlfaCADES4Win, AlfaCADES4Win64)

CC2 contains VSProjects folder which contains 4 subfolders:<br>
AlfaCAD32-master<br>
AlfaCAD64-master<br>
AlfObjects32<br>
AlfObjects64

each of them contains \*.sln file which is Visual Studio 2022 solution so:

- AlfObjects64 folder contains project files for auxiliary DLL file for AlfaCAD 64bit
- AlfObjects32 folder contains project files for auxiliary DLL file for AlfaCAD 32bit

AlfaCAD needs auxiliary module AlfaObjects.lib (library) and AlfaObjects.dll (dynamic-link library). This library is necessary for inter-process operations in case multiple AlfaCAD instances are working at the same time, avoiding unnecessary multiplying instances in case of opening program as associated with specific file type. It also supports drag-and-drop process.

It is not necessary to compile those projects, library files already exist there.

- AlfaCAD64-master folder contains main AlfaCAD project for AlfaCADxx4Win64.exe<br>
as 64bit application where xx is language version, so none-English, PL-Polish, UA-Ukrainian, ES-Spanish

- AlfaCAD32-master folder contains main AlfaCAD project for AlfaCADxx4Win.exe<br>
as 32bit application where xx is language version, so none-English, PL-Polish, UA-Ukrainian, ES-Spanish

AlfaCAD64 and AlfaCAD64 solutions contain 8 compilation profiles each:

- Debug
- Release
- Debug\_PL
- Release\_PL
- Debug\_UA
- Release\_UA
- Debug\_ES
- Release\_ES

In the project AlfaCAD64 the target should be selected as x64, for AlfaCAD32 as x86 or Win32

Each produces binary file in and is storing in folder:

- Debug
- Release
- DebugPL
- ReleasePL      and so on

You can probably compile it straight away, without re-compiling another libraries, because necessary static or dynamic link libraries were left in associated folders.

However, you will also need the following third-party or system libraries, which are always available in Windows Software Development Kit. [https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) Please install it.

Next to those libraries, the following ones will be linked too:

allegro-4.4.3.1\allegro\build\lib\RelWithDebInfo\alleg44.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
allegro5-4.4.4\build\lib\RelWithDebInfo\alleg44.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit

allegro-4.4.3.1\allegro\build\lib\RelWithDebInfo\jpgalleg.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
allegro5-4.4.4\build\lib\RelWithDebInfo\jpgalleg.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit<br>

..\AlfObjects64\Release64\AlfaObjects.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
..\AlfObjects32\Release\AlfaObjects.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit<br>

glyph-keeper\_v2\obj\glyph-allegs.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
glyph-keeper32\obj\glyph-alleg32.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit<br>
glyph-keeper64\obj\glyph-allegs.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code> 64bit – alternative

libharu-master\src\build\Release\hpdf.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
libharu-master\src\build32\Release\hpdf.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit

zlib\build\Release\zlibstatic.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
zlib\build32\Release\zlibstatic.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit

lpng1637\projects\vstudio\x64\Release\libpng16.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
lpng1637\projects\vstudio32\Release\libpng16.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit

qt5\qt5-build\qtbase\lib\Qt5Core.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
qt5\qt5-build\qtbase\lib\Qt5Gui.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
qt5\qt5-build\qtbase\lib\Qt53DCore.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit

qt5\qt5-build32\qtbase\lib\Qt5Core.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit<br>
qt5\qt5-build32\qtbase\lib\Qt5Gui.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit<br>
qt5\qt5-build32\qtbase\lib\Qt53DCore.lib<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit

If anyone has problems with linking provided library files, or wants to modify them, for any reason, here is the recipe how to do that:

***AlfaCAD***<br>
***MacOS***

Details of compilation under MacOS will be described soon, as there are differences compared to the Linux version.

***Allegro***

Allegro is a cross-platform library mainly aimed at video game and multimedia programming. It handles common, low-level tasks such as creating windows, accepting user input, loading data, drawing images, playing sounds, etc. and generally abstracting away the underlying platform. However, Allegro is not a game engine: you are free to design and structure your program as you like. For example CAD programs. Allegro was originally created by Shawn Hargreaves as early as 1990 for Atari ST, then migrated trough DJGPP compilers in 1995 in MS-DOS, to eventually be ported in Windows, Unix and Linux.

There are slightly different versions of Allegro 4 used for Linux, Windows 64bit and Windows 32bit versions of AlfaCAD.

***allegro5-4.4.3*** folder represents the best Linux version of the library, with some extensions within X11 part of it made by AlfaCAD author, mostly concerning the screen for multi-monitor setup and window resizing, as well as keyboard extensions to support UTF8.

***allegro-4.4.3.1*** folder represents the unique Windows 64bit version of the library, and is the only 64bit Windows version ever existed, developed by AlfaCAD author, for AlfaCAD creation purpose. There is a number of changes, also extensions in GDI module concerning the screen for multi-monitor setup and window resizing, drag-and-drop support, as well as keyboard extensions to support UTF8.

***allegro5-4.4.4*** folder represents the standard Allegro version of the newest possible edition within version 4, however it doesn’t support 64bit binary format, so it’s used for 32bit Windows version of AlfaCAD. There are similar extensions in GDI module concerning the screen for multi-monitor setup and window resizing, drag-and-drop support, as well as keyboard extensions to support UTF8.

AlfaCAD needs two of library files produced within entire Allegro project: for Linux:

- liballeg.a
- libjpgalleg.a

for Windows:

- alleg44.lib
- jpgalleg.lib


Those libraries are dynamically linked with AlfaCAD, so next to \*.lib there are shared library for Linux:<br>
liballeg.so.4.4.3<br>
liballeg.so.4.4 (alias to liballeg.so.4.4.3)<br>
liballeg.so (alias to liballeg.so.4.4)

and dynamic-link library for Windows:<br>
alleg44.dll<br>
jpgalleg.dll<br>
respectively for 64 and 32bit versions

***Linux compilation***

Compilation is based on cmake with allegro5-4.4.3/CmakeCache.txt configuration file. It’s enough to compile allegro Release and jpgalleg Release profiles to get all what is needed.<br>
Produced \*.so files should be copied to the main AlfaCAD folder, if re-compiled after any changes. \*.a files will be used by AlfaCAD project, and they should stay where they are.

***Windows compilation***

There are VS2022 projects:<br>
allegro-4.4.3.1\allegro\build\ALLEGRO.sln"<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>64bit<br>
only two project have to be compiled:<br>
allegro Release or RelWithDebInfo   x64
jpgalleg Release or RelWithDebInfo   x64

it will produce:<br>
allegro-4.4.3.1\allegro\build\lib\RelWithDebInfo\alleg44.lib 64bit<br>
allegro-4.4.3.1\allegro\build\lib\RelWithDebInfo\jpgalleg.lib 64bit<br>
and<br>
allegro-4.4.3.1\allegro\build\lib\RelWithDebInfo\alleg44.dll 64bit<br>
allegro-4.4.3.1\allegro\build\lib\RelWithDebInfo\jpgalleg.dll 64bit

allegro5-4.4.4\build\ALLEGRO.sln"<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>32bit<br>
allegro RelWithDebInfo  Win32<br>
jpgalleg RelWithDebInfo  Win32

it will produce:<br>
allegro5-4.4.4\build\lib\RelWithDebInfo\alleg44.lib 32bit<br>
allegro5-4.4.4\build\lib\RelWithDebInfo\jpgalleg.lib 32bit<br>
and<br>
allegro5-4.4.4\build\lib\RelWithDebInfo\alleg44.dll 32bit<br>
allegro5-4.4.4\build\lib\RelWithDebInfo\jpgalleg.dll 32bit

***MacOS compilation***

*Details of compilation under MacOS will be described soon, as there are differences compared to the Linux version.*

***glyph-keeper***

glyph-keeper is a library created by Kirill Kryukov, which helped to brought freetype (True Type) fonts to AlfaCAD. I’m glad I found his work to use it. This library was slightly modified by AlfaCAD author, as well as tested with newer FreeType library.

There are 1 version for Linux and 3 versions for Windows of that library. 

In Linux:<br>
glyph-keeper folder

In Windows:<br>
glyph-keeper\_v2  little bit experimental version, based on upgraded version of freetype<br>
glyph-keeper32 32bit standard library<br>
glyph-keeper64  64bit standard library

In current configuration glyph-keeper\_v2 is selected for 64bit AlfaCAD and glyph-keeper32 for 32bit version of it.

Use make for Linux,<br>
make /f Makefile.GNU.all TARGET=ALLEGRO ALLEGRO\_STATICLINK=1

and  MinGW  for Windows (follow the instruction)<br>
nmake /f Makefile.MSVC71 TARGET=ALLEGRO ALLEGRO\_STATICLINK=1

64bit:<br>
folder glyph-keeper64  or glyph-keeper\_v2

32bit:<br>
folder glyph-keeper32

you will get glyph-allegs.lib<br>
which is statically linked with AlfaCAD

***libharu***

libharu is a library by Takeshi Kanno and Antony Dovgal for generating  PDF.<br>
It is used to print drawings into PDF format as well as to present the results of static and dynamic analysis.

***Linux:***<br>
There is CmakeLists.txt in libharu-master folder, which is a project configuration file for cmake, or, preferably Clion IDE. There are two profiles: Debug, Release.<br>
They will produce statically linking library, such:<br>
/home/marek/libharu-master/cmake-build-release/src/libhpdfs.a

Just do it.

***Windows:***<br>
There is a folder libharu-master<br>
64bit VS project:<br>
"libharu-master\src\build\Project.sln"<br>
If you select Release x64, you will get:<br>
"libharu-master\src\build\Release\hpdf.lib"

32bit VS project:<br>
"libharu-master\src\build32\Project.sln"<br>
If you select Release Win32, you will get:<br>
"libharu-master\src\build32\Release\hpdf.lib"

***qt5***

This library is necessary for quadrangular transformation within AlfaCAD, for very specific function, mostly used for insulation hatching transformation, but not only.

***Linux***

Just install Qt5

those libraries will be used:

- "/usr/lib/libQt5Core.so"
- "/usr/lib/libQt5Widgets.so"
- "/usr/lib/libQt5Gui.so"
- "/usr/lib/libQt5InputSupport.a"
- "/usr/lib/libQt5PrintSupport.so"
- "/usr/lib/libQt5Positioning.so"

***Windows***

there is qt5 folder within Windows-pack, minimal package, with all the necessary header files and libraries. It is not necessary to install Qt5 on Windows and it is a huge package unless you want to make significant changes to AlfaCAD using that library.

***zlib***

This is zipping library, both for Windows and Linux.<br>
The library will be statically linked with AlfaCAD.

***Linux:***

follow the instruction to get:      zlib/libz.a

***Windows:***<br>
for 64bit:<br>
VS solution: zlib\build\zlib.sln<br>
zlibstatic Release x64<br>
you will get:<br>
zlib\build\Release\zlibstatic.lib

for 32bit:<br>
VS solution: zlib\build32\zlib.sln<br>
zlibstatic Release Win32<br>
you will get:<br>
zlib\build32\Release\zlibstatic.lib

***libpng1637***

PNG Reference Library developed [https://sourceforge.net/projects/libpng/files/libpng16/1.6.37/](https://sourceforge.net/projects/libpng/files/libpng16/1.6.37/) by bunch of authors listed in AUTHORS.md link on page. Please read LICENCE file for copyright notice, disclaimer, and license.

***Linux***<br>
There is cmake/CLion configuration file: libpng1637/CmakeLists.txt<br>
Compilation profile is png-static Release

You will get:<br>
libpng1637/cmake-build-release/libpng.a

***Windows***<br>
64bit:<br>
VS solution: "lpng1637\projects\vstudio\vstudio.sln"

Project: zlib Release x64<br>
and libpng Release x64<br>
You will get:<br>
"lpng1637\projects\vstudio\x64\Release\libpng16.lib"<br>
"lpng1637\projects\vstudio\x64\Release\libpng16.dll"

32bit:<br>
VS solution: "lpng1637\projects\vstudio32\vstudio.sln"

Project: zip Release x86<br>
and: libpng Release x86<br>
You will get:<br>
"lpng1637\projects\vstudio32\Win32\_DLL\_Release\libpng16.lib"<br>
copy to:<br>
"lpng1637\projects\vstudio32\Release\libpng16.lib"
and:<br>
"lpng1637\projects\vstudio32\Win32\_DLL\_Release\libpng16.dll"<br>
copy to:<br>
"lpng1637\projects\vstudio32\Release\libpng16.dll"

***CavalierContours***

CavalierContours is a small library developed by jbuckmccready <https://github.com/jbuckmccready/CavalierContours>

for processing 2D polylines containing both straight line and constant radius arc segments. This C++ library is not being actively developed. Development is continuing in Rust. Library is used in AlfaCAD to support Edit-Offset function for polylines.

It doesn’t need to be compiled, source files are included in AlfaCAD project.

***clip***

This is a library developed by David Capello, Copyright (c) 2015-2022, to copy/retrieve content to/from the clipboard/pasteboard.

This library is used exclusively in Linux version of AlfaCAD.

There is clip/CmakeLists.txt file for cmake (or CLion as my choice) and with selected profile:<br>
clip Release <br>
It will produce clip/cmake-build-release/libclip.a

***kdialog4alfa***

This is a supporting program used exclusively in Linux version. Like original kdialog [https://develop.kde.org/docs/administration/kdialog/](https://develop.kde.org/docs/administration/kdialog/), program created by Matthias Hoelzer 1998 and other authors among them the last maintainer Tristan Miller 2020 and published under the GPL-2.0 License.

The version elaborated by AlfaCAD author is extending dialog boxes with special one for creating single line or multiple lines text, with several attributes, as a part of AlfaCAD, however called as separate program, to use advanced text manipulations, based on Qt library.

The compilation is straight forward:

in   kdialog-master/build  folder run:   

make clean

make

it will produce: kdialog-master/build/bin/kdialog4alfa

Please do not overwrite  kdialog-master folder with newer kdialog version. It will not work with AlfaCAD. Those files were altered: widgets.cpp, kdialog.cpp, NcFramelessHelper.cpp, NcFramelessHelper.h, widgets.h, utils.cpp, utils.h

***libredwg***

This is an open source program used in its original version to read DWG file (native Autocad drawing files):  <https://www.gnu.org/software/libredwg/>

Because libredwg is used in its original shape, it doesn’t convert DWG files to AlfaCAD files, this task is still front of AlfaCAD author, instead is converting DWG to DXF exchangeable format, then AlfaCAD handles reading DXF like is doing that while importing DXF files.

***Linux version:***

libredwg-master/CmakeLists.txt   is a configuration file for cmake (or CLion as a IDE of my choice).<br>
Choosing:  dwg2dxf Release

produces:  libredwg-master/cmake-build-release/dwg2dxf

Choosing:  dxf2dwg Release 

produces:  libredwg-master/cmake-build-release/dxf2dwg

it’s also producing big share library file:  libredwg-master copy/cmake-build-release/libredwg.so which must be included in AlfaCAD folder to convert drawings.

***Windows version:***

Because pre-compiled versions exist and they are continuously under development, it’s easier to download them directly, both 64 and 32 bit versions from here: <https://github.com/LibreDWG/libredwg/releases>

LibreDWG is constantly being developed. There is no reason not to use a newer version of this program, so the newer version dwg2dxf (or dwg2dxf.exe for Windows) can successfully replace the version provided in the installation package. LibreDWG is very well made, it reads DWG from many versions of Autocad and other CAD programs that use the DWG format. It should be remembered that the DWG format specification is not published, so the development process of the program is practically a so- called "reverse engineering".


***potrace-1.16***

Potrace is a tool for tracing a bitmap, which means, transforming a bitmap into a smooth, scalable image.<br>
Written by Peter Selinger, Copyright (C) 2001-2019, was extended by AlfaCAD author by a modul which generate a native AlfaCAD blok, to insert it into a drawing as a vector block, containing polylines (lines and arcs) and splines.<br>
backend_alx.h and backend_alx.files were added, main.c file was modified.

***Linux***<br>
in folder potrace-1.16:<br>
make clean<br>
make

It will produce:<br>
potrace-1.16/src/potrace   and<br>
potrace-1.16/src/mkbitmap

***Windows***<br>
VS solution: potrace-1.16\potrace.sln

64bit:<br>
project: potrace Release_64  x64<br>
produces:<br>
potrace-1.16\x64\Release_64\potrace.exe<br>
potrace-1.16\x64\Release_64\mkbitmap.exe<code>&nbsp;&nbsp;&nbsp;</code>won’t be changed

32bit:<br>
project: potrace Release Win32<br>
produces:<br>
potrace-1.16\Release\potrace.exe<br>
potrace-1.16\Release\mkbitmap.exe<code>&nbsp;&nbsp;&nbsp;</code>won’t be changed

***image2image***<br>
***image2jpg***<br>
***image2pcx***<br>
***image2png***<br>
***jpg2noexif***

These programs are used to convert between different image graphic formats and are used in image file import functions, as well as in the function of printing the drawing or saving to a graphic file.<br>
These programs exist either as python scripts (.py) in the case of Linux, or in compiled form (.exe) in the case of Windows.

Compiled versions are included to simplify installation, since python is not always installed on Windows, while it is usually present on Linux.<br>
Python needs the pillows library so that scripts can be run.<br>
*.pyw files (equivalents to *.py files for Windows) are archived in the pyw folder, and can replace compiled versions, which requires minor changes to the program, in the same way as it is done in the Linux version.


***Frame3DDa***

Frame3DD is free open-source software for static and dynamic structural analysis of 2D and 3D frames and trusses with elastic and geometric stiffness. Copyright (C) 1992-2009  Henri P. Gavin

[***https://frame3dd.sourceforge.net/***](https://frame3dd.sourceforge.net/)

Original version of Frame3DD was modified by AlfaCAD author, that’s why the name of the folder was changed to Frame3DDa.

Due to modifications made, attached to the project Frame3DD version is the only one which will work with AlfaCAD. If you want to modify this program, please start from delivered version.

By the way: Frame3DD is not under development anymore by its authors, that's why we can assume that most up-to-date version is that one modified by AlfaCAD author.

Main changes:

1. Correction of a uniformly distributed load in the case of finite radii of the nodes between which the loaded element is spanned.
1. Correction of internal forces along the length of the element in the case of finite radii of the nodes between which the element is spanned.
1. Changing the method of dividing the length of an element for determining internal forces in cross- sections in the case of finite radii of the nodes between which a given element is spanned.
1. Added generation of dynamic analysis results in a format optimized for vibration modes animation in AlfaCAD.


***Linux***

Frame3DDa/CmakeLists.txt is a configuration file for cmake (Clion as IDE of my choice).

Use  frame3dd Release profile to compile it.

It will produce:  Frame3DDa/cmake-build-release/frame3dd

This program has to be copied into main AlfaCAD folder, if any alteration was made.

***Windows***

frame3dda

in:  Frame3DDa\src\Makefile  you can make a choice, or use  Frame3DDa\src\Makefile32 instead for 32bit

\#for 64bit with Strawberry (if installed for perl)<br>
LDFLAGS = -lm<br>
\#for 32bit with no Strawberry (if installed for perl, so disable it)<br>
\#LDFLAGS = -lm -m32

from:  "Frame3DDa\src
64bit:

make clean<br>
make

32bit:

make -f Makefile32 clean<br>
make -f Makefile32

It will produce:  Frame3DDa\src\frame3dd.exe

This program has to be copied into main AlfaCAD folder, if any alteration was made.

***Story behind AlfaCAD***

The forerunner of AlfaCAD was written around 1992 using a very primitive 16bit Borland C compiler and a simple BGI (Borland Graphic Interface) graphics library that had only 16 colors to choose from. The program was created in the Center of Europe, and it was a cheap alternative to the expensive and complicated CAD programs available at that time. It was called AlfCAD, and the root of the name came from the character named Alf, alien-looking puppet created in 1984 by Paul Fusco for the American TV series called "Alf". Because Alf was smart.

This simple program, after the breakup of the team of 2 authors, including the author of the contemporary AlfaCAD, was rewritten from scratch using more advanced tools, the 32-bit DJGPP compiler and the GRX graphics library. Eventually, at the end of the 20th century, the Allegro library was implemented, initially as a 32-bit extended DOS application, and finally as a native 32-bit Windows application using the Visual C compiler. In the meantime, the program was cloned as a program for designing industrial automation (Aster), and separate applications to support the design of underground sewage, water and gas networks (AlfCAD-Profile), as well as other clones supporting the design of reinforced concrete slabs and even designing houses using expanded clay block technology.

Then the economic crisis and personal crisis pushed the author away from the development of the program for over 16 years, to occupy him with completely different things, in another place, abroad, far from the users of his programs, which over time, due to the lack of support from the author, were replaced by other programs. During this time, the author was engaged in the development of programs supporting trading on derivatives exchanges, later cryptocurrencies. Finally, the author himself needed, as an engineer, to return to engineering graphics for a while, when after the next bend in the road he was heading for, he found other goals. Disappointment with the usability of what was available to download for free from the Internet finally pushed the author to dig out from an old hard drive the source code he had once written, and so the concept of bringing the program back to life in a new form was born.

Not to call the program pathetically Phoenix-CAD, although it was reborn from the ashes, it was given a name that reminds the name of its progenitor, but it has nothing to do with an extraterrestrial creature. Rather, the name suggests that it is such a basic CAD, like the first letter of the Greek alphabet. In Numerology the energy represented by the letter A resonates with the ideas of confidence, independence, courage, and leadership. It is resolute and purposeful. According to the ancient science of numerology, the letter A, also the Greek Α or α with its numerical value of 1, is associated with the qualities of leadership, individuality, and self-expression. It represents the beginning, initiative, and the pioneering spirit. Well, good enough.

Alfa spelling deviates from the English norm and is spelled with an f as it is in most European languages because the spelling Alpha may not be pronounced properly by native speakers of some languages – who may not know that ph should be pronounced as f. The author of AlfaCAD adopted this rule from the NATO phonetic alphabet.

Good ideas never die...

At the beginning of 2020, the author began the work on AlfaCAD, and at the end of the year he began the work on a 64-bit version of the Allegro 4 library, which did not exist. This was achieved on Christmas Eve 2020, and a day later AlfCAD was already 64-bit. The author still maintains the 32-bit version as a legacy, although maintaining it probably no longer makes sense, everyone uses the 64-bit version of Windows.

In 2022, the author began the work on a 64-bit version of AlfaCAD as a native Linux application with Xlib graphics (X11, XOrg).

Today, after more than 2 years of working with Arch Linux, KDE, CLion, Linux and AlfaCAD for Linux has gained the status of a key distribution, and the entire development process of the program has been moved to Linux, only with re-compiling the code on Windows using Visual Studio 2022 Community Edition (a version of cmake/CLion setup for Windows is under construction).

For backward compatibility, the program can load drawing files made in previous programs and converts them to AlfaCAD format. The same applies to drawing blocks, so the procedure for reading drawing files and blocks is somewhat complex. Many drawings included in the program installation package as examples come from previous programs.

Due to the fact that part of the code was created quite a long time ago, when the author did not think that he would one day publish his code and make it available to the whole world, some comments are written in the author's native language, as are the names of some variables which, although symbolic, suggest their use through their name. The newer part of the code uses English both for variable names, if they are in a readable form, not only symbolic, and for comments. The same applies to the names of source files.

The author will work to improve the shared code in this respect, but also knows from his own experience that in the era of automatic translators such an obstacle is only apparent.

In any situation, when your attempt to compile the program fails, despite your best efforts and fulfilling all the requirements described above, or when some part of the code seems incomprehensible to you or maybe contains errors, do not hesitate to inform me about it. Some solution will be found. Here is my email address:    <author@alfacad.net>

A ready-to-use installation package in 4 language versions, along with updates and a description of the latest extensions, can be downloaded from  [https://alfacad.net](https://alfacad.net/), 4 languages, so commonly used English, author’s mother tongue Polish, Ukrainian because if even just one single house will be rebuilt after the war using AlfaCAD, it was worth doing it, and Spanish porque lo estoy aprendiendo.

Enjoy AlfaCAD

author

Marek Ratajczak<code>&nbsp;&nbsp;&nbsp;&nbsp;</code>September 2026<br>


*Ultimately, I did what I always thought I would do. I'm not just taking advantage of Open Source, I'm contributing to it too.*

*Now if you like, you can hire me. I'm ready to participate in the project, where my expertise is good enough. You can check it out, just take my code.*

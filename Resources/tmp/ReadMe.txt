Content
=======
1. What is Iconizer
2. How to work with Iconizer
3. How to compile Iconizer


1. What is Iconizer
===================
Iconizer is a simple application to create and edit Windows Icons. It was inspired by old project
png2ico, but it does not use the png2ico code. It has been written from scratch. The application
provides GUI to easily manipulate the icon (.ico) files. Unlike png2ico projet, which was a corss
platform command line utility, Iconizer only runs on MS Windows. The reason for not porting this
application to other platforms is that icons are Windows specific, so it is not expected that
somebody would like to create or edit icons on other platforms.

What is Windows Icon?
---------------------
Windows Icon is a graphics file used to visually represent an application or file type on MS Windows
operating systems. A single icon file contains one or more bitmap images, and also so called "mask"
image for each of the stored images. Mask is used to define transparent areas of the image. The
icon file may store several images for different sizes and pixel depths. The operating system looks
for appropriate image within icon file to display, depending on the display device capabilities.

Windows XP and Vista Icons
--------------------------
Until Windows XP, the icon files typically contained bitmap images for 4 and 8 pixel depths with
sizes 16x16, 32x32 and 48x48 pixels. Windows XP introduced 32bit icon images, to support antialiasing,
and Windows Vista introduced 256x256 size to support aero (?). In order to keep the icon file size
small, the 256x256 Vista image can be stored in PNG format. Starting with XP, the icon image can
also contain large number of images - previous versions only accept icons with up to 10 images.
With correct order of images within the icon file, the Vista or XP icon can still be correctly read
by older operating systems - Windows 95, 98 and Milenium, and NT4, 2000 - provided that the number
of images within the file does not exceed 10. Iconizer alows you to create such icons.

Who might want to use Iconizer?
-------------------------------
Iconizer can be used directly by software developers, or by graphics designers supporting software
development. The process of using Iconizer does not invole the graphics design, so no art skill
is required for end users.


2. How to work with Iconizer
============================
There are basically two areas of using icon files. One is for an application or file type
representation within shell, and the other is for displaying images on toolbar buttons and possibly
other GUI elements. We will discus both the options.

Before starting Iconizer
------------------------
Before creating an icon file, one or more raster images should be prepared by a 3rd party tool. The
images accepted by iconizer should be in BMP or PNG format, the latter is preferred. The advantage of
PNG image is its translucency. Icon images usually do not occupy the whole rectangular area defined by
the image size, the trend is to leave some parts of the image transparent, or even translucent.
Translucency, or alpha chanell, is typically used for image antialiasing, icon shadows or so.

When BMP image is used as input, Iconizer allows choosing one color as transparent. This is not very
powerful. With PNG file, the transparency/translucency is defined naturally.

It is absolutely up the designer what application he/she will use to prepare the icon images. A very
good workflow is to use PovRay to create 3D scene with mixed photo and vector graphics elements and
render the scene into 256x256 PNG file. You have great control on which parts of the image will be
transparent or translucent. Finally, the image can be post processed in Gimp. This is truly Open
Source solution.

How to install Iconizer
-----------------------
Iconizer does not provide an installation utility. Simply unzip the binary archive appropriate for
your operating system into a directory, possibly create a shortcut to Iconizer.exe, and run it. Upon
exit, Iconizer will create Iconizer.xml file in the same directory as the exe file resides. This file
keeps some GUI settings - main window position and few more. Iconizer neither write to nor read from the
registry.

To unistall Iconizer simply delete the binary files and the xml file. Iconizer does not leave any rubbish
in your system.

Typical usage
-------------
Typically the user will want to create an icon to represent some entity within Windows shell. For
this purpose, a 256x256, 32bit PNG image is sufficient. Start iconizer and run File -> New. It will
create a new, blank icon. Run Image -> Add and select the prepared image file you want. "Add Image"
dialog will pop up with image preview and set of options to enter. You need to select what image sizes
should be included in the icon, and what color depths. If you are not sure, you can use one of the
preset buttons to create either XP or Vista style icon. Note that both icons should still be legible
for older operating system. The only difference is that Vista icon will most likely be quite a larger
file in size.

There are other two options - allow dithering and optimal palette - which control how images will be
subsampled to lower bit depths. 4bit images always use the system palette, however with 8bit images,
you can either leave the optimal palette unchecked - in this case the image will be projected into
256 color "web safe" pallete, or you can let the Iconizer to calculate optimal palette for the given
image.

If you want to use BMP image as a source, "Transparent Color" tool is also available on the "Add
Image" dialog. Click "Set" button and select a pixel on the preview image. If you are not satisfied
with the result, you can clear the transparent color by "Clear" button.

Once you are ready, click "OK" and the images will be inserted into the icon file. Now you can go
through the image list and check the results. Iconizer will insert the image in optimal order to
match your criteria, however, you can still rearrange the images by simply drag and drop the items
in the list.

In the Image menu, you now have several options to further manipulate the icon file. You can add
other images into icon. You can delete an image from the icon, the same can be done by "Del" key.
You can replace the image of particular size and bit depth by the "Load..." command. You can save
particular image into BMP (or PNG in case of Vista 256x256 image) file by the "Save..." command.
Finally, for 4 and 8 bit images, a simple image editor is available to adjust particular pixels.

Once you are satisfied with your icon, click File -> Save or Save As to save the icon file on the
disk.

Create button image usage
-------------------------
Icon file can also be used for toolbar buttons and other GUI graphics, which use ImageList objects.
The advantage is that ImageList API provides mechanism to insert icons, which allows you to use trans-
parent or antialiased images also on GUI controls. In this case, you will prepare image with the size
appropriate for your toolbar button and include it into icon file as custom sized image. You will insert
it as 4, 8 and 32 bit color depth. See MSDN on how to load such an icon into your image list at run time.

Editing Icons
-------------
Iconizer can also be used to edit existing icon files. Run File -> Open to load an existing icon file.
Then you can manipulate the icon images as in the previous cases.


3. How to compile Iconizer
==========================
Iconizer is writen in pure C++ and compiled by GCC clones for MS Windows platform - MinGW for 32 bit
platforms and MinGW64 for 64 bit platform. It does not use any obscure libraries like MFC or so, so it
most likely could be compiled by other C++ compilers such as Borland or MS C++ compilers. We only
provide makefiles for MinGW at the moment, makefiles for other compilers can be subject of donation.

Dependencies
------------
Iconizer only depends on other OS project, which is "libpng". It creates its own dll called PNGUtils.dll,
which itslef could possibly be distributed under LGPL license. However, libpng itself depends on zlib
project, so compilation of PNGUtils is not straightforward. If you want to avoid compiling zlib and libpng,
you may want to use binary files libz.a and libpng.a delivered with Iconizer. However, if you want to
compile iconizer with a compiler other than MinGW, you will need to compile also these two projects.

Compile zlib and libpng libraries
---------------------------------
We need to compile these two libraries with some cooperation. Create a comon folder for these two projects.
Create "libpng" and "zlib" subfolders in this folder. Download the latest sources of zlib and libpng and 
extract them into the corresponding folders preserving the archive directories. Go to "zlib\win32" folder
and copy "Makefile.gcc" to "zlib" folder. In case of 64 bit platform, use the file "makefile.mingw64"
from the archive indicated in next paragraph.

Download the "makezlibpng.zip" file from our project. Extract the batch file zlib_cmpl_mingwxx.bat, where
xx stands for the 32/64bit into "zlib" folder and run it. If everything goes OK you should have "libz.a"
file in the "zlib" fodler now. You will probably get some error on 64 bit platform, but this is not
important for us.

Extract the libpng archive into libpng folder. Create subfolder "Build" under your "libpngxxx" folder.
Extract the file "libpng_cmpl_mingwxx.bat" into "Build" directory and copy "makefile.mingw" from "scripts"
folder to "Build" folder. In case of 64 bit, you will need "makefile.mingw64" provided with our archive.
Edit "makefile.mingw"/"makefile.mingw64", find "ZLIBLIN" and "ZLIBINC" declarations and type the correct
location for zlib header and lib files. From the above setup both can be "../../../zlib". Finally run the
"libpng_cmpl_mingwxx.bat" file. If everything works fine, you should have "libpng.a" file now.

Iconizer versions
-----------------
Three types of iconizer can be compiled. ANSI version for Windows 95, 98 and Millenium, UNICODE version
for Windows NT4, 2000, XP and newer 32 bit systems, and 64 bit version.

Compile Iconizer
----------------
Download "Iconizer.zip" file from our project (if you read this file, you've most like done this already).
Extract the archive into a folder with your intended Iconizer project. Create "Build" subdirectpry of your
main project folder and copy the files "libz.a" and "libpng.a" into this "Build" directory. Edit the
appropriate makefile and correct the path for zlib and libpng includes - LIBZINC and LIBPNGINC.

If you don't want to compile libz.a and libpng.a, you can use the files provided in "makezlibpng.zip", in
appropriate "binxx" folder. However, you will still need source files of libz and libpng projects in order
to be able to compile Iconizer.

If you want 64 bit version, run "Iconizer_cmpl_mingw64.bat".

If you want UNICODE version (suitable for Windows NT, 2000, XP, Vista, 7 and all Servers) of 32 bit Iconizer,
run "Iconizer_cmpl_mingw32.bat". If you want ANSI version (suitable for Windows 95, 98 and Millenium), edit
the "makefile.mingw32" file and coment the CFLAGS with UNICODE definition and uncoment the other one. Then
run "Iconizer_cmpl_mingw32.bat".

Language versions
-----------------
Currently, the only language available is English. To produce other language mutations, translating of
"Iconizer_en.rc" file to other languages and recompiling the application is sufficient.

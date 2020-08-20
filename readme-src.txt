=================================================
Readme for Magenta Source Code (Visual C++ 6.0/.NET)

Version: 1.0 Build 61.

Public Release on:
3/31/2008

Readme last updated:
8/20/2020

http://www.fadedtwilight.org/magenta

================================================

Preface
=======

This is a historical artifact. Cyan Chat, in the form in which it existed when this was created,
no longer exists. I'm just keeping this around in case anyone is interested in a) how we used to do
things, and b) what we used to do to amuse ourselves in the Before Times of the Internet.

Introduction
============

Welcome to the Magenta source code.

Some stuff looks ugly, and some stuff looks rather nice, but that's the way it is with all source code. 
Any code that is without comments should be obvious in some way (e.g. all the linear searching I had 
to do within it). And if you see really evil casting (which you should), don't blame me. A lot of 
Windows code has a lot of weird casting, and you have to live with it, unfortunately.

The code is written using MFC, and it is also statically linked with the libraries to reduce dependencies
down to direct linking the Windows system DLLs. In order to statically link with MFC, you MUST have VC++ 6 
Professional. Also, due to some interesting additions in recent days, make sure that you have the most 
*recent* version of MFC, which is available by downloading the latest MS Platform SDK or ordering a CD with it.

I'm currently building using Visual C++ 6.0. Both a project workspace and solution have been included for people who
use either development environment; for some reason, compiling under .NET causes Magenta to misbehave when run 
outside of a debugger. I'm currently investigating this.

Source Code Index
=================

Here's an index of all the files and directories and their function. Any header files not specifically mentioned
correspond directly with their CPP file:

res/			- Contains all resource files (icons, etc.)
Images/			- Random images that *became* resources
Help/			- Sources for the help file. Intended to be used with the HTML Help Workshop.
Annotation.cpp          - Implementation of the Log Annotation dialog.
AutoLog.cpp		- Implementation of the Auto Log Options dialog.
BlockList.cpp		- Implementation of the block list window
ChatConsole.cpp		- Implementation of the chat console
ChatSocket.cpp		- Implementation of override of CSocket that allows for event handling when 
                          a packet comes in
ChatWindow.cpp		- The main chat window
dlgbars.cpp		- Code needed for putting dialog bars (toolbars, status bars) in a dialog box. 
                          Provided by MSDN.
Ignored.cpp		- Implementation of the ignored user list
InputBox.cpp		- A simple input box implementation
Log.cpp			- Implementation of CLog, a class designed for Magenta that handles memory-based
			  logging.
Magenta.cpp		- Main application file for Magenta, also contains version info extraction code.
Magenta.exe.manifest	- Application manifest for using Visual Styles (not sure if it works, but it is
			  written correctly.)
Magenta.rc		- Resource script for the program
mdlsmain.cpp		- Implementation of a "main modeless dialog window". Provided by MSDN.
modeless.cpp		- Implementation of a modeless dialog. Provided by MSDN.
NameSlide.cpp		- Dialog box for the "name-sliding" function
NewEditBox.cpp		- Override of the standard CEdit class for auto-completion
NewListBox.cpp		- Override of the CListBox class that allows Ctrl+clicking
Options.cpp		- The options dialog
Person.cpp		- CPerson abstracts out a single online user. Used internally for maintaining online lists.
PrivateChat.cpp		- The private chat window
resource.h		- Contains resource constant definitions.
Search.cpp		- Search Snapshot dialog box implementation.
StatusWindow.cpp	- The graphical "online list" window
StdAfx.cpp		- Standard MFC file used for precompiled headers
Tokenizer.cpp		- String tokenizer class written specifically for Magenta
UrlGrabber.cpp		- URL Grabber window
Constants.h		- File of useful constants and definitions used throughout the program.

Available Compilation Targets
=============================

Win32 - Debug:			Standard debug target.
Win32 - Release: 		Standard release version target.

Author Info
===========

Rick Coogle
AKA Yohshee
mailto:yohshee@hotmail.com
http://www.fadedtwilight.org

Copyright(C) 2002-2008
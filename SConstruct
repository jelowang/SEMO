# encoding: utf-8
# $Id: SConstruct,v0.1 2011/02/06 14:54:00 $
# This file contains rules to build software with the SCons tool
# (see the http://www.scons.org/ for details on SCons).
# Copyright (C) 2011, mooosu <mooosu@gmail.com>

#	Semo Compiler 0.3.0

#	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
#	GNU general public license as published by the Free Software Foundation.
#	The project lunched by Jelo Wang since 2.Feb.2008 from 'Techniques of Knowledge' community. 

#	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
#	the license as published by the free software foundation.this program is distributed in the hope 
#	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
#	or fitness for a particular purpose.																					
																												
#	(C)	突壳开源Techniques of Knowledge
#		an open source community since 2008
#		Community : http://www.tok.cc
#		Contact Us : jelo.wang@gmail.com

#		-Thanks to Our Committers and Friends
#		-Best Wish to all who Contributed and Inspired
#		-Techniques of Knowledge 
#		-致伙伴们最美好祝愿
#		-突壳开源社区

import os

semo = Environment()

cpp_path = ["front-para/c-front-para","common","mid-para"]

semo.Append( CPPPATH=cpp_path)
semo.Prepend( LIBPATH=[ ])
semo.Prepend( LIBS=[])

semo.Append( CPPFLAGS=" -ggdb3 -O0" )
semo.Append( LINKFLAGS=" -Wl,--as-needed -Wl,-zdefs " )


conf = Configure(semo)

common = ["main.c","sc.c"] + Glob("front-para/c-front-para/*.c")+ Glob("common/*.c")

semoSources= common

semo.Program(target="semo",source = semoSources)



# vim:ft=python:

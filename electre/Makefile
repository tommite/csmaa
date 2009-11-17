#
#  Copyright (C) 2005 Tommi Tervonen
#
#  This file is part of ELECTRE.
#
#  ELECTRE is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  ELECTRE is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with ELECTRE; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

objs= main.o electre.o electre_loader.o main_tri.o

#CXXFLAGS=-Wall -O3
CXXFLAGS=-Wall -g -DDEBUG

all: electre electre_tri

electre.o: electre.h electre.cpp
electre_loader.o: electre_loader.h electre_loader.cpp electre.h
main.o: main.cpp electre.h electre_loader.h
main_tri.o: main_tri.cpp electre.h electre_loader.h

electre: main.o electre.o electre_loader.o 
	g++ -o electre main.o electre.o electre_loader.o
electre_tri: main_tri.o electre.o electre_loader.o 
	g++ -o electre_tri main_tri.o electre.o electre_loader.o

clean: 
	-rm -f $(objs) *~ electre electre_tri

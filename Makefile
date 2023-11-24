# Makefile for sockets
# 
# Copyright (C) 2023
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, see <https://www.gnu.org/licenses/>
# 
# Description: Makefile of the sockets project
#
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
# 
# Date: 2023-10-31

SERVER       = server
CLIENT       = client

# Directories
SRCDIR       = src
OBJDIR       = obj
BINDIR       = bin

# Binary
SERVERBIN        = $(BINDIR)/$(SERVER)
CLIENTBIN        = $(BINDIR)/$(CLIENT)

# .c files
SRC        = $(wildcard $(SRCDIR)/*.c)

# .o files
OBJ        = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# Compilation flags
CFLAGS       = -Wall -Wextra -O3

# Compiler
CC         = gcc

all: distclean $(OBJDIR) $(BINDIR) $(SERVERBIN) $(CLIENTBIN)

$(SERVERBIN): $(OBJDIR)/$(SERVER).o
	$(CC) -o $@ $< $(CFLAGS)

$(CLIENTBIN): $(OBJDIR)/$(CLIENT).o
	$(CC) -o $@ $< $(CFLAGS)

$(BINDIR):
	mkdir $@
$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rvf $(OBJDIR)

distclean: clean
	rm -rvf *.log
	rm -rvf $(BINDIR)
	
.PHONY: all clean distclean


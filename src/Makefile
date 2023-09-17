#
# Makefile for AMPL-2023
#

# XXX Note: This Makefile is a bit more verbose, with more explicit rules, than
# I might normally write.  But this is because I want to you try and understand
# properly how Makefiles work, so that you can write your own.

# XXX Note: Once you are done considering these "XXX Note" comments, you are
# welcome to delete them.

# compiler flags
DEBUG    = -ggdb
OPTIMISE = -O0
WARNINGS = -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
CFLAGS   = $(DEBUG) $(OPTIMISE) $(WARNINGS)
DFLAGS   = -DDEBUG_PARSER -DDEBUG_SYMBOL_TABLE -DDEBUG_HASH_TABLE -DDEBUG_CODEGEN

# commands
# XXX Note: On Unix, the "cc" executable refers to the system's C compiler.  In
# open-source Unices like Ubuntu, "cc" is most likely a link to the actual C
# compiler executable, which is typically either "clang" or "gcc".  Clang is an
# LLVM front end for the C language, and is the default compiler on macOS... but
# it is also installed on the NARGA Ubuntu setup.  In contrast, GCC refers to
# the GNU compiler collection, and "gcc" is the GNU C compiler, which is the
# default on Ubuntu.  In my opinion, with the current state of the art, "clang"
# provides more informative error messages and static analysis than "gcc".  If
# you work on a computer without "clang" and you have root, then consider
# installing "clang".  If you do not, then set the CC variable below to either
# "cc" (to use the system default) or "gcc" (to use GCC, which should be
# available by default).  Linux installations typically provide functionality to
# manage "alternatives", which is to say, the default commands; refer to the
# system documentation for "update-alternatives", in particular, for the
# "cc"-named alternatives.
CC       = gcc
RM       = rm -f
COMPILE  = $(CC) $(CFLAGS) $(DFLAGS)
INSTALL  = install

# files
EXES     = amplc testhashtable testscanner testsymboltable

# directories
BINDIR   = ../bin
LOCALBIN = ~/.local/bin

# XXX Note: Setting LOCALBIN to ~/bin used to be accepted practice; and
# remember, "~" refers to your home directoy.  Nowadays, many systems (and
# people) prefer placing user-specific executables in ~/.local/bin instead.  (In
# the olden days it was de rigueur to show off arcane knowledge of Unices by
# having one's user binaries in plain sight, whereas nowadays everything must
# appear to work automagically.)  Also refer to the note below on setting up
# your PATH variable.

### RULES ######################################################################

# executables

amplc: amplc.c codegen.o error.o hashtable.o scanner.o symboltable.o token.o \
       valtypes.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

testhashtable: testhashtable.c error.o hashtable.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

testparser: amplc.c error.o scanner.o token.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$(basename $<) $^

testscanner: testscanner.c error.o scanner.o token.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

testsymboltable: testsymboltable.c error.o hashtable.o symboltable.o token.o \
                 valtypes.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

testtypechecking: amplc.c error.o hashtable.o scanner.o symboltable.o token.o \
                  valtypes.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$(basename $<) $^

# units

codegen.o: codegen.c boolean.h codegen.h error.h jvm.h symboltable.h token.h \
           valtypes.h
	$(COMPILE) -c $<

error.o: error.c error.h
	$(COMPILE) -c $<

hashtable.o: hashtable.c hashtable.h
	$(COMPILE) -c $<

scanner.o: scanner.c scanner.h
	$(COMPILE) -c $<

symboltable.o: symboltable.c boolean.h error.h hashtable.h symboltable.h \
               token.h valtypes.h
	$(COMPILE) -c $<

token.o: token.c token.h
	$(COMPILE) -c $<

valtypes.o: valtypes.c valtypes.h
	$(COMPILE) -c $<

# BINDIR

$(BINDIR):
	mkdir $(BINDIR)

### PHONY TARGETS ##############################################################

.PHONY: all clean install uninstall types

all: amplc

clean:
	$(RM) $(foreach EXEFILE, $(EXES), $(BINDIR)/$(EXEFILE))
	$(RM) *.o
	$(RM) -rf $(BINDIR)/*.dSYM

# XXX Note: For your program to be in your PATH, ensure that the following is
# somewhere near the end of your ~/.profile (for macOS, this might actually be
# ~/.bash_profile) file:
#
# if [ -d "$HOME/.local/bin" ] ; then
#     PATH="$HOME/.local/bin:$PATH"
# fi
#
# If this is not set, the install rule will copy the executable to
# ~/.local/bin, but it will not be resolved automatically by the shell.  If you
# update the profile, either close and reopen the terminal, or use the "source"
# command to reload the profile.
#
# Also, to enable bash completion (of filenames with .ampl extensions), add
# the following at the end of ~/.bashrc:
#
# # bash completion for AMPL
# complete -f -X '!*.ampl' amplc

# Install all compiler-related binaries in the local bin.
install:
	mkdir -p $(LOCALBIN)
	$(INSTALL) $(foreach EXEFILE, $(EXES), $(wildcard $(BINDIR)/$(EXEFILE))) \
		$(LOCALBIN)

# Remove all compiler-related binaries from the local bin.
uninstall:
	$(RM) $(foreach EXEFILE, $(EXES), $(wildcard $(LOCALBIN)/$(EXEFILE)))

# XXX Note: Make a highlight file for user-defined types.  This requires
# Universal (or older Exuberant) ctags and AWK.  To use this in Vim, add the
# following four lines to your Vim resource configuration file, typically
# ~/.vimrc for Vim and ~/.config/nvim/init.vim for Neovim (but Neovim also
# supports Lua as setup language, the commands for which are different and go
# in to ~/.config/nvim/init.lua):
# au BufRead,BufNewFile *.[ch] let fname = expand('<afile>:p:h') . '/types.vim'
# au BufRead,BufNewFile *.[ch] if filereadable(fname)
# au BufRead,BufNewFile *.[ch]   exe 'so ' . fname
# au BufRead,BufNewFile *.[ch] endif
types: types.vim
types.vim: *.[ch]
	ctags --c-kinds=t -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cType\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' > $@
	ctags --c-kinds=gsu -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cStorageClass\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' >> $@
	ctags --c-kinds=e -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cConstant\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' >> $@
	ctags --c-kinds=d -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword cDefine\t")}\
		{printf("%s ", $$1)}\
		END{if(!NR){print "XXX_placeholder"}else{print ""}}' >> $@

# VERSION    = core or simp 
# SOLVERNAME = name of the SAT solver
# SOLVERDIR  = subdirectory of the SAT solver
# NSPACE     = namespace of the SAT solver
# 
# e.g. minisat compilation with core version:
#
# VERSION    = core
# SOLVERNAME = "Minisat"
# SOLVERDIR  = minisat
# NSPACE     = Minisat
#
VERSION    = core
SOLVERNAME = "Glucose4.1"
SOLVERDIR  = glucose4.1
NSPACE     = Glucose
# THE REMAINING OF THE MAKEFILE SHOULD BE LEFT UNCHANGED
EXEC       = decomp
DEPDIR     = mtl utils core
DEPDIR 	   += ../..
DEPDIR     +=  ../../encodings ../../algorithms ../../graph ../../classifier
DEPDIR     += ../../../quick-cliques/src
MROOT      = $(PWD)/open-wbo/solvers/$(SOLVERDIR)
LFLAGS     += -lgmpxx -lgmp
# The flag MYDEBUG below indicates a debugging for Lucas/Dror code. Turn to 0 before final use. 4/2/2018
CFLAGS     = -Wall -Wno-parentheses -std=c++11 -DNSPACE=$(NSPACE) -DSOLVERNAME=$(SOLVERNAME) -DVERSION=$(VERSION) -DALLOW_ALLOC_ZERO_BYTES -DMYDEBUG=0 -g 
ifeq ($(VERSION),simp)
DEPDIR     += simp
CFLAGS     += -DSIMP=1 
ifeq ($(SOLVERDIR),glucored)
LFLAGS     += -pthread
CFLAGS     += -DGLUCORED
DEPDIR     += reducer glucored
endif
endif
include $(MROOT)/mtl/template.mk

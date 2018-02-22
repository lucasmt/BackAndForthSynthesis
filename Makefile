CXX=g++
CXXFLAGS= -std=c++11 -O2

HDIR=open-wbo/solvers/glucose4.1

ODIR=.

_OBJ = CNFChain.o CNFFormula.o CNFSpec.o Graph.o MSSSpec.o TrivialSpec.o VarSet.o WeightedGraph.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

decomp: $(OBJ)
	$(CXX) -o decomp main.cpp $^ $(CXXFLAGS)

$(ODIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) -I$(HDIR) -DNSPACE=Glucose

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o decomp

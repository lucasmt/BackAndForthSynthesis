#include "myCliqueHolder.h"

// system includes
#include <list>
#include <vector>
#include <climits>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

/*!  \file myCliqueHolder.cpp

    \brief We overpass the original Tomita algorithm by setting an object called CliqueHolder that holdes a maximal clique, along with other parameters which are necessary for the recursive execution of listAllMaximalCliquesMatrixRecursive() from TomitaAlgorithm.cpp
    \author Dror Fried

    \copyright Copyright (c) 2018 Dror Fried. This code is released under the GNU Public License (GPL) 3.0.

*/


    CliqueHolder::CliqueHolder(long* MycliqueCount, std::list<int> &MypartialClique,
    char** MyadjacencyMatrix,
    int* MyvertexSets, 
    int* MyvertexLookup, 
    int Mysize,
    int MybeginX,
    int MybeginP,
    int MybeginR,
    long& MystepsSinceLastReportedClique)
    {
        setCliqueHolder(MycliqueCount, MypartialClique,
            MyadjacencyMatrix,
            MyvertexSets, 
            MyvertexLookup, 
            Mysize,
            MybeginX,
            MybeginP,
            MybeginR,
            MystepsSinceLastReportedClique);    
        
    }
    
    CliqueHolder::~CliqueHolder(){}
    
    
    void CliqueHolder::setCliqueHolder(long* MycliqueCount, std::list<int> &MypartialClique,
    char** MyadjacencyMatrix,
    int* MyvertexSets, 
    int* MyvertexLookup, 
    int Mysize,
    int MybeginX,
    int MybeginP,
    int MybeginR,
    long& MystepsSinceLastReportedClique)
    {
       this-> MycliqueCount = MycliqueCount;
       this -> MypartialClique = MypartialClique ; //DF: make sure I can do that!
       this -> MyadjacencyMatrix = MyadjacencyMatrix;
       this -> MyvertexSets = MyvertexSets;
       this -> MyvertexLookup = MyvertexLookup;
       this -> Mysize = Mysize;
       this -> MybeginX = MybeginX;
       this -> MybeginP = MybeginP;
       this -> MybeginR = MybeginR;
       this -> MystepsSinceLastReportedClique = MystepsSinceLastReportedClique;
    }
       
       
        
        
        
    }
    
    

    std::list<int> CliqueHolder::getClique()
    {
        
     return  MypartialClique;  
    }



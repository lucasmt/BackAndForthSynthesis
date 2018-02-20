#ifndef _MY_CLIQUE_HOLDER_H
#define _MY_CLIQUE_HOLDER_H

/* 
    This program is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version. 
 
    This program is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details. 
 
    You should have received a copy of the GNU General Public License 
    along with this program.  If not, see <http://www.gnu.org/licenses/> 
*/

// local includes
//include "Tools.h"
//#include "MemoryManager.h"

//#include "Algorithm.h"

// system includes
#include <list>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstdlib>

/*! \file myCliqueHolder.h

    \brief see myCliqueHolder.cpp
    
    \author Dror Fried

    \copyright Copyright (c) 2018 Dror Fried. This code is released under the GNU Public License (GPL) 3.0.

*/

class CliqueHolder 
{
public:
    CliqueHolder(long* MycliqueCount, std::list<int> &MypartialClique,
    char** MyadjacencyMatrix,
    int* MyvertexSets, 
    int* MyvertexLookup, 
    int Mysize,
    int MybeginX,
    int MybeginP,
    int MybeginR,
    long& MystepsSinceLastReportedClique);
    
    
    void setCliqueHolder(long* MycliqueCount, std::list<int> &MypartialClique,
    char** MyadjacencyMatrix,
    int* MyvertexSets, 
    int* MyvertexLookup, 
    int Mysize,
    int MybeginX,
    int MybeginP,
    int MybeginR,
    long& MystepsSinceLastReportedClique);

    std::list<int> getClique();

private:
    long* MycliqueCount;
    std::list<int> &MypartialClique;
    char** MyadjacencyMatrix;
    int* MyvertexSets; 
    int* MyvertexLookup; 
    int Mysize;
    int MybeginX;
    int MybeginP;
    int MybeginR;
    long& MystepsSinceLastReportedClique;
};

#endif

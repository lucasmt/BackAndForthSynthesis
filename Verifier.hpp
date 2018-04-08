/* DF 4.4.2018.
 * This class is designed to verify the list of MSS that we get from Algorithm.hpp. 
 * Given a list L ={<M1,a1>,.....<Mn,an>} of tuples where each Mi is an MSS and ai is a corresponding assigment, we would like to verify that this list is indeed a solution for our synthesis problem.
 * Our general synthesis algorithm works as follows:
 * 
0) Assume F(x,y) is our input formula, F1(x) is the x part and F2(x) is the y part.
1) Given an input c to x, let F1^f(c) be the false clauses of F1(c).
2) Find an MSS s from our list L that contains F1^f(c).
3) Find in our list the matching assignment d for y that corresponds to the MSS s
4) Return s.

Now in this class we need to verify this algorithm. For this we need to verify the list L and to verify step 2 of the algorithm.

* Verify step 2 of the algorithm looks easy (algorithmically).

* To verify the list L of MSS we need to do the following:
 
1) We need to make sure that for every input c for x, F1^f(c) is contained in an mss from L.

2) We need to verify that our list works: namely that if <s,d> is an <MSS,assignment for y> in our list
then that d satisfies all the clauses in s.


We present several methods for (1) and a simple method for (2).

*/

#pragma once

#include "Set.hpp"
#include "Vector.hpp"
#include "CNFFormula.hpp"
#include "CNFSpec.hpp"
#include "Printing.hpp"
#include "CNFChain.hpp"
#include "Model.hpp"

#include <list>
#include <functional>
#include <chrono>
#include <iostream>

class Verifier
{
    
	Model model;
    CNFSpec f;
    CNFChain cnfChain;

	bool checkIfCovered(const Set<BVar>& assignment) const;

public:

Verifier(Model model, CNFSpec f, CNFChain cnfChain);

~Verifier();


//The following method verify that each MSS is set to the correct assignment. It does it as follows: for every pair <M,a> of <MSS,assignment> in the list the method checks that the assignment satisfies the clauses in the MSS.
bool VerifyMSSList() const;

bool VerifyInputCover() const;

bool RandomVerifyInputCover() const;
};

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

#include "Verifier.hpp"
#include <iostream>
#include <random>

using std::cout;
using std::endl;
using std::bernoulli_distribution;
using std::random_device;
using std::mt19937;
using std::move;
using std::function;

Verifier::Verifier(Model model,CNFSpec spec,CNFChain chain)
	: f(spec), cnfChain(chain)  //This notation means that f is initiallized with spec, without first initializing empty f.
{
    this->model = model;
   // this->f = spec;                 //This would not work since CNFSpec does not have an empty constructor
    printf("Verifier Generated\n");
}

Verifier::~Verifier(){}


/* DF: 4.6.2018. This algorithm works as follows: For every MSS pair <M.a> of <MSS, assignment> we iterate over all the <Z,clause(Y)> graph and for each pair <z,c(y)> we check if z is contained in the Mss M. 
 * If so, then we check if the assignment a satisfies the clause c(y). If not, we return false.
 * The algorithm return true iff for all MSS pairs <M,a> and for every pair <z,c(y)> : if z\in M then c(a) is true.
 */
bool Verifier::VerifyMSSList() const
{
	printf("Verifying MSS list\n");
    
	MSSSpec second = cnfChain.second; //This holds <the z vector and the entire Y CNF formula>, each z is associate with a corresponding 
    
	for (size_t id = 0; id < model.componentCount(); id++)
	{
		for (const Set<BVar>& mss : model.mssForComponent(id))
		{
			//printing the mss list  
			Set<BVar> indicatorAssignment = setDifference(mss, f.outputVars());
			print(indicatorAssignment, "z");
			cout << " |-> Output assignment: ";
			Set<BVar> outputAssignment = setDifference(mss, indicatorAssignment);
			print(outputAssignment, "y");
			cout << endl;
       
        
			//verifying that the assignment match F2
			for (size_t i = 0; i < second.indicatorVars().size(); i++)
			{
				auto inVar = indicatorAssignment.find(second.indicatorVars()[i]);
				if (inVar!=indicatorAssignment.end())
				{
					bool evalClause = ((second.outputCNF())[i]).eval(outputAssignment);
					if (!evalClause)
					{
						cout<<"ERROR IN EVALUATION!!!!!"<<endl;
						return false;
					}
				}
            
			}        

			cout<<" mss eval true "<<endl;

		}
	}
      
    return true;
}

/**
 * Recursively enumerates all assignments and invokes the callback for each.
 * 
 * - 'fixed' denotes the variables that are fixed for every assignment
 * - 'remaining' denotes the variables that can be set to either true or false
 * 
 * Every assignment is composed by taking the union of 'fixed' with a subset of 'remaining'
 */
bool forAllAssignments(Set<BVar>& fixed,
                       Set<BVar>& remaining,
                       function<bool(const Set<BVar>&)> callback)
{
	if (remaining.empty())
	{
		return callback(fixed);
	}
	else
	{
		/* Take the first of the remaining variables as the pivot */
		BVar pivot = *remaining.begin();

		remaining.erase(remaining.begin());

		/* Recursive call with the pivot set to false */
		bool result = forAllAssignments(fixed, remaining, callback);

		if (result)
		{
			fixed.insert(pivot);

			/* Recursive call with the pivot set to true */
			result = forAllAssignments(fixed, remaining, callback);

			/* Remove the pivot from the fixed variables... */
			fixed.erase(fixed.find(pivot));
		}

		/* ...and add it back to the remaining variables */
		remaining.insert(pivot);

		return result;
	}
}

bool Verifier::checkIfCovered(const Set<BVar>& inputAssignment) const
{
	cout << "Test input: ";
	print(inputAssignment, "x");
	cout << endl;

	/* Set of z variables activated by the given input */
	Set<BVar> outputAssignment = cnfChain.first.eval(inputAssignment);

	cout << "Activated variables: ";
	print(outputAssignment, "z");
	cout << endl;

	const Vector<Set<BVar>>& components = model.allComponents();

	/* For every component... */
	for (size_t i = 0; i < components.size(); i++)
	{
		/* ...restrict the assignment to the variables in the component */
		Set<BVar> restrictedAssignment = setIntersection(outputAssignment, components[i]);

		bool foundMSSCover = false;

		/* ...and look for an MSS that covers the restricted assignment */
		for (const Set<BVar>& mss : model.mssForComponent(i))
		{
			foundMSSCover = isSubset(restrictedAssignment, mss);

			if (foundMSSCover)
			{
				cout << "Found partial cover: ";
				print(restrictedAssignment, "z");
				cout << " is covered by ";
				print(setDifference(mss, f.outputVars()), "z"); /*< remove y variables from the MSS for printing */
				cout << endl;
				break;
			}
		}

		if (!foundMSSCover)
		{
			cout << "No cover found for ";
			print(restrictedAssignment, "z");
			cout << endl;

			return false;
		}
	}

	return true;
}

bool Verifier::VerifyInputCover() const
{
	cout << "Verifying coverage" << endl;

	Set<BVar> inputVars = f.inputVars();
	Set<BVar> potentialAssignment;

	/* Check for every possible assignment of the input variables if there is an MSS that covers it */
	bool ok = forAllAssignments(potentialAssignment, inputVars,
	                            [this] (const Set<BVar>& assignment) { return checkIfCovered(assignment); });

	if (ok)
		cout << "All test inputs were covered" << endl;
	else
		cout << "Error: there were test inputs that weren't covered" << endl;

	return ok;
}

/**
 * Uses the given RNG to select a random subset of the given set of variables.
 */
Set<BVar> randomSubset(const Set<BVar>& vars,
                       mt19937& rng)
{
	/* Generates booleans with equal probability */
	bernoulli_distribution dist(0.5);

	Set<BVar> subset;

	for (BVar var : vars)
	{
		if (dist(rng))
			subset.insert(var);
	}

	return subset;
}


bool Verifier::RandomVerifyInputCover() const
{
	cout << "Verifying coverage" << endl;

	/* Initialize random generation */
	random_device rd;
	mt19937 rng(rd());

	size_t sampleSize = 500; // number of sample assignments taken
	bool ok = true; // is set to false when verification fails

	for (size_t i = 0; i < sampleSize; i++)
	{
		/* Generates random assignment to the x variables */
		Set<BVar> inputAssignment = randomSubset(f.inputVars(), rng);

		ok &= checkIfCovered(inputAssignment);
	}

	if (ok)
		cout << "All test inputs were covered" << endl;
	else
		cout << "Error: there were test inputs that weren't covered" << endl;
		
	return ok;
}

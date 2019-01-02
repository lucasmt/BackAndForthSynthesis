#include "ReadInput.hpp"
#include "CNFDecomp.hpp"
#include "Algorithm.hpp"
#include "Printing.hpp"
#include "Verifier.hpp"

#include <chrono>
#include <iostream>




using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::endl;
using std::exception;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
	  cout << "Expected format: " << argv[0] << " <input-file>" << endl;
	}
	else
	{
		try
		{
        
#if MYDEBUG
			cout <<"*** Debug mode: " <<MYDEBUG <<" *****"<<endl;  
#endif
			/* Path to the input file */
			string inputPath(argv[1]);

			/* Parses the file into a CNF specification */
			CNFSpec f = loadDIMACS(inputPath);
#if MYDEBUG >=1
			cout << "=== CNF  Overall Specification ===" << endl;
			cout << "Input variables: ";
			print(f.inputVars(), "x");
			cout << endl;
			cout << "Output variables: ";
			print(f.outputVars(), "y");
			cout << endl;
			cout << "CNF formula:" << endl;
			print(f, "x", "y");
#endif                        
			/* Decompose specification into (F1, F2) */
			CNFChain cnfChain = cnfDecomp(f);
#if MYDEBUG >=1
			cout << "=== F1 ===" << endl;
			print(cnfChain.first, "x", "z");
			cout << "=== F2 ===" << endl;
			print(cnfChain.second, "z", "y");
#endif
			auto start = system_clock::now(); /*< start timing */

			//********************************   This is the main method of the algorithm ********************************
			/* Call the synthesis algorithm */
			Model model =
				//BAFAlgorithm(cnfChain.first, cnfChain.second); //        This is the non Decomposable version
				BAFConnectedComponents(cnfChain.first, cnfChain.second);  //This is the decomposable version
        
			//************************************************************************************************************  

			auto time = duration_cast<milliseconds>(system_clock::now() - start); /*< stop timing */

#if MYDEBUG >=1      
			printf("**************************************************************************************\n");
			cout << "=== Computed Overall MSS ===" << endl;
      
			const Vector<Set<BVar>>& components = model.allComponents();

			for (size_t i = 0; i < components.size(); i++)
			{
				cout << "MSS for component " << i << ": ";
				print(components[i], "z");
				cout << endl;

				for (const Set<BVar>& mss : model.mssForComponent(i))
				{
					cout << "MSS: ";

					Set<BVar> indicatorAssignment = setDifference(mss, f.outputVars());
					print(indicatorAssignment, "z");

					cout << " |-> Output assignment: ";

					Set<BVar> outputAssignment = setDifference(mss, indicatorAssignment);
					print(outputAssignment, "y");

					cout << endl;
				}
			}
#endif			

			cout << "=== Stats ===" << endl;

			cout << "Decision-list length: " << model.mssCount() << endl;
			cout << "Synthesis time: " << time.count() << "ms" << endl;
      
      
			/* DF: 4/4/2018 Adding a verifier object that is generated by the MSS list and perform various verification on the list.*/

			cout << "=== Verifying ===" << endl;
    
			Verifier MyVerifier(model,f,cnfChain);
			
			bool ok = MyVerifier.VerifyMSSList();

			if (f.inputVars().size() <= 15) //< if number of possible assignments is "small", just try all of them 
				ok &= MyVerifier.VerifyInputCover();
			else
				ok &= MyVerifier.RandomVerifyInputCover(); //< otherwise, select a random sample 
      
			if (ok)
				cout << "The model passed the verification" << endl;
			else
				cout << "The model failed the verification" << endl;
  
    
			//DF: a small checking code for the eval function.
			/*
			 * cout<<"====Verifying my code ====" <<endl;
			 CNFClause tmpClause(5);
			 tmpClause|=4;
			 tmpClause|=3;
			 tmpClause|=-6;
     
			 cout<<"c1:";
			 print(tmpClause,"x");
			 cout<<endl;
      
      
      
      
			 CNFClause tmpClause2(7);
			 tmpClause2|=4;
			 tmpClause2|=-3;
      
			 cout<<"c2:";
			 print(tmpClause2,"x");
			 cout<<endl;
      
			 Set<BVar> tmpAss;
			 tmpAss.insert(7);
			 tmpAss.insert(2);      
			 tmpAss.insert(3);           
      

   
      
			 print(tmpAss,"g");   
			 cout<<endl;
      
			 CNFFormula tmpFormula;
			 tmpFormula&=tmpClause;
			 tmpFormula&=tmpClause2;
       
			 bool resa = tmpClause.eval(tmpAss);
			 cout<<"result c1:"<<resa<<endl;
      
			 bool resb = tmpClause2.eval(tmpAss);
			 cout<<"result c2:"<<resb<<endl;
      
			 bool resTotal = tmpFormula.eval(tmpAss);
			 cout<<"result Total:"<<resTotal<<endl;
      
			*/
    
		}
    
    
		catch (const exception& e)
		{
			cout << e.what() << endl;
		}
	}
}

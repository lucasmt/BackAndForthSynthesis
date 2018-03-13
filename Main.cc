#include "ReadInput.hpp"
#include "CNFDecomp.hpp"
#include "Algorithm.hpp"
#include "Printing.hpp"

#include <chrono>
#include <iostream>

using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "Expected format: decomp <input-file>" << endl;
  }
  else
  {
    try
    {
      /* Path to the input file */
      string inputPath(argv[1]);

      /* Parses the file into a CNF specification */
      CNFSpec f = loadDIMACS(inputPath);

      cout << "=== CNF specification ===" << endl;
      cout << "Input variables: ";
      print(f.inputVars());
      cout << endl;
      cout << "Output variables: ";
      print(f.outputVars());
      cout << endl;
      cout << "CNF formula:" << endl;
      print(f.cnf());

      /* Decompose specification into (F1, F2) */
      CNFChain cnfChain = cnfDecomp(f);

      cout << "=== F1 ===" << endl;
      print(cnfChain.first);
      cout << "=== F2 ===" << endl;
      print(cnfChain.second);

      auto start = system_clock::now(); /*< start timing */

      /* Call the synthesis algorithm */
      Vector<Set<BVar>> mssList =
	BAFAlgorithm(cnfChain.first, cnfChain.second);
	//BAFConnectedComponents(cnfChain.first, cnfChain.second);

      auto time = duration_cast<milliseconds>(system_clock::now() - start); /*< stop timing */

      cout << "=== Computed MSS ===" << endl;
      
      for (const Set<BVar>& mss : mssList)
      {
	cout << "MSS: ";

	Set<BVar> indicatorAssignment = setDifference(mss, f.outputVars());
	print(indicatorAssignment);

	cout << " |-> Output assignment: ";

	Set<BVar> outputAssignment = setDifference(mss, indicatorAssignment);
	print(outputAssignment);

	cout << endl;
      }

      cout << "=== Stats ===" << endl;

      cout << "MSS count: " << mssList.size() << " sets" << endl;
      cout << "Running time: " << time.count() << "ms" << endl;
    }
    catch (const runtime_error& e)
    {
      cout << e.what() << endl;
    }
  }
}

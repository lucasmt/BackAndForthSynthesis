#include "ReadInput.hpp"
#include "CNFDecomp.hpp"
#include "Algorithm.hpp"

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

      /* Decompose specification into (F1, F2) */
      CNFChain cnfChain = cnfDecomp(f);

      auto start = system_clock::now(); /*< start timing */

      /* Call the synthesis algorithm */
      Vector<Set<BVar>> mssList =
	backAndForthAlgorithm(cnfChain.first, cnfChain.second);

      auto time = duration_cast<milliseconds>(system_clock::now() - start); /*< stop timing */

      cout << mssList.size() << " sets" << endl;
      cout << time.count() << "ms" << endl;
    }
    catch (const runtime_error& e)
    {
      cout << e.what() << endl;
    }
  }
}

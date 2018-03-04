#include "Algorithm.h"

// system includes
#include <string>

using namespace std;

Algorithm::Algorithm(std::string const &name)
 : m_sName(name)
 , m_bQuiet(false)
 , m_vCallBacks()
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::AddCallBack(std::function<bool(std::list<int> const&)> callback)
{
    m_vCallBacks.push_back(callback);
}

bool Algorithm::ExecuteCallBacks(std::list<int> const &vertexSet) const
{
    for (auto &function : m_vCallBacks) {
      bool cont = function(vertexSet);
      
      if (!cont) return false;
    }

    return true;
}

void Algorithm::SetName(string const &name)
{
    m_sName = name;
}

string Algorithm::GetName() const
{
    return m_sName;
}

void Algorithm::SetQuiet(bool const quiet)
{
    m_bQuiet = quiet;
}

bool Algorithm::GetQuiet() const
{
    return m_bQuiet;
}

#include <algorithm>

#include "Model.hpp"

using std::any_of;
using std::move;

size_t Model::addComponent(Set<BVar> component)
{
	size_t id = _componentList.size();

	_componentList.push_back(move(component));
	_componentMSS.emplace_back(); /*< initialize empty vector of MSS */

	return id;
}

void Model::addMSS(size_t componentId, Set<BVar> mss)
{
	_componentMSS[componentId].push_back(move(mss));
}

const Vector<Set<BVar>>& Model::allComponents() const
{
	return _componentList;
}

const Vector<Set<BVar>>& Model::mssForComponent(size_t componentId) const
{
	return _componentMSS[componentId];
}

size_t Model::componentCount() const
{
	return _componentList.size();
}

size_t Model::mssCount() const
{
	size_t count = 0;

	for (const Vector<Set<BVar>>& mssList : _componentMSS)
	{
		count += mssList.size();
	}

	return count;
}

bool Model::alreadyCovered(size_t componentId, const Set<BVar>& s)
{
	auto isSuperset = [&s] (const Set<BVar>& mss) { return isSubset(s, mss); };
	
	return any_of(_componentMSS[componentId].begin(),
	              _componentMSS[componentId].end(),
	              isSuperset);
}

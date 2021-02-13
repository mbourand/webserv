#include "MethodSource.hpp"

MethodSource::MethodSource()
{
	_methods.insert(new ConnectMethod());
	_methods.insert(new DeleteMethod());
	_methods.insert(new GetMethod());
	_methods.insert(new HeadMethod());
	_methods.insert(new OptionsMethod());
	_methods.insert(new PostMethod());
	_methods.insert(new PutMethod());
	_methods.insert(new TraceMethod());
}

MethodSource::MethodSource(const MethodSource& other)
{
	for (std::set<IMethod*>::iterator it = other._methods.begin(); it != other._methods.end(); it++)
		_methods.insert((*it)->clone());
}

MethodSource& MethodSource::operator=(const MethodSource& other)
{
	for (std::set<IMethod*>::iterator it = other._methods.begin(); it != other._methods.end(); it++)
		_methods.insert((*it)->clone());
	return *this;
}

MethodSource::~MethodSource()
{
	if (_methods.empty())
		return;
	for (std::set<IMethod*>::iterator it = _methods.begin(); it != _methods.end(); it++)
		delete *it;
}

bool MethodSource::contains(std::string type) const
{
	for (std::set<IMethod*>::iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		if ((*it)->getType() == type)
			return true;
	}
	return false;
}

IMethod* MethodSource::createByType(std::string type) const
{
	for (std::set<IMethod*>::iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		if ((*it)->getType() == type)
			return (*it)->clone();
	}
	return NULL;
}

IMethod* MethodSource::getByType(std::string type) const
{
	for (std::set<IMethod*>::iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		if ((*it)->getType() == type)
			return *it;
	}
	return NULL;
}



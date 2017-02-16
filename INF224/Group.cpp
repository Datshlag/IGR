#include "Group.h"

using namespace std;

#if VERSION == 8

	Group::~Group() { /*cout << "Group " << groupName << "destroyed";*/ }
	Group::Group(string _groupName): list<MultimediaObject*>(), groupName(_groupName) { }
	string Group::getName() const { return groupName; }
	void Group::displayElements(ostream& os) const {

		os << "Group "<< getName() << " contains : " << endl;
		for(auto it = begin(); it != end(); it++) {

			(*it)->display(os);
		}
	}

#elif VERSION >= 9

	typedef  shared_ptr<MultimediaObject> MOPtr;
	Group::~Group() { }
	Group::Group(string _groupName): list<MOPtr>(), groupName(_groupName) { }
	string Group::getName() const { return groupName; }
	void Group::displayElements(ostream& os) const {

		os << "Group '"<< getName() << "' contains : ";
		for(auto it = begin(); it != end(); it++) {

			(*it)->display(os);
			os << " / ";
		}
	}

#endif
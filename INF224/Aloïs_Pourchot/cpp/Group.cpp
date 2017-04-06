#include "Group.h"

using namespace std;

#if VERSION == 8
	
	///Destructor used just to keep track of the objects.
	Group::~Group() { cout << "Group " << groupName << "destroyed"; }
	Group::Group(string _groupName): list<MultimediaObject*>(), groupName(_groupName) { }
	string Group::getName() const { return groupName; }
	void Group::displayElements(ostream& os) const {

		os << "Group "<< getName() << " contains : " << endl;
		///C++11 like loop
		for(auto it = begin(); it != end(); it++) {

			(*it)->display(os);
		}
	}

#elif VERSION >= 9

	///Destructor used just to keep track of the objects.
	Group::~Group() { cout << "Group " << groupName << "destroyed"; }
	Group::Group(string _groupName): list<MOPtr>(), groupName(_groupName) { }
	string Group::getName() const { return groupName; }
	void Group::displayElements(ostream& os) const {

		os << "Group '"<< getName() << "' contains : ";
		///C++11 like loop
		for(auto it = begin(); it != end(); it++) {

			(*it)->display(os);
			os << " / ";
		}
	}

#endif
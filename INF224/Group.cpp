#include "Group.h"

using namespace std;

Group::~Group() { }

Group::Group(string _groupName): list<MultimediaObject*>(), groupName(_groupName) { }

string Group::getName() const { return groupName; }

void Group::displayElements(ostream& os) const {

	for(auto it = begin(); it != end(); it++) {

		(*it)->display(os);
	}
}
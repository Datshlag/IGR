#pragma once
#include <list>
#include <string>
#include "MultimediaObject.h"
using namespace std;

typedef ptrMo = shared_ptr<MultimediaObject>;

class Group : public list<ptrMo> {

	private:
		string groupName;

	public:
		~Group();
		Group(string _groupName);

		string getName() const;

		void displayElements(ostream& os) const;
};
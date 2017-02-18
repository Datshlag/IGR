#pragma once

#include <list>
#include <string>
#include <memory>

#include "MultimediaObject.h"
#include "Version.h"

class Data;

using namespace std;

#if VERSION == 8

	class Group : public list<MultimediaObject*> {

		private:
			string groupName;

		public:
			~Group();
			Group(string _groupName);
			string getName() const;
			void displayElements(ostream& os) const;
	};

#elif VERSION >= 9

	typedef shared_ptr<MultimediaObject> MOPtr;
	class Group : public list<MOPtr> {

		private:
			string groupName;

		public:
			~Group();
			Group(string _groupName);
			string getName() const;
			void displayElements(ostream& os) const;
	};

#endif
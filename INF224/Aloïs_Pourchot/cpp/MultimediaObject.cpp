#include "MultimediaObject.h"

typedef MultimediaObject mo;

mo::~MultimediaObject () { };
mo::MultimediaObject(): name(""), pathname("") { }
mo::MultimediaObject(const string &name, const string &pathname): name(name), pathname(pathname) { }

string mo::getClassName() const { return "MultimediaObject"; }
string mo::getPath() const { return pathname; }
string mo::getName() const { return name; }

void mo::setPath(const string &_pathname) { pathname = _pathname; }
void mo::setName(const string &_name) { name = _name; }

void mo::display(ostream &os) const {

    os << " name : " << name << " "
       << " path : " << pathname;
}

void mo::write(ostream  &os) const {

	os << name << endl;
	os << pathname << endl;
}

void mo::read(istream &is) {

	///We use getline so that we don't stop at the first space encountered.
	getline(is, name);
	getline(is, pathname);
}

#include "MultimediaObject.h"

typedef MultimediaObject mo;

mo::~MultimediaObject () { };

mo::MultimediaObject(): name(""), pathname("") { }
mo::MultimediaObject(const std::string &name, const std::string &pathname): name(name), pathname(pathname) { }

void mo::setName(const std::string &_name) { name = _name; }
void mo::setPath(const std::string &_pathname) { pathname = _pathname; }

std::string mo::getName() const { return name; }
std::string mo::getPath() const { return pathname; }

void mo::display(std::ostream &os) const {

    os << " name : " << name << " "
       << " path : " << pathname;
}

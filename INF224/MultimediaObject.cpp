#include "MultimediaObject.h"

MultimediaObject::~MultimediaObject() { }

MultimediaObject::MultimediaObject(): name(nullptr), pathname(nullptr) { }
MultimediaObject::MultimediaObject(const std::string name, const std::string pathname): name(name), pathname(pathname) { }

void MultimediaObject::setName(const std::string _name) { name = _name; }
void MultimediaObject::setPath(const std::string _pathname) { pathname = _pathname; }

const std::string MultimediaObject::getName() { return name; }
const std::string MultimediaObject::getPath() { return pathname; }

const void MultimediaObject::display(std::ostream& os) {

    os << " name : " << name << std::endl
       << " path : " << pathname << std::endl;
}

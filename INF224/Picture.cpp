#include "Picture.h"

typedef MultimediaObject mo;
typedef Picture pic;

pic::~Picture() { };

pic::Picture(): mo::MultimediaObject(), latitude(0), longitude(0) { }
pic::Picture(const std::string &name, const std::string &pathname, const double &_latitude, const double &_longitude):
        mo::MultimediaObject(name, pathname),
        latitude(_latitude),
        longitude(_longitude) { }

double pic::getLatitude() const { return latitude; }
double pic::getLongitude() const { return longitude; }

void pic::setLength(const double &_latitude) { latitude = _latitude; }
void pic::setLongitude(const double &_longitude) { longitude = _longitude; }

void pic::play() const { system(("imagej " + pic::getPath() + "&").c_str()); }

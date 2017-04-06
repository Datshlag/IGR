#include "Picture.h"

typedef MultimediaObject mo;
typedef Picture pic;

///Destructor used just to keep track of the objects
pic::~Picture() { cout << "Picture : " + getName() + " destroyed" << endl; }
pic::Picture(): mo::MultimediaObject(), latitude(0), longitude(0) { }
pic::Picture(const string &name, const string &pathname, const double &_latitude, const double &_longitude):
        mo::MultimediaObject(name, pathname),
        latitude(_latitude),
        longitude(_longitude) { }

double pic::getLatitude() const { return latitude; }
double pic::getLongitude() const { return longitude; }
string pic::getClassName() const { return "Picture"; }

void pic::setLatitude(const double &_latitude) { latitude = _latitude; }
void pic::setLongitude(const double &_longitude) { longitude = _longitude; }

void pic::display(ostream& os) const {

	MultimediaObject::display(os);
	os << " latitude : " << latitude 
	   << " longitude : " << longitude;
}

void pic::play() const { system(("imagej " + getPath() + "&").c_str()); }

void pic::write(ostream &os) const {

	MultimediaObject::write(os);
	os << latitude << endl;
	os << longitude << endl;
}

void pic::read(istream &is) {

	MultimediaObject::read(is);
	is >> latitude;
	is >> longitude;
}

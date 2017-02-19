#include "Video.h"

typedef MultimediaObject mo;
typedef Video vid;
    
///Destructor used just to keep track of the objects
vid::~Video() { cout << "Video : " + getName() + " destroyed" << endl; }
vid::Video(): mo::MultimediaObject(), length(0) { }
vid::Video(const string &name, const string &pathname, const int &_length):
        mo::MultimediaObject(name, pathname),
        length(_length) { }
        
int vid::getLength() const { return length; }
string vid::getClassName() const { return "Video"; }

void vid::setLength(const int &_length) { length = _length; }

void vid::display(ostream& os) const {

	MultimediaObject::display(os);
	os << " length : " << length;
}

void vid::play() const { system(("mpv " + getPath() + "&").c_str()); }

void vid::write(ostream &os) const { 

	MultimediaObject::write(os);
	os << length << endl;
}

void vid::read(istream &is) {

	MultimediaObject::read(is);
	is >> length;
}

#include "Video.h"

typedef MultimediaObject mo;
typedef Video vid;
    
vid::~Video() { std::cout << "Video : " + getName() + " destroyed" << std::endl; }

vid::Video(): mo::MultimediaObject(), length(0) { }
vid::Video(const std::string &name, const std::string &pathname, const int &_length):
        mo::MultimediaObject(name, pathname),
        length(_length) { }

int vid::getLength() const { return length; }
void vid::setLength(const int &_length) { length = _length; }

void vid::play() const { system(("mpv " + getPath() + "&").c_str()); }

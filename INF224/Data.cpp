#include "Data.h"

Data::~Data() { }
Data::Data() { }

MOPtr Data::newVideo(const string &name) {

	MOPtr video(new Video(name, "./" + name, 0));
	mOTable[name] = video;
	return video;
}

MOPtr Data::newPicture(const string &name) {

	MOPtr picture(new Picture(name, "./" + name, 0.0, 0.0));
	mOTable[name] = picture;
	return picture;
}

MOPtr Data::newFilm(const string &name) {

	MOPtr film(new Film(name, "./" + name, 0, nullptr, 0));
	mOTable[name] = film;
	return film;
}

GroupPtr Data::newGroup(const string &name) {

	GroupPtr group(new Group(name));
	groupTable[name] = group;
	return group;
}

void Data::searchMultimediaObject(const string &name, ostream &os) const {

	auto it = mOTable.find(name);
	if(it != mOTable.end()) (it->second)->display(os);
	else os << "Multimedia Object " << name << " not found." << endl;
}

void Data::searchGroup(const string &name, ostream &os) const {

	auto it = groupTable.find(name);
	if(it != groupTable.end()) (it->second)->displayElements(os);
	else os << "Group " << name << " not found." << endl;
}

void Data::playMultimediaObject(const string &name, ostream &os) const {

	auto it = mOTable.find(name);
	if(it != mOTable.end()) (it->second)->play();
	else os << "Multimedia Object " << name << " not found." << endl;
}
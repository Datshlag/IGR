#include "Data.h"


///No need to do anything in destructor thanks to smartpointers !
Data::~Data() { }
Data::Data() { }

VideoPtr Data::newVideo(const string &name) {

	VideoPtr video(new Video(name, "./" + name, 0));
	mOTable[name] = video;
	return video;
}

VideoPtr Data::newVideo(istream& is) {

	VideoPtr video(new Video("name", "./", 0));
	///We read the data from the input stream
	video->read(is);
	mOTable[video->getName()] = video;
	return video;
}

PicPtr Data::newPicture(const string &name) {

	PicPtr picture(new Picture(name, "./" + name, 0.0, 0.0));
	mOTable[name] = picture;
	return picture;
}

PicPtr Data::newPicture(istream &is) {

	PicPtr picture(new Picture("name", "./", 0.0, 0.0));
	///We read the data from the input stream
	picture->read(is);
	mOTable[picture->getName()] = picture;
	return picture;
}

FilmPtr Data::newFilm(const string &name) {

	FilmPtr film(new Film(name, "./" + name, 0, nullptr, 0));
	mOTable[name] = film;
	return film;
}

FilmPtr Data::newFilm(istream& is) {

	FilmPtr film(new Film("name", "./", 0, nullptr, 0));
	///We read the data from the input stream
	film->read(is);
	mOTable[film->getName()] = film;
	return film;
}

GroupPtr Data::newGroup(const string &name) {

	GroupPtr group(new Group(name));
	groupTable[name] = group;
	return group;
}

void Data::displayElements(ostream& os) const {

	os << mOTable.size() << " elements : " << endl;
	for(auto it: mOTable) {

		os << "  * " << (it.second)->getClassName() << ":";
		(it.second)->display(os);
		os << endl;
	}

}

void Data::searchMultimediaObject(const string &name, ostream &os) const {

	///We use the find method which returns a iterator on the element of it's in the table, or
	///mOTable.end() if it isn't.
	auto it = mOTable.find(name);
	if(it != mOTable.end()) (it->second)->display(os);
	else os << "Multimedia Object " << name << " not found.";
}

void Data::searchGroup(const string &name, ostream &os) const {

	///We use the find method which returns a iterator on the element of it's in the table, or
	///mOTable.end() if it isn't.
	auto it = groupTable.find(name);
	if(it != groupTable.end()) (it->second)->displayElements(os);
	else os << "Group " << name << " not found.";
}

void Data::playMultimediaObject(const string &name, ostream &os) const {

	///We use the find method which returns a iterator on the element of it's in the table, or
	///mOTable.end() if it isn't.
	auto it = mOTable.find(name);
	if(it != mOTable.end()) (it->second)->play();
	else os << "Multimedia Object " << name << " not found.";
}

bool Data::processRequest(TCPConnection& cnx, const string& request, string& response) {

	cerr << "\nRequest received : '" << request << "'" << endl;
	
	///Parsing the request to get the function and the parameters.
	stringstream reqss;
	stringstream respss;
	reqss.str(request);

	string function, arg1, arg2;
	reqss >> function;
	reqss >> arg1;
	reqss >> arg2;

	///Calling the appropriate function with the correct parameters.
	TCPLock lock(cnx);
	if(function == "search") {

		if(arg1 == "group") {

			searchGroup(arg2, respss);
		}
		else if (arg1 == "content") {

			searchMultimediaObject(arg2, respss);
		}
		else respss << "Incorrect request, please try again.";
	}
	else if (function == "play") {

		playMultimediaObject(arg1, respss);
		respss << "Playing now '" << arg1 << "' on server.";
	}
	else respss << "Incorrect request, please try again.";

	///Sending answer back to client.
	response = respss.str();

	///Returning true since we don't want to cut communication.
	return true;
}

bool Data::save(const string& fileName) const {

	///Opening file
	ofstream f(fileName);
	if(!f) {

		cerr << "Couldn't open file '" << fileName << "'" << std::endl;
		return false;
	}

	for(auto it: mOTable) {

		string str = (it.second)->getClassName();
		f << str << endl;
		(it.second)->write(f);
	}

	return true;
}

bool Data::load(const string& fileName) {

	///Opening file
	ifstream f(fileName);
 	if (!f) {
		cerr << "Can't open file " << fileName << endl;
		return false;
 	}

	while (f) { 

		string str;
		getline(f, str);


		if (str == "Video") {
			VideoPtr video = newVideo(f);
		}
		else if (str == "Picture") {
			PicPtr picture = newPicture(f);
		}
		else if (str == "Film") {
			FilmPtr film = newFilm(f);
		}
	}

	return true;
}
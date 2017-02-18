#include "Data.h"

Data::~Data() { }
Data::Data() { }

VideoPtr Data::newVideo(const string &name) {

	VideoPtr video(new Video(name, "./" + name, 0));
	mOTable[name] = video;
	return video;
}

VideoPtr Data::newVideo(istream& is) {

	VideoPtr video(new Video("name", "./", 0));
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

	auto it = mOTable.find(name);
	if(it != mOTable.end()) (it->second)->display(os);
	else os << "Multimedia Object " << name << " not found.";
}

void Data::searchGroup(const string &name, ostream &os) const {

	auto it = groupTable.find(name);
	if(it != groupTable.end()) (it->second)->displayElements(os);
	else os << "Group " << name << " not found.";
}

void Data::playMultimediaObject(const string &name, ostream &os) const {

	auto it = mOTable.find(name);
	if(it != mOTable.end()) (it->second)->play();
	else os << "Multimedia Object " << name << " not found.";
}

bool Data::processRequest(TCPConnection& cnx, const string& request, string& response) {

	cerr << "\nRequest received : '" << request << "'" << endl;
	
	// 1) pour decouper la requête:
	// on peut par exemple utiliser stringstream et getline()

	stringstream reqss;
	stringstream respss;
	reqss.str(request);

	string function, arg1, arg2;
	reqss >> function;
	reqss >> arg1;
	reqss >> arg2;

	// 2) faire le traitement:
	// - si le traitement modifie les donnees inclure: TCPLock lock(cnx, true);
	// - sinon juste: TCPLock lock(cnx);

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

	// 3) retourner la reponse au client:
	// - pour l'instant ca retourne juste OK suivi de la requête
	// - pour retourner quelque chose de plus utile on peut appeler la methode print()
	//   des objets ou des groupes en lui passant en argument un stringstream
	// - attention, la requête NE DOIT PAS contenir les caractères \n ou \r car
	//   ils servent à délimiter les messages entre le serveur et le client

	response = respss.str();

	// renvoyer false si on veut clore la connexion avec le client
	return true;
}

bool Data::save(const string& fileName) const {

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
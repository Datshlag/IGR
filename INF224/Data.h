#pragma once

#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

#include "Version.h"
#include "MultimediaObject.h"
#include "Film.h"
#include "Video.h"
#include "Picture.h"
#include "Group.h"
#include "tcpServer.h"

using namespace std;
using namespace cppu;

typedef shared_ptr<Group> GroupPtr;
typedef shared_ptr<MultimediaObject> MOPtr;
typedef shared_ptr<Picture> PicPtr;
typedef shared_ptr<Video> VideoPtr;
typedef shared_ptr<Film> FilmPtr;
typedef map<string, GroupPtr> GroupTable;
typedef map<string, MOPtr> MOTable;

class Data {

	private:
		GroupTable groupTable;
		MOTable mOTable;

	public:
		virtual ~Data();
		Data();

		VideoPtr newVideo(const string& name);
		VideoPtr newVideo(istream& is);
		PicPtr newPicture(const string& name);
		PicPtr newPicture(istream& is);
		FilmPtr newFilm(const string& name);
		FilmPtr newFilm(istream& is);
		GroupPtr newGroup(const string& name);

		void displayElements(ostream& os) const;
		void searchMultimediaObject(const string &name, ostream &os) const;
		void searchGroup(const string &name, ostream &os) const;
		void playMultimediaObject(const string &name, ostream &os) const;
		bool processRequest(TCPConnection& cnx, const string& request, string& response);
		bool save(const string& fileName) const;
		bool load(const string& fileName);
};
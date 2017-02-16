#pragma once

#include <map>
#include <sstream>

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
typedef map<string, GroupPtr> GroupTable;
typedef map<string, MOPtr> MOTable;

class Data {

	private:
		GroupTable groupTable;
		MOTable mOTable;

	public:
		virtual ~Data();
		Data();

		MOPtr newVideo(const string& name);
		MOPtr newPicture(const string& name);
		MOPtr newFilm(const string& name);
		GroupPtr newGroup(const string& name);

		void searchMultimediaObject(const string &name, ostream &os) const;
		void searchGroup(const string &name, ostream &os) const;
		void playMultimediaObject(const string &name, ostream &os) const;

		bool processRequest(TCPConnection& cnx, const string& request, string& response);
};
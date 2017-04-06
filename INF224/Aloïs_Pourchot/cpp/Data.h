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

///Just for convenience sake
typedef shared_ptr<Group> GroupPtr;
typedef shared_ptr<MultimediaObject> MOPtr;
typedef shared_ptr<Picture> PicPtr;
typedef shared_ptr<Video> VideoPtr;
typedef shared_ptr<Film> FilmPtr;
typedef map<string, GroupPtr> GroupTable;
typedef map<string, MOPtr> MOTable;

/**
 * @brief      Data class. This is the class which is used to keep tracks of all the multimediaobjects and groups created.
 * 			   Data is stored using std::map, using names as keys. We use smart pointers everywhere for a better memory usage.
 */
class Data {

	private:
		GroupTable groupTable; /**< The group table. */
		MOTable mOTable; /**< The multimediaobject table. */

	public:
		/**
		 * @brief      Destroys the data object.
		 */
		virtual ~Data();
		/**
		 * @brief      Creates the data object.
		 */
		Data();

		/**
		 * @brief      Used to create a new blank video.
		 *
		 * @param[in]  name  The name
		 *
		 * @return     Smart pointer to the video created.
		 */
		VideoPtr newVideo(const string& name);
		/**
		 * @brief      Used to create a new video from a file.
		 *
		 * @param      is    input stream.
		 *
		 * @return     Smart pointer to the video created.
		 */
		VideoPtr newVideo(istream& is);
		/**
		 * @brief      Used to create a new blank picture.
		 *
		 * @param[in]  name  The name
		 *
		 * @return     Smart pointer to the picture created.
		 */
		PicPtr newPicture(const string& name);
		/**
		 * @brief      Used to create a new picture from a file.
		 *
		 * @param      is    input stream.
		 *
		 * @return     Smart pointer to the picture created.
		 */
		PicPtr newPicture(istream& is);
		/**
		 * @brief      Used to create a new blank film.
		 *
		 * @param[in]  name  The name
		 *
		 * @return     Smart pointer to the film created.
		 */
		FilmPtr newFilm(const string& name);
		/**
		 * @brief      Used to create a new film from a file.
		 *
		 * @param      is    input stream.
		 *
		 * @return     Smart pointer to the film created.
		 */
		FilmPtr newFilm(istream& is);
		/**
		 * @brief      Used to create a new blank group.
		 *
		 * @param[in]  name  The name
		 *
		 * @return     Smart pointer to the group created.
		 */
		GroupPtr newGroup(const string& name);

		/**
		 * @brief      Displays info on all the elements of the mOTable.
		 *
		 * @param      os    The output stream.
		 */
		void displayElements(ostream& os) const;
		/**
		 * @brief      Look for a multimedia object with the given name in the multimediaobject table.
		 *
		 * @param[in]  name  The name of the object we're looking for.
		 * @param      os    The output stream.
		 */
		void searchMultimediaObject(const string &name, ostream &os) const;
		/**
		 * @brief      Look for a group with the given name in the group table.
		 *
		 * @param[in]  name  The name of the group we're looking for.
		 * @param      os    The output stream.
		 */
		void searchGroup(const string &name, ostream &os) const;
		/**
		 * @brief      Play the multimedia object with the given name, displays error if we can't find the object.
		 *
		 * @param[in]  name  The name of the object we want to play.
		 * @param      os    The output stream.
		 */
		void playMultimediaObject(const string &name, ostream &os) const;
		/**
		 * @brief      Process the request received from the server and sends back the appropriate answer.
		 *
		 * @param      cnx       The tcp connection
		 * @param[in]  request   The request
		 * @param      response  The response
		 *
		 * @return     Returns true if successful, false otherwise.
		 */
		bool processRequest(TCPConnection& cnx, const string& request, string& response);
		/**
		 * @brief      Saves all data in the multimediaobject table.
		 *
		 * @param[in]  fileName  The file name where we want to save the data.
		 *
		 * @return     Returns true if successful, false otherwise.
		 */
		bool save(const string& fileName) const;
		/**
		 * @brief      Loads data from the file in the multimedia object table.
		 *
		 * @param[in]  fileName  The file name from where we read the data.
		 *
		 * @return     Returns true if successful, false otherwise.
		 */
		bool load(const string& fileName);
};
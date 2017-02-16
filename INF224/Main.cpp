#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <list>

#include "Version.h"

#include "MultimediaObject.h"
#include "Film.h"
#include "Group.h"
#include "Picture.h"
#include "Video.h"
#include "Data.h"
#include "tcpServer.h"

const int PORT = 3331;


int main (int argc, char ** argv) {

	#if VERSION == 5

		MultimediaObject** multimediaObjectArray = new MultimediaObject*[2];
		multimediaObjectArray[0] = new Video("FILM1", "film1.mp3", 120);
		multimediaObjectArray[1] = new Picture("PIC1", "pic1.png", 1920.0, 1080.0);

		multimediaObjectArray[0]->play();
		multimediaObjectArray[1]->play();
	#endif

	#if VERSION == 6

		int chapters [] = {10, 20, 30, 40, 50};
		Film* film = new Film("cool movie", "./cool_movie", 150, chapters, 5);
		*chapters = NULL;
		film->displayChapters(std::cout);

		int chapters_new [] = {1, 2, 3, 4, 5};
		film->setChapters(chapters_new, 5);
		*chapters_new = NULL;
		film->displayChapters(std::cout);

		/*  Ci-dessous ne compile pas car rompt l'encapsulation forcée par le const, 
			on protège donc les données du film.

			chapters = film->getChapters(); 
			chapters[0] = 2;

			const int chapters_const[5] = {10, 20, 30, 40, 50};
			chapters_const = film->getChapters();
			chapters_const[0] = 2;
		*/
	#endif

	#if VERSION == 8

		MultimediaObject* pic1 = new Picture("PIC1", "pic1.png", 1920.0, 1080.0);
		MultimediaObject* pic2 = new Picture("PIC2", "pic2.png", 1920.0, 1080.0);

		MultimediaObject* vid1 = new Video("FILM1", "film1.mp3", 120);
		MultimediaObject* vid2 = new Video("FILM2", "film2.mp3", 120);

		Group* group1 = new Group("cool stuff");
		group1->push_back(pic1);
		group1->push_back(pic2);
		group1->push_back(vid1);

		Group* group2 = new Group("fun stuff");
		group2->push_back(pic2);
		group2->push_back(vid1);
		group2->push_back(vid2);

		group1->displayElements(std::cout);
		group2->displayElements(std::cout);

		delete group1;

		/* Pas de soucis */
		group2->displayElements(std::cout);

		/*  Problème ici car le contenu pointé par vid2 est aussi pointé par un élément
			du groupe, il y a risque de segfault !

			delete vid2;
			vid2 = NULL;
			group2->displayElements(std::cout);
		*/
	#endif

	#if VERSION == 9

		std::shared_ptr<MultimediaObject> pic1 (new Picture("PIC1", "pic1.png", 1920.0, 1080.0));
		std::shared_ptr<MultimediaObject> pic2 (new Picture("PIC2", "pic2.png", 1920.0, 1080.0));

		std::shared_ptr<MultimediaObject> vid1 (new Video("FILM1", "film1.mp3", 120));
		std::shared_ptr<MultimediaObject> vid2 (new Video("FILM2", "film2.mp3", 120));

		Group* group1 = new Group("cool stuff");
		group1->push_back(pic1);
		group1->push_back(pic2);
		group1->push_back(vid1);

		Group* group2 = new Group("fun stuff");
		group2->push_back(pic2);
		group2->push_back(vid1);
		group2->push_back(vid2);

		pic1.reset();
		pic2.reset();
		vid1.reset();
		vid2.reset();

		std::cout << "Deleting group1 containing pic1, pic2 and vid1" << std::endl;
		delete group1;

		std::cout << "Deleting group2 containing pic2, vid1 and vid2" << std::endl;
		delete group2;

	#endif

	#if VERSION == 10

		Data *data = new Data();

		data->newVideo("chat.mp4");
		data->newVideo("chien.mp4");
		data->newGroup("Animaux");
		data->searchMultimediaObject("chien.mp4", std::cout);
		data->searchGroup("voyage", std::cout);

		delete data;

	#endif

	#if VERSION == 11

		// cree le TCPServer
		shared_ptr<TCPServer> server(new TCPServer());

		// cree l'objet qui gère les données
		shared_ptr<Data> data(new Data());
		data->newVideo("chien.mp4");
		data->newGroup("vacances")->push_back(data->newFilm("VACANCES2012.avi"));

		// le serveur appelera cette méthode chaque fois qu'il y a une requête
		server->setCallback(*data, &Data::processRequest);

		// lance la boucle infinie du serveur
		cout << "Starting Server on port " << PORT << endl;
		cout << "Known requests are the following :" << endl;
		cout << " * search group _groupname" << endl;
		cout << " * search content _contentname" << endl;
		cout << " * play _contentname" << endl;
		int status = server->run(PORT);

		// en cas d'erreur
		if (status < 0) {
			cerr << "Could not start Server on port " << PORT << endl;
			return 1;
		}

	#endif

	return 0;
}

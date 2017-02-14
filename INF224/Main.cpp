#include <iostream>
#include <string>
#include <algorithm>
#include <list>

#include "MultimediaObject.h"
#include "Video.h"
#include "Picture.h"
#include "Group.h"
#include "Film.h"

enum VERSION {ETAPE5, ETAPE6, ETAPE7};
VERSION curr_version = ETAPE7;

int main (int argc, char ** argv) {

	if(curr_version == ETAPE5) {

		MultimediaObject** multimediaObjectArray = new MultimediaObject*[2];
		multimediaObjectArray[0] = new Video("FILM1", "film1.mp3", 120);
		multimediaObjectArray[1] = new Picture("PIC1", "pic1.png", 1920.0, 1080.0);

		multimediaObjectArray[0]->play();
		multimediaObjectArray[1]->play();
	}

	if(curr_version == ETAPE6) {

		int chapters [] = {10, 20, 30, 40, 50};
		Film* film = new Film("cool movie", "./cool_movie", 150, chapters, 5);
		*chapters = NULL;
		film->displayChapters(std::cout);

		int chapters_new [] = {1, 2, 3, 4, 5};
		film->setChapters(chapters_new, 5);
		*chapters_new = NULL;
		film->displayChapters(std::cout);

		/*  Ci-dessous ne compile pas car rompt l'encapsulation forcée par le const, 
			on protège donc les données de film.

			chapters = film->getChapters(); 
			chapters[0] = 2;

			const int chapters_const[5] = {10, 20, 30, 40, 50};
			chapters_const = film->getChapters();
			chapters_const[0] = 2;
		*/
	}

	if(curr_version == ETAPE7) {

		Picture* pic1 = new Picture("PIC1", "pic1.png", 1920.0, 1080.0);
		Picture* pic2 = new Picture("PIC2", "pic2.png", 1920.0, 1080.0);

		Video* vid1 = new Video("FILM1", "film1.mp3", 120);
		Video* vid2 = new Video("FILM2", "film2.mp3", 120);

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
	}

	return 0;
}

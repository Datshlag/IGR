#include <algorithm>
#include <iostream>
#include <string>

#include "Film.h"

Film::~Film() { delete[] chapters; }

Film::Film(): Video::Video(), chapters(nullptr), numberOfChapters(0) { }
Film::Film (std::string _name, std::string _pathname, 
            int _length, int* _chapters,
            int _numberOfChapters): 
			Video::Video(_name, _pathname, _length), 
			numberOfChapters(_numberOfChapters) { 

	chapters = new int[numberOfChapters];
	std::copy_n(_chapters, numberOfChapters, chapters);
}

int Film::getNumberOfChapters() const { return numberOfChapters; }
const int* Film::getChapters() const { return chapters; }

void Film::setChapters(const int * newChapters, int newChaptersNumber) {

	delete[] chapters;
	numberOfChapters = newChaptersNumber;
	chapters = new int[numberOfChapters];
	std::copy_n(newChapters, numberOfChapters, chapters);
}

void Film::displayChapters(std::ostream& os) const{ 

	for(int i = 0; i < numberOfChapters; i++) {
        os << "Le chapitre : " << i + 1 << " dure "
        << chapters[i] << " secondes" << std::endl;
	}
}
#include "Film.h"

Film::~Film() { delete[] chapters; }
Film::Film(): Video::Video(), chapters(nullptr), numberOfChapters(0) { }
Film::Film (string _name, string _pathname, 
            int _length, int* _chapters,
            int _numberOfChapters): 
			Video::Video(_name, _pathname, _length), 
			numberOfChapters(_numberOfChapters) { 

	chapters = new int[numberOfChapters];
	copy_n(_chapters, numberOfChapters, chapters);
}

int Film::getNumberOfChapters() const { return numberOfChapters; }
const int* Film::getChapters() const { return chapters; }
string Film::getClassName() const { return "Film"; }

void Film::setChapters(const int * newChapters, int newChaptersNumber) {

	if(chapters) delete[] chapters;
	numberOfChapters = newChaptersNumber;
	chapters = new int[numberOfChapters];
	copy_n(newChapters, numberOfChapters, chapters);
}

void Film::displayChapters(ostream& os) const{ 

	for(int i = 0; i < numberOfChapters; i++) {
        os << "Le chapitre : " << i + 1 << " dure "
        << chapters[i] << " secondes" << endl;
	}
}

void Film::write(ostream& os) const {

	Video::write(os);
	os << numberOfChapters << endl;
	for(int i = 0; i < numberOfChapters; i++) {

		os << chapters[i] << " ";
	}
	os << endl;
}

void Film::read(istream &is) {

	Video::read(is);
	is >> numberOfChapters;

	if(chapters) delete[] chapters;
	chapters = new int[numberOfChapters];

	for(int i = 0; i < numberOfChapters; i++)
		is >> chapters[i];
}
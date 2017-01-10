#pragma once

#include "Video.h"

class Film : public Video {

  private:
      int* chapters;
      int numberOfChapters;

  public:
      virtual ~Film() { delete[] chapters; }

      Film(): Video(), chapters(NULL), numberOfChapters(0) { }
      Film(std::string name, std::string pathname, int _length, int* _chapters,
          int _numberOfChapters): Video(name, pathname, length),
          numberOfChapters(_numberOfChapters) {

          chapters = new int[numberOfChapters];
          std::copy_n(_chapters, numberOfChapters, chapters);
      }

      const int getNumberOfChapters() { return numberOfChapters; }
      const int* getChapters() {

          int * result = new int[numberOfChapters];
          std::copy_n(chapters, numberOfChapters, result);
          return result;
      }

      const void displayChapters(std::ostream& os) {

          for(int i = 0; i < numberOfChapters; i++) {

            os << "Le chapitre : " << i << " dure "
            << chapters[i] << " secondes" << std::endl;
          }
      }

};

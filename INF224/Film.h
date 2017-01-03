#include "Video.h"

class Film : public Video {

  private:
      int* chapters;
      int numberOfChapters;

  public:
      virtual ~Film() { delete[] chapters; }

      Film(): Video(), chapters(NULL), numberOfChapters(0) { }
      Film(std::string name, std::string pathname, int _length, int* _chapters, int _numberOfChapters): Video(name, pathname, length), numberOfChapters(_numberOfChapters) {

          chapters = new int[numberOfChapters];
          for(int i = 0; i<numberOfChapters; i++) {

            chapters[i] = _chapters[i];
          }
      }

      const int getNumberOfChapters() { return numberOfChapters };

}

#include "Video.h"

#include <algorithm>

class Film : public Video {

  private:
      int* chapters;
      int numberOfChapters;

  public:
      virtual ~Film() { delete[] chapters; }

      Film(): Video(), chapters(NULL), numberOfChapters(0) { }
      Film(std::string name, std::string pathname, int _length, int* _chapters, int _numberOfChapters): Video(name, pathname, length), numberOfChapters(_numberOfChapters) {

          chapters = new int[numberOfChapters];
          std::copy_n(_chapters, numberOfChapters; chapters);
      }

      const int getNumberOfChapters() { return numberOfChapters };

}

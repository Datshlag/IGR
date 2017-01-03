#include "MultimediaObject.h"

class Video : public MultimediaObject {

  private:
      int length;

  public:
      virtual ~Video() { }

      Video(): MultimediaObject(), length(0) { }
      Video(std::string name, std::string pathname, int _length): MultimediaObject(name, pathname), length(_length) { }

      const int getLength() { return length; }
      void setLength(const int &_length) { length = _length; }

      void const play() { system(("mpv" + getPath() + "&").c_str()); }
};

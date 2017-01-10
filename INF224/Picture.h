#include "MultimediaObject.h"

class Picture : public MultimediaObject {

  private:
      double latitude;
      double longitude;

  public:
      virtual ~Picture() { }

      Picture(): MultimediaObject(), latitude(0), longitude(0) { }
      Picture(std::string name, std::string pathname, double _latitude, double _longitude): MultimediaObject(name, pathname), latitude(_latitude), longitude(_longitude) { }

      const double getLatitude() { return latitude; }
      const double getLongitude() { return longitude; }

      void setLength(const double &_latitude) { latitude = _latitude; }
      void setLongitude(const double &_longitude) { longitude = _longitude; }

      const void play() { system(("imagej" + getPath() + "&").c_str()); }
};

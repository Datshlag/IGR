#pragma once

#include <memory>

#include "MultimediaObject.h"
#include "Version.h"

class Data;

class Picture : public MultimediaObject {

  private:
        double latitude;
        double longitude;

  public:
        virtual ~Picture();

        Picture();
        Picture(const std::string &name, const std::string &pathname, const double &_latitude, const double &_longitude);

        double getLatitude() const;
        double getLongitude() const;

        void setLength(const double &_latitude);
        void setLongitude(const double &_longitude);

        void play() const override;
};

#pragma once

#include <iostream>
#include <string>

#include "MultimediaObject.h"
#include "Version.h"

using namespace std;

class Picture : public MultimediaObject {

    private:
        double latitude;
        double longitude;

    public:
        virtual ~Picture();

        Picture();
        Picture(const string &name, const string &pathname, const double &_latitude, const double &_longitude);

        double getLatitude() const;
        double getLongitude() const;
        string getClassName() const override;

        void setLatitude(const double &_latitude);
        void setLongitude(const double &_longitude);

        void display(ostream& os) const override;
        void play() const override;
        void write(ostream &os) const override;
        void read(istream &is) override;
};

#pragma once

#include <iostream>
#include <string>

#include "MultimediaObject.h"
#include "Version.h"

using namespace std;

class Video : public MultimediaObject {

    private:
        int length;

    public:
        virtual ~Video();
        Video();
        Video(const string &name, const string &pathname, const int &_length);

        int getLength() const;
        string getClassName() const override;

        void setLength(const int &_length);

        void display(ostream& os) const override;
        void play() const override;
        void write(ostream& os) const override;
        void read(istream& is) override;

};
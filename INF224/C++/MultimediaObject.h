#pragma once

#include <iostream>
#include <string>

using namespace std;

class MultimediaObject {

    private:
        string name;
        string pathname;

    public:
        virtual ~MultimediaObject ();
        MultimediaObject();
        MultimediaObject(const string &name, const string &pathname);

        string getName() const;
        string getPath() const;
        virtual string getClassName() const;

        void setName(const string &_name);
        void setPath(const string &_pathname);

        virtual void display(ostream &os) const;
        virtual void play() const = 0;
        virtual void write(ostream &os) const;
        virtual void read(istream &is);
};
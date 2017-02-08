#pragma once

#include <iostream>
#include <string>

class MultimediaObject {

    private:
        std::string name;
        std::string pathname;

    public:
        virtual ~MultimediaObject ();

        MultimediaObject();
        MultimediaObject(const std::string &name, const std::string &pathname);

        std::string getName() const;
        std::string getPath() const;

        void setName(const std::string &_name);
        void setPath(const std::string &_pathname);

        void display(std::ostream &os) const;

        virtual void play() const = 0;
};
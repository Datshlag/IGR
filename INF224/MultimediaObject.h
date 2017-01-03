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
    MultimediaObject(const std::string name, const std::string pathname);

    const std::string getName();
    const std::string getPath();

    void setName(const std::string _name);
    void setPath(const std::string _pathname);

    const void display(std::ostream& os);

    virtual const void play() {};
};

#pragma once

#include <memory>

#include "MultimediaObject.h"
#include "Version.h"

class Data;

class Video : public MultimediaObject {

    private:
        int length;

    public:
        virtual ~Video();

        Video();
        Video(const std::string &name, const std::string &pathname, const int &_length);

        int getLength() const;
        void setLength(const int &_length);

        void play() const override;
};
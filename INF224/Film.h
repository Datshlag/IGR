#pragma once

#include "Video.h"
#include "Version.h"

class Data;

class Film : public Video {

    private:
        int* chapters;
        int numberOfChapters;

    public:
        virtual ~Film();

        Film();
        Film (std::string name, 
            std::string pathname, 
            int _length, 
            int* _chapters,
            int _numberOfChapters);

        int getNumberOfChapters() const;
        const int* getChapters() const;

        void setChapters(const int* newChapters, int newChaptersNumber);
        void displayChapters(std::ostream& os) const;
};

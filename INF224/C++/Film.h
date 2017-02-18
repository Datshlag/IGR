#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include "Video.h"
#include "Version.h"

using namespace std;

class Film : public Video {

    private:
        int* chapters;
        int numberOfChapters;

    public:
        virtual ~Film();

        Film();
        Film (string name, 
            string pathname, 
            int _length, 
            int* _chapters,
            int _numberOfChapters);

        int getNumberOfChapters() const;
        const int* getChapters() const;
        string getClassName() const override;

        void setChapters(const int* newChapters, int newChaptersNumber);
        void displayChapters(ostream& os) const;
        void write(ostream& os) const;
        void read(istream &is) override;
};

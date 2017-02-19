#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include "Video.h"
#include "Version.h"

using namespace std;

/**
 * @brief      Film class, extends the video class. Using pointers for learning purpose.
 */
class Film : public Video {

    private:
        int* chapters; /**< The length of the different chapters. */
        int numberOfChapters; /**< The number of chapters. */

    public:
        /**
         * @brief      Destroys the film.
         */
        virtual ~Film();
        /**
         * @brief      Constructs the film.
         */
        Film();
        /**
         * @brief      Constructs the film.
         *
         * @param[in]  name               The name
         * @param[in]  pathname           The pathname
         * @param[in]  _length            The length
         * @param      _chapters          The chapters
         * @param[in]  _numberOfChapters  The number of chapters
         */
        Film (string name, 
            string pathname, 
            int _length, 
            int* _chapters,
            int _numberOfChapters);

        /**
         * @brief      Gets the number of chapters.
         *
         * @return     The number of chapters.
         */
        int getNumberOfChapters() const;
        /**
         * @brief      Gets the chapters.
         *
         * @return     The chapters.
         */
        const int* getChapters() const;
        /**
         * @brief      Gets the class name.
         *
         * @return     The class name.
         */
        string getClassName() const override;

        /**
         * @brief      Sets the chapters.
         *
         * @param[in]  newChapters        The new chapters
         * @param[in]  newChaptersNumber  The new chapters number
         */
        void setChapters(const int* newChapters, int newChaptersNumber);

        /**
         * @brief      Displays the chapter's length.
         *
         * @param      os    The ooutput stream.
         */
        void displayChapters(ostream& os) const;
        /**
         * @brief      Used to write the film's content, in a file for instance.
         *
         * @param      os    The output stream.
         */
        void write(ostream& os) const;
        /**
         * @brief      Used to read the film's content, from a file for instance.
         *
         * @param      is    The input stream.
         */
        void read(istream &is) override;
};

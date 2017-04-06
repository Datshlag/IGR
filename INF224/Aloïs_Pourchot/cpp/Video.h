#pragma once

#include <iostream>
#include <string>

#include "MultimediaObject.h"
#include "Version.h"

using namespace std;

/**
 * @brief      Video class, extends the abstract class MultimediaObject.
 */
class Video : public MultimediaObject {

    private:
        int length; /**< The length of the video. */

    public:
        /**
         * @brief      Destroys the object.
         */
        virtual ~Video();
        /**
         * @brief      Creates the object.
         */
        Video();
        /**
         * @brief      Creates the object.
         *
         * @param[in]  name      The name
         * @param[in]  pathname  The pathname
         * @param[in]  _length   The length
         */
        Video(const string &name, const string &pathname, const int &_length);

        /**
         * @brief      Gets the length.
         *
         * @return     The length.
         */
        int getLength() const;
        /**
         * @brief      Gets the class name.
         *
         * @return     The class name. 
         */
        string getClassName() const override;

        /**
         * @brief      Sets the length.
         *
         * @param[in]  _length  The length
         */
        void setLength(const int &_length);

        /**
         * @brief      Displays info on the video.
         *
         * @param      os    The output stream.
         */
        void display(ostream& os) const override;
        /**
         * @brief      Plays the video using mpv.
         */
        void play() const override;
        /**
         * @brief      Used to write the video's content, in a file for instance.
         *
         * @param      os    The output stream.
         */
        void write(ostream& os) const override;
        /**
         * @brief      Used to read the video's content, from a file for instance.
         *
         * @param      is    The input stream.
         */
        void read(istream& is) override;

};
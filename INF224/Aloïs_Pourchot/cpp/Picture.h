#pragma once

#include <iostream>
#include <string>

#include "MultimediaObject.h"
#include "Version.h"

using namespace std;

/**
 * @brief      Picture class, extends the abstract class MultimediaObject.
 */
class Picture : public MultimediaObject {

    private:
        double latitude; /**< The picture's height. */
        double longitude; /**< The picture's width. */

    public:
        /**
         * @brief      Destroys the picture.
         */
        virtual ~Picture();
        /**
         * @brief      Constructs the picture.
         */
        Picture();
        /**
         * @brief      Constructs the picture.
         *
         * @param[in]  name        The name
         * @param[in]  pathname    The pathname
         * @param[in]  _latitude   The latitude
         * @param[in]  _longitude  The longitude
         */
        Picture(const string &name, const string &pathname, const double &_latitude, const double &_longitude);

        /**
         * @brief      Gets the latitude.
         *
         * @return     The latitude.
         */
        double getLatitude() const;
        /**
         * @brief      Gets the longitude.
         *
         * @return     The longitude.
         */
        double getLongitude() const;
        /**
         * @brief      Gets the class name.
         *
         * @return     The class name.
         */
        string getClassName() const override;

        /**
         * @brief      Sets the latitude.
         *
         * @param[in]  _latitude  The latitude
         */
        void setLatitude(const double &_latitude);
        /**
         * @brief      Sets the longitude.
         *
         * @param[in]  _longitude  The longitude
         */
        void setLongitude(const double &_longitude);

        /**
         * @brief      Displays info on the picture.
         *
         * @param      os    The output stream
         */
        void display(ostream& os) const override;
        /**
         * @brief      Shows the picture using imagej.S
         */
        void play() const override;
        /**
         * @brief      Used to write the picture's content, in a file for instance.
         *
         * @param      os    The output stream.
         */
        void write(ostream &os) const override;
        /**
         * @brief      Used to read the video's content, from a file for instance.
         *
         * @param      is    The input stream.
         */
        void read(istream &is) override;
};

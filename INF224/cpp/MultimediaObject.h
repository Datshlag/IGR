#pragma once

#include <iostream>
#include <string>

using namespace std;

/**
 * @brief      Abstract class for a multimedia object
 */

class MultimediaObject {

    private:
        string name; /**< Name of the object. */
        string pathname; /**< Path of the object. */

    public:
        /**
         * @brief      Destroys the object.
         */
        virtual ~MultimediaObject ();
        /**
         * @brief      Creates the object.
         */
        MultimediaObject();
        /**
         * @brief      Creates the object.
         *
         * @param[in]  name      The name
         * @param[in]  pathname  The pathname
         */
        MultimediaObject(const string &name, const string &pathname);

        /**
         * @brief      Gets the name.
         *
         * @return     The name.
         */
        string getName() const;
        /**
         * @brief      Gets the path.
         *
         * @return     The path.
         */
        string getPath() const;
        /**
         * @brief      Gets the class name.
         *
         * @return     The class name.
         */
        virtual string getClassName() const;

        /**
         * @brief      Sets the name.
         *
         * @param[in]  _name  The name
         */
        void setName(const string &_name);
        /**
         * @brief      Sets the path.
         *
         * @param[in]  _pathname  The pathname
         */
        void setPath(const string &_pathname);

        /**
         * @brief      Displays info on the object.
         *
         * @param      os    The output stream.
         */
        virtual void display(ostream &os) const;
        /**
         * @brief      Plays the object. This method is abstract.
         */
        virtual void play() const = 0;
        /**
         * @brief      Used to write the object's content, in a file for instance.
         *
         * @param      os    The output stream.
         */
        virtual void write(ostream &os) const;
        /**
         * @brief      Used to read the object's content, from a file for instance.
         *
         * @param      is    The input stream.
         */
        virtual void read(istream &is);
};
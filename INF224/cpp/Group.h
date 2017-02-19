#pragma once

#include <list>
#include <string>
#include <memory>

#include "MultimediaObject.h"
#include "Version.h"

using namespace std;

#if VERSION == 8
	/**
	 * @brief      Group class extends the std::list class. We store pointers to multimedia objects which can lead to memory leaks 
	 * 			   if we don't pay enough attention, so we later use shared_ptr instead.
	 */
	class Group : public list<MultimediaObject*> {

		private:
			string groupName; /**< The group name. */

		public:
			/**
			 * @brief      Destroys the group.
			 */
			~Group();
			/**
			 * @brief      Creates the group.
			 *
			 * @param[in]  _groupName  The group name
			 */
			Group(string _groupName);
			/**
			 * @brief      Gets the name.
			 *
			 * @return     The name.
			 */
			string getName() const;

			/**
			 * @brief      Displays info on all elements of the group.
			 *
			 * @param      os    The ouput stream.
			 */
			void displayElements(ostream& os) const;
	};

#elif VERSION >= 9

	typedef shared_ptr<MultimediaObject> MOPtr;

	/**
	 * @brief      Group class extends the std::list class. We store shared_ptr of multimedia objects to prevent any memory leak.
	 */
	class Group : public list<MOPtr> {

		private:
			string groupName; /**< The group name. */

		public:
			/**
			 * @brief      Destroys the group.
			 */
			~Group();
			/**
			 * @brief      Creates the group.
			 *
			 * @param[in]  _groupName  The group name
			 */
			Group(string _groupName);
			/**
			 * @brief      Gets the name.
			 *
			 * @return     The name.
			 */
			string getName() const;

			/**
			 * @brief      Displays info on all elements of the group.
			 *
			 * @param      os    The ouput stream.
			 */
			void displayElements(ostream& os) const;
	};

#endif
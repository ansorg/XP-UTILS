/*
 * NavEntry.h
 *
 *  Created on: 02.02.2013
 *      Author: Jens
 */

#ifndef NAVENTRY_H_
#define NAVENTRY_H_

#include "Utils.h"
#include "XPLMNavigation.h"

namespace XPUtilsPlugin {

	class NavEntry {
	public:
		NavEntry();
		virtual ~NavEntry();

		XPLMNavRef navRef;
		XPLMNavType navType;
		std::string navName;
		std::string navId;
		bool isRegional;
	};

} /* namespace FMS */
#endif /* NAVENTRY_H_ */

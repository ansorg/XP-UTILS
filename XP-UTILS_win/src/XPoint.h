#ifndef POINT_H_
#define POINT_H_

/*

 Copyright 2008, Jens Ansorg - x-plane@ansorg-web.de


 This file is part of X-Plane Airport Navigator Plugin.

 Apt-Navigator is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Apt-Navigator is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Apt-Navigator.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "Utils.h"

namespace XPUtilsPlugin {

#define RADIUS 6371
#define FEET_METER 3.2808

	class XPoint {
		private:

			double normalized(double&);

		public:
			double z;
			double lat;
			double lon;

			XPoint();
			XPoint(const double lat, const double lon);
			virtual ~XPoint();

			void debug();
			void debug(bool);

			XPoint getPointAt(const float angle, const float distance);

			float getPointDistance(const XPoint& p);
			float getPointBearing(const XPoint& p);

			static double degToRad(const double deg) {
				return deg * M_PI / 180;
			}

			static double radToDeg(const double& rad) {
				return rad * 180 / M_PI;
			}

			static double dist(const double& lat1, const double& lon1, const double& lat2, const double& lon2) {
				return acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon2 - lon1)) * RADIUS;
			}

			static double bearing(const double& lat1, const double& lon1, const double& lat2, const double& lon2) {

				/*
				 double Pi2 = M_PI * 2;
				 return fmod(atan2(sin(lon1-lon2)*cos(lat2),cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), Pi2);
				 */

				double b = atan2(sin(lon2 - lon1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1)) * 180 / M_PI;
				return fmod(b + 360, 360);
			}
	};

}

#endif /*POINT_H_*/

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
#include "XPoint.h"

namespace XPUtilsPlugin {

	XPoint XPoint::getPointAt(const float heading, const float dist) {
		//cout << "initial XPoint lat: " << lat << " lon: " << lon << " - heading: " << heading << " dist: " << dist << endl;
		double lat = degToRad(this->lat);
		double lon = degToRad(this->lon);
		double brng = degToRad(heading);
		double d = dist / 1000;

		double sinLat = sin(lat);
		double cosDistRad = cos(d / RADIUS);
		double cosLat = cos(lat);
		double sinDistRad = sin(d / RADIUS);
		double cosBrng = cos(brng);

		double lat2 = asin(sinLat * cosDistRad + cosLat * sinDistRad * cosBrng);
		double lon2 = lon + atan2(sin(brng) * sin(d / RADIUS) * cos(lat), cos(d / RADIUS) - sin(lat) * sin(lat2));
		lon2 = fmod(lon2 + M_PI, 2 * M_PI) - M_PI;
		/*
		 double lat2 = asin(sin(lat)*cos(d)+cos(lat)*sin(d)*cos(brng));
		 double lon2;
		 */
		lat2 = radToDeg(lat2);
		lon2 = radToDeg(lon2);

		//cout << "Moved XPoint   lat: " << lat2 << " lon: " << lon2 << " distance: " << getPointDistance(XPoint(lat2, lon2)) << endl;
		return XPoint(lat2, lon2);
	}

	float XPoint::getPointDistance(const XPoint& p) {
		return dist(degToRad(lat), degToRad(lon), degToRad(p.lat), degToRad(p.lon));
	}

	float XPoint::getPointBearing(const XPoint& p) {
		return bearing(degToRad(lat), degToRad(lon), degToRad(p.lat), degToRad(p.lon));
	}

	double XPoint::normalized(double& in) {
		return in;// * 10000;
	}

	void XPoint::debug(bool endline) {
		if (endline) {
			printf("lat: %f - lon: %f\n", normalized(lat), normalized(lon));
		} else {
			printf("lat: %f - lon: %f ", normalized(lat), normalized(lon));
		}
	}

	void XPoint::debug() {
		debug(false);
	}

	XPoint::XPoint() {
		lat = lon = z = 0.0;
	}

	XPoint::XPoint(const double latIn, const double lonIn) {
		lat = latIn;
		lon = lonIn;
		z = 0.0;
	}

	XPoint::~XPoint() {
	}

}

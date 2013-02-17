/*
 * XP-Utils.h
 *
 *  Created on: 20.01.2013
 *      Author: Jens
 */

#ifndef FMS_H_
#define FMS_H_

#include "Utils.h"
#include "XPoint.h"
#include "FMSWindow.h"
#include "TextWindow.h"

namespace XPUtilsPlugin {

	class XPUtils {
		private:
			float callBackInterval;
			double nextTextWindowUpdate;
			XPLMDataRef dRefPlaneLat, dRefPlaneLon, dRefPlaneAgl, dRefPlanePsi, dRefPlaneMagPsi;
			XPLMDataRef dRefBaroSealevelInhg ;//sim/weather/barometer_sealevel_inhg
			XPLMDataRef dRefGaugeBaroPilot; //sim/cockpit2/gauges/actuators/barometer_setting_in_hg_pilot
			XPoint planePosition;

			/* widgets */
			FMSWindow * pFmsWindow;
			TextWindow * pTextWindow;

			static const float BARO_29_92 = 29.92;

			void handleSetBarometer(int);


		public:
			XPUtils();
			virtual ~XPUtils();
			/* plugin methods */
			//void handleMenu(int menuId);
			void handleHotkey(int keyId);
			void pluginStart();
			void pluginEnd();
			void showTextMessage(std::string, int type, int duration);

			FMSWindow& getFmsWindow();
			TextWindow& getTextWindow();
			float flightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon);

			enum commandIdRefs {
				COMMAND_SHOW_FMSWINDOW, COMMAND_SET_BAROMETER, COMMAND_SET_BARO_STD
			};

			XPLMHotKeyID showFmsWindow, setBarometer, setBaroStd;
	};

	extern XPUtils xpUtils;

} /* namespace FMS */
#endif /* FMS_H_ */

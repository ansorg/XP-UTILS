/*
 * XP-Utils.cpp
 *
 *  Created on: 20.01.2013
 *      Author: Jens
 */

#include "XP-Utils.h"

using namespace XPUtilsPlugin;

namespace XPUtilsPlugin {
	XPUtils xpUtils;

	XPUtils::XPUtils() {
		// TODO Auto-generated constructor stub
		callBackInterval = -1;
		nextTextWindowUpdate = 0.0;
		planePosition = XPoint();
	}

	XPUtils::~XPUtils() {
		// TODO Auto-generated destructor stub
	}

	void XPUtils::showTextMessage(std::string message, int type, int duration = 0){
		getTextWindow().showMsg(message, type, duration);
	}

	void XPUtils::handleHotkey(int keyId) {
		switch (keyId) {
		case COMMAND_SHOW_FMSWINDOW:
			planePosition.lat = XPLMGetDataf(dRefPlaneLat);
			planePosition.lon = XPLMGetDataf(dRefPlaneLon);
			getFmsWindow().show(planePosition);
			break;
		case COMMAND_SET_BARO_STD:
		case COMMAND_SET_BAROMETER:
			handleSetBarometer(keyId);
			break;
		default:
			break;
		}
	}

	void XPUtils::handleSetBarometer(int cmdId) {

		float baroCurrentInhg = XPLMGetDataf(dRefBaroSealevelInhg);
		float gaugeBaroInhg = XPLMGetDataf(dRefGaugeBaroPilot);

		if (cmdId == COMMAND_SET_BAROMETER) {
			std::cout << "setting to actual pressure" << std::endl;
			XPLMSetDataf(dRefGaugeBaroPilot, baroCurrentInhg);
		} else {
			std::cout << "setting to standard pressure" << std::endl;
			XPLMSetDataf(dRefGaugeBaroPilot, BARO_29_92);
		}
		//read the value again and show it
		gaugeBaroInhg = XPLMGetDataf(dRefGaugeBaroPilot);
		//XPLMSpeakString(Utils::floatToString(gaugeBaroInhg).c_str());
		//showTextMessage(Utils::floatToString(gaugeBaroInhg), MESSAGE_TYPE_BARO, 3);
		char baroChar[6];
		sprintf(baroChar, "%.2f", gaugeBaroInhg);
		showTextMessage(Utils::charToString(baroChar), MESSAGE_TYPE_BARO, 3);
	}

	void XPUtils::pluginStart() {
		Utils::log("XPUtils::pluginStart", LOG_INFO);
		dRefPlaneLat = XPLMFindDataRef("sim/flightmodel/position/latitude");
		dRefPlaneLon = XPLMFindDataRef("sim/flightmodel/position/longitude");
		dRefBaroSealevelInhg = XPLMFindDataRef("sim/weather/barometer_sealevel_inhg");
		dRefGaugeBaroPilot = XPLMFindDataRef("sim/cockpit2/gauges/actuators/barometer_setting_in_hg_pilot");
	}

	void XPUtils::pluginEnd() {
		XPLMUnregisterDataAccessor(dRefBaroSealevelInhg);
		XPLMUnregisterDataAccessor(dRefGaugeBaroPilot);
		XPLMUnregisterDataAccessor(dRefPlaneLat);
		XPLMUnregisterDataAccessor(dRefPlaneLon);
	}

	FMSWindow& XPUtils::getFmsWindow() {
		if (pFmsWindow == NULL) {
			pFmsWindow = new FMSWindow();
		}
		return *pFmsWindow;
	}

	TextWindow& XPUtils::getTextWindow(){
		if (pTextWindow == NULL) {
			pTextWindow = new TextWindow();
		}
		return *pTextWindow;
	}

	float XPUtils::flightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon) {
		if(XPLMGetElapsedTime() >= nextTextWindowUpdate && nextTextWindowUpdate != 0.0) {
			Utils::log( "hide called after " + Utils::floatToString(inElapsedSinceLastCall));
			getTextWindow().hideMsg();
			nextTextWindowUpdate = 0.0;
		}
		if (getTextWindow().getMessageDuration() > 0) {
			nextTextWindowUpdate = XPLMGetElapsedTime() + getTextWindow().getMessageDuration();
			Utils::log("do hide at " + Utils::doubleToString(nextTextWindowUpdate));
			return getTextWindow().getMessageDuration();
		} else {
			return callBackInterval;
		}
	}

} /* namespace FMS */

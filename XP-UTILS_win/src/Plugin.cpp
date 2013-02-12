#include "Plugin.h"

/*

 Copyright 2009, Jens Ansorg - x-plane@ansorg-web.de


 This file is part of FOV-Loader.

 FOV-Loader is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 FOV-Loader is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with FOV-Loader.  If not, see <http://www.gnu.org/licenses/>.

 */

/*
 * XPluginStart
 *
 * Our start routine registers our window and does any other initialization we
 * must do.
 *
 */

using namespace XPUtilsPlugin;

PLUGIN_API __attribute__((visibility("default"))) int XPluginStart(char * outName, char * outSig, char * outDesc) {
	/* First we must fill in the passed in buffers to describe our
	 * plugin to the plugin-system. */

	strcpy(outName, const_cast<char *> (Utils::getFullPluginName().c_str()));
	strcpy(outSig, PLUGINSIG);
	strcpy(outDesc, PLUGINDESC);

	Utils::log("XPluginStart", LOG_INFO);

	int keyFlag = xplm_DownFlag + xplm_ShiftFlag + xplm_ControlFlag;
	xpUtils.showFmsWindow = XPLMRegisterHotKey(XPLM_VK_F, keyFlag, "show FMS window", FMSHotKeyCallback, (void*) xpUtils.COMMAND_SHOW_FMSWINDOW);
	xpUtils.setBaroStd = XPLMRegisterHotKey(XPLM_VK_B, keyFlag, "set actual Baro", FMSHotKeyCallback, (void*) xpUtils.COMMAND_SET_BARO_STD);
	xpUtils.setBarometer = XPLMRegisterHotKey(XPLM_VK_B, xplm_DownFlag + xplm_ControlFlag, "set standard Baro", FMSHotKeyCallback, (void*) xpUtils.COMMAND_SET_BAROMETER);

	xpUtils.pluginStart();

	/* We must return 1 to indicate successful initialization, otherwise we
	 * will not be called back again. */
	return 1;
}

/*
 * XPluginStop
 *
 * Our cleanup routine deallocates our window.
 *
 */
PLUGIN_API __attribute__((visibility("default"))) void XPluginStop(void) {
	Utils::log("XPluginStop", LOG_INFO);
	xpUtils.pluginEnd();
}

/*
 * XPluginDisable
 *
 * We do not need to do anything when we are disabled, but we must provide the handler.
 *
 */
PLUGIN_API __attribute__((visibility("default"))) void XPluginDisable(void) {
	XPLMUnregisterFlightLoopCallback(FlightLoopCallback, NULL);
}

/*
 * XPluginEnable.
 *
 * We don't do any enable-specific initialization, but we must return 1 to indicate
 * that we may be enabled at this time.
 *
 */
PLUGIN_API __attribute__((visibility("default"))) int XPluginEnable(void) {
	Utils::log("XPluginEnable", LOG_INFO);
	XPLMRegisterFlightLoopCallback(FlightLoopCallback, 1, NULL);
	return 1;
}

/*
 * XPluginReceiveMessage
 *
 * We don't have to do anything in our receive message handler, but we must provide one.
 *
 */
PLUGIN_API __attribute__((visibility("default"))) void XPluginReceiveMessage(XPLMPluginID inFromWho, intptr_t inMessage, void * inParam) {
}

namespace XPUtilsPlugin {

	float FlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon) {
		return xpUtils.flightLoopCallback(inElapsedSinceLastCall, inElapsedTimeSinceLastFlightLoop, inCounter, inRefcon);
	}

	void FMSHotKeyCallback(void * inRefcon) {
		int keyId = (intptr_t) inRefcon;
		xpUtils.handleHotkey(keyId);
	}

	/* global handlers */
	int prefWinHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2) {
		if (inMessage == xpMsg_Create)
			return 0; // prevent endless loop
		return xpUtils.getFmsWindow().handler(inMessage, inWidget, inParam1, inParam2);
	}
	int prefTextBoxHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2) {
		if (inMessage == xpMsg_Create)
			return 0; // prevent endless loop
		return xpUtils.getFmsWindow().TextFieldHandler(inMessage, inWidget, inParam1, inParam2);
	}

	void TextWindowDrawWindowCallback(XPLMWindowID inWindowID, void* inRefcon) {
		xpUtils.getTextWindow().DrawCallback(inWindowID, inRefcon);
	}

	void TextWindowHandleKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void* inRefcon, int losingFocus) {
	}

	int TextWindowHandleMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void* inRefcon) {
		return xpUtils.getTextWindow().MouseCallback(inWindowID, x, y, inMouse, inRefcon);
	}

}

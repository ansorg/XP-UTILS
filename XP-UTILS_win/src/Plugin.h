#ifndef PLUGIN_H_
#define PLUGIN_H_

/*

 Copyright 2013, Jens Ansorg - x-plane@ansorg-web.de


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

#include "XP-Utils.h"

namespace XPUtilsPlugin {
	float FlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon);
	void FMSHotKeyCallback(void * inRefcon);
	int prefWinHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2);
	int prefTextBoxHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2);

	void TextWindowDrawWindowCallback(XPLMWindowID inWindowID, void * inRefcon);
	void TextWindowHandleKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus);
	int TextWindowHandleMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon);

}
#endif /*PLUGIN_H_*/

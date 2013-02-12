/*
 * FMSWindow.cpp
 *
 *  Created on: 20.01.2013
 *      Author: Jens
 */

#include "FMSWindow.h"
#include "Plugin.h"

namespace XPUtilsPlugin {

	FMSWindow::FMSWindow() {
		width = 600;
		height = 100;

		int x = 100;
		int y = 200;
		int x2 = x + width;
		int y2 = y - height;

		windowWidget = XPCreateWidget(x, y, x2, y2, 0, // Visible
				//utils.strToChar(utils.getPluginName() + " preferences"), // desc
				"Enter the flight plan data:",//descr
				1, // root
				NULL, // no container
				xpWidgetClass_MainWindow);
		//XPSetWidgetProperty(windowWidget, xpProperty_MainWindowType, xpMainWindowStyle_Translucent);
		XPSetWidgetProperty(windowWidget, xpProperty_MainWindowType, xpMainWindowStyle_MainWindow);
		XPSetWidgetProperty(windowWidget, xpProperty_MainWindowHasCloseBoxes, 1);

		txtFMSEntry = XPCreateWidget(x + WIDGET_LEFT_POS, y - 25, x2 - WIDGET_BORDER_WIDTH - 10, y - 65, 1, "", 0, windowWidget, xpWidgetClass_TextField);
		XPSetWidgetProperty(txtFMSEntry, xpProperty_TextFieldType, xpTextEntryField);

		//apply button
		btnApply = XPCreateWidget(x2 - 100, y2 + WIDGET_BORDER_WIDTH + 25, x2 - WIDGET_BORDER_WIDTH - 10, y2 + WIDGET_BORDER_WIDTH + 0, 1, "Apply", 0,
				windowWidget, xpWidgetClass_Button);
		XPSetWidgetProperty(btnApply, xpProperty_ButtonType, xpPushButton);

		//cancel button
		btnCancel = XPCreateWidget(x + WIDGET_LEFT_POS, y2 + WIDGET_BORDER_WIDTH + 25, x + WIDGET_LEFT_POS + 50, y2 + WIDGET_BORDER_WIDTH + 0, 1, "Cancel", 0,
				windowWidget, xpWidgetClass_Button);
		XPSetWidgetProperty(btnCancel, xpProperty_ButtonType, xpPushButton);

		XPAddWidgetCallback(txtFMSEntry, prefTextBoxHandler);
		XPAddWidgetCallback(windowWidget, prefWinHandler);
	}

	FMSWindow::~FMSWindow() {
		// TODO Auto-generated destructor stub
		if (windowWidget) {
			XPLMDestroyWindow(windowWidget);
		}
	}

	void FMSWindow::show(XPoint planePosition) {
		//if there is no FMS entry yet, get the current airport as first value suggestion
		long nEntries = XPLMCountFMSEntries();
		if (nEntries == 0) {
			char outID[32];
			char outName[256];

			float lat = planePosition.lat;
			float lon = planePosition.lon;

			XPLMNavRef airport = XPLMFindNavAid(NULL, NULL, &lat, &lon, NULL, xplm_Nav_Airport);

			if (airport != XPLM_NAV_NOT_FOUND) {
				XPLMGetNavAidInfo(airport, NULL, &lat, &lon, NULL, NULL, NULL, outID, outName, NULL);
				std::cout << outID << ", " << outName << std::endl;
				planePosition.lat = lat;
				planePosition.lon = lon;
			} else {
				Utils::log("no airport found at current position");
			}
			XPSetWidgetDescriptor(txtFMSEntry, outID);
		}

		if (!XPIsWidgetVisible(windowWidget)) {
			//Utils::log("PrefsWindow::show() - make visible");
			XPShowWidget(windowWidget);
		}
		XPBringRootWidgetToFront(txtFMSEntry);
		XPSetKeyboardFocus(txtFMSEntry);
	}

	void FMSWindow::hide() {
		char Buffer[256];

		XPGetWidgetDescriptor(windowWidget, Buffer, sizeof(Buffer));
		Utils::log(Buffer);

		if (XPIsWidgetVisible(windowWidget)) {
			XPHideWidget(windowWidget);
		}
	}

	int FMSWindow::handler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2) {

		if (inMessage == xpMessage_CloseButtonPushed) {
			//Utils::log("xpMessage_CloseButtonPushed");
			hide();
			return 1;
		}

		// Test for a button pressed
		if (inMessage == xpMsg_PushButtonPressed) {
			if (inParam1 == (intptr_t) btnApply) {
				handleApplyMsg();
				hide();
				return 1;
			}

			if (inParam1 == (intptr_t) btnCancel) {
				hide();
				return 1;
			}
		}

		return 0;
	}

	void FMSWindow::handleApplyMsg() {
		char buff[256];
		char debug_str[160];

		XPGetWidgetDescriptor(txtFMSEntry, buff, sizeof(buff));

		std::string widgetText = "";
		std::string announceMsg = "Seting Flightpath: ";

		std::cout << "fmsText: " << buff << std::endl;

		std::vector<std::string> fmsEntries = Utils::split(buff, ' ');

		for (std::vector<std::string>::iterator it = fmsEntries.begin(); it != fmsEntries.end(); ++it) {
			std::cout << "element: " << *it << std::endl;
		}

		long nEntries = XPLMCountFMSEntries();
		for (int n = nEntries - 1; n >= 0; --n) {
			XPLMClearFMSEntry(n);
		}
		int fmsEntryCount = 0;
		for (unsigned int i = 0; i < fmsEntries.size(); ++i) {

			std::vector<std::string> fmsTuple = Utils::split(fmsEntries[i].c_str(), '/');

			std::string navAid = Utils::StringToUpper(fmsTuple[0]);

			NavEntry * navref = findNavAid(navAid);

			if(navref) {
				std::cout << "found navaid: " << navref->navId << std::endl;

				sprintf(debug_str, "NAVAID: [%d] Info = %s - %s (%u) [%s]", i, navref->navId.c_str(), navref->navName.c_str(), navref->navType, navref->isRegional ? "R" : "G");
				Utils::log(debug_str, LOG_INFO);
				widgetText += navref->navId;
				announceMsg += navref->navName;

				int altitude = 10000;
				if (fmsTuple.size() == 2) {
					altitude = atoi(fmsTuple[1].c_str());
					widgetText += "/" + Utils::intToString(altitude);
				}
				if (i + 1 < fmsEntries.size()) {
					//add a space if not the last entry
					widgetText += " ";
					announceMsg += " to ";
				}
				XPLMSetFMSEntryInfo(fmsEntryCount++, navref->navRef, altitude);

			} else {
				std::cout << "not found: " << navAid << "" << std::endl;
			}
		}
		std::cout << "new FMS Text: " << widgetText << std::endl;
		//XPLMSpeakString(announceMsg.c_str());
		XPSetWidgetDescriptor(txtFMSEntry, widgetText.c_str());
		xpUtils.showTextMessage(announceMsg, MESSAGE_TYPE_FMS, 5);
	}

	/*
	 * Find a navaid for the given ID
	 * return only distinct results.
	 */
	NavEntry * FMSWindow::findNavAid(std::string navItem) {
		XPLMNavRef ref = XPLMGetFirstNavAid();
		char navName[256];
		char navId[32];
		char navReg[1];
		XPLMNavType navType;
		int counter = 0;
		std::vector<NavEntry*> navEntries;
		do {
			ref = XPLMGetNextNavAid(ref);
			XPLMGetNavAidInfo(ref, &navType, NULL, NULL, NULL, NULL, NULL, navId, navName, navReg);
			if (navId == navItem) {
				std::cout << "found [" << navId << ", " << navName << "]" << std::endl;
				NavEntry * navEntry = new NavEntry();
				navEntry->isRegional = (navReg[0] == 1);
				navEntry->navId = navId;
				navEntry->navName = navName;
				navEntry->navType = navType;
				navEntry->navRef = ref;
				navEntries.push_back(navEntry);
				//break;
			}
			counter++;
		} while (ref != XPLM_NAV_NOT_FOUND);
		std::cout << "looped through " << counter << " navaids" << std::endl;

		if (navEntries.size() == 1) {
			return navEntries.at(0);
		} else {
			return NULL;
		}
	}

	int FMSWindow::TextFieldHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2) {
		if (inMessage == xpMsg_KeyPress) {
			//char theChar = KEY_CHAR(inParam1);
			unsigned char theKey = KEY_VKEY(inParam1);
			//bool upKey = KEY_FLAGS(inParam1) & xplm_UpFlag;
			//bool downKey = KEY_FLAGS(inParam1) & xplm_DownFlag;

			//Utils::log(theChar);
			//Utils::log(theKey);

			//std::cout << "theChar: " << theChar << std::endl;
			//std::cout << "theKey: " << theKey << std::endl;

			//handle the RETURN key
			if (theKey == XPLM_VK_RETURN) {
				handleApplyMsg();
				hide();
				return 1;
			}

			//handle the escape key
			if (theKey == XPLM_KEY_ESCAPE) {
				hide();
				return 1;
			}

			//check for valid entry => numbers. decimal, delete
			/*
			 if ((theChar < XPLM_KEY_0 || theChar > XPLM_KEY_9) && theChar != XPLM_KEY_DECIMAL && theChar != XPLM_KEY_DELETE && theChar != XPLM_KEY_LEFT
			 && theChar != XPLM_KEY_RIGHT) {//
			 return 1; //eat the char
			 }
			 */
		}

		return 0;
	}

} /* namespace FMS */

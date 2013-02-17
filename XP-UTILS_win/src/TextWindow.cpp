/*
 * TextWindow.cpp
 *
 *  Created on: 02.02.2013
 *      Author: Jens
 */

#include "TextWindow.h"
#include "Plugin.h"

namespace XPUtilsPlugin {

	TextWindow::TextWindow() {
		Utils::log("TextWindow::TextWindow");
		msgToDraw = "Hello World";
		msgDuration = 0;
		windowType = MESSAGE_TYPE_BARO;
		hideIsPending = false;
		textWindow = XPLMCreateWindow(20, 600, 800, 570, /* Area of the window. */
		0, /* Start invisible. */
		TextWindowDrawWindowCallback, /* Callbacks */
		TextWindowHandleKeyCallback, TextWindowHandleMouseClickCallback, NULL); /* Refcon - not used. */
	}

	TextWindow::~TextWindow() {
		// TODO Auto-generated destructor stub
		if (textWindow) {
			XPLMDestroyWindow(textWindow);
		}
	}

	int TextWindow::MouseCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void* inRefcon) {
		hide();
		return 1;
	}

	void TextWindow::showMsg(std::string message, int type, int duration = 0) {
		Utils::log("TextWindow::showMsg");
		msgToDraw = message;
		msgDuration = duration;
		windowType = type;
		switch (windowType) {
		case MESSAGE_TYPE_FMS:
			XPLMSetWindowGeometry(textWindow , 20, 600, 500, 570);
			break;
		case MESSAGE_TYPE_BARO:
			XPLMSetWindowGeometry(textWindow, 20, 600, 60, 570);
			break;
		default:
			break;
		}
		show();
		if (duration > 0) {
#ifdef DEBUG
			XPLMDebugString("start timer to hide textmessage");
#endif
			hideIsPending = TRUE;
		}
	}

	void TextWindow::DrawCallback(XPLMWindowID inWindowID, void* inRefcon) {
		int left, top, right, bottom;
		float color[] = { 1.0, 1.0, 1.0 }; /* RGB White */

		/* First we get the location of the window passed in to us. */
		XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

		/* We now use an XPLMGraphics routine to draw a translucent dark
		 * rectangle that is our window's shape. */
		XPLMDrawTranslucentDarkBox(left, top, right, bottom);

		/* Finally we draw the text into the window, also using XPLMGraphics
		 * routines.  The NULL indicates no word wrapping. */
		XPLMDrawString(color, left + 5, top - 20, (char *) msgToDraw.c_str(), NULL, xplmFont_Basic);

	}

} /* namespace FMSPlugin */

/*
 * TextWindow.h
 *
 * Created on: 02.02.2013
 * Author: Jens
 */

#ifndef TEXTWINDOW_H_
#define TEXTWINDOW_H_

#include "Utils.h"
#include "XPLMGraphics.h"

namespace XPUtilsPlugin {

	class TextWindow {
		private:
			XPLMWindowID textWindow;
			std::string msgToDraw;
			int msgDuration;
			int windowType;
			bool hideIsPending;

			void show() {XPLMSetWindowIsVisible(textWindow, 1);};
			void hide() {
				XPLMSetWindowIsVisible(textWindow, 0);
				msgDuration = 0;
				hideIsPending = false;
			};

		public:
			TextWindow();
			virtual ~TextWindow();
			void DrawCallback(XPLMWindowID inWindowID, void * inRefcon);
			int MouseCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void *inRefcon);

			void showMsg(std::string message, int type, int duration);
			void hideMsg() {hide();};
			int getMessageDuration() { return msgDuration; };
			bool getHideIsPending() { return hideIsPending; };
	};

} /* namespace FMSPlugin */
#endif /* TEXTWINDOW_H_ */

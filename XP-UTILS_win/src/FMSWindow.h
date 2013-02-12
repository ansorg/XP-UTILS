/*
 * FMSWindow.h
 *
 *  Created on: 20.01.2013
 *      Author: Jens
 */

#ifndef FMSWINDOW_H_
#define FMSWINDOW_H_

#include "Utils.h"
#include "XPoint.h"
#include "NavEntry.h"
#include "XPWidgetUtils.h"
#include "XPLMNavigation.h"

namespace XPUtilsPlugin {

	enum textMessageType {
		MESSAGE_TYPE_FMS,
		MESSAGE_TYPE_BARO
	};

	class FMSWindow {
		private:

			XPLMWindowID windowWidget;
			XPWidgetID btnApply, btnCancel, txtFMSEntry;

			int width;
			int height;

			void handleApplyMsg();
			NavEntry * findNavAid(std::string);

		public:
			FMSWindow();
			virtual ~FMSWindow();

			void show(XPoint);
			void hide();
			int handler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2);
			int TextFieldHandler(XPWidgetMessage inMessage, XPWidgetID inWidget, intptr_t inParam1, intptr_t inParam2);


	};

} /* namespace FMS */
#endif /* FMSWINDOW_H_ */

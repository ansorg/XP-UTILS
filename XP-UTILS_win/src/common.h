#ifndef COMMON_H_
#define COMMON_H_

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
 along with FMS.  If not, see <http://www.gnu.org/licenses/>.

 */

/*
 * */
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <cstdlib>

/// Sandy Barbour
/// Handles all platform OpenGL headers.
#if IBM
#include <gl\gl.h>
#include <gl\glu.h>
#elif LIN
#define TRUE 1
#define FALSE 0
#include <GL/gl.h>
#include <GL/glu.h>
#else
#define TRUE 1
#define FALSE 0
#if __GNUC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <gl.h>
#include <glu.h>
#endif
#endif
/// Sandy Barbour

#if APL
#include <Carbon/Carbon.h>
#endif

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"
#include "XPLMCamera.h"
#include "XPLMDataAccess.h"
#include "XPLMDisplay.h"
#include "XPLMMenus.h"
#include "XPLMProcessing.h"
#include "XPLMPlugin.h"
#include "XPLMPlanes.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"

#define WIDGET_BORDER_WIDTH 15
#define WIDGET_TITLE_HEIGHT 25

#define WIDGET_LEFT_POS 25
#define WIDGET_LBL_HEIGHT 20

#define PLUGINNAME "XP-Utils"
#define PLUGINVERSION "1.0.1 "
#define PLUGINSIG "ansorg.xplane.xputils"
#define PLUGINDESC "Various Helpers: FMS waypoint entry, Baro-Setting, ..."
#define PLUGINPREFSEXT ".ini"

#endif /*COMMON_H_*/

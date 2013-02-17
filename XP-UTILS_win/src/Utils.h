#ifndef UTILS_H_
#define UTILS_H_

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

#include "common.h"

#define CR 13            /* Decimal code of Carriage Return char */
#define LF 10            /* Decimal code of Line Feed char */
#define EOF_MARKER 26    /* Decimal code of DOS end-of-file marker */
#define MAX_REC_LEN 2048 /* Maximum size of input buffer */

namespace XPUtilsPlugin {
	/*
	 * file as per http://www.mrx.net/c/readfunctions.html
	 *
	 * */
	int T_fgetc(FILE *InputFile, char *ReadBuffer);

	enum LOGMODE {
		LOG_INFO, LOG_DEBUG, LOG_ERROR
	};

	class Utils {
	public:
		Utils();
		virtual ~Utils();

		//http://stackoverflow.com/a/10467633/1856755
		static std::string currentDateTime() {
		    time_t     now = time(0);
		    struct tm  tstruct;
		    char       buf[80];
		    tstruct = *localtime(&now);
		    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
		    // for more information about date/time format
		    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
		    return buf;
		}

		/* utility methods */
		static std::string doubleToString(double x) {
			std::ostringstream o;
			if (!(o << x)) {
				//throw ConversionException("stringify(float)");
			}
			return o.str();
		}

		static std::string floatToString(float x) {
			std::ostringstream o;
			if (!(o << x)) {
				//throw ConversionException("stringify(float)");
			}
			return o.str();
		}

		static std::string charToString(char * c) {
			std::string s = c;
			return s;
		}

		static std::string intToString(int x) {
			std::ostringstream o;
			if (!(o << x)) {
				//throw ConversionException("stringify(float)");
			}
			return o.str();
		}

		static float stringToFloat(std::string s) {
			std::istringstream is(s);
			float f;
			is >> f;
			return f;
		}

		static double stringToDouble(std::string s) {
			std::istringstream is(s);
			double d;
			is >> d;
			return d;
		}

		static const char* strToChar(std::string in) {
			return const_cast<char *>(in.c_str());
		}

		static std::string strAppend(char* c1, char* c2) {
			char buff[strlen(c1) + strlen(c2)];
			strcpy(buff, c1);
			strcat(buff, c2);
			return buff;
		}

		static std::vector<std::string> split(const char *str, char c = ' ') {
			std::vector<std::string> result;

			do {
				const char *begin = str;

				while (*str != c && *str)
					str++;

				result.push_back(std::string(begin, str));
			} while (0 != *str++);

			return result;
		}

		static std::string StringToUpper(std::string strToConvert) {
			//change each element of the string to upper case
			for (unsigned int i = 0; i < strToConvert.length(); i++) {
				strToConvert[i] = toupper(strToConvert[i]);
			}
			return strToConvert; //return the converted string
		}

		static std::string StringToLower(std::string strToConvert) {
			//change each element of the string to lower case
			for (unsigned int i = 0; i < strToConvert.length(); i++) {
				strToConvert[i] = tolower(strToConvert[i]);
			}
			return strToConvert; //return the converted string
		}

		static void log(std::string msg, int mode = LOG_DEBUG) {
			switch (mode) {
			case LOG_INFO: {
				std::string out = Utils::getFullPluginName() + " [INFO]: " + msg + "\n";
				XPLMDebugString(const_cast<char *>(out.c_str()));
			}
				break;
			case LOG_ERROR: {
				std::string out = Utils::getFullPluginName() + " [ERROR]: " + msg + "\n";
				XPLMDebugString(const_cast<char *>(out.c_str()));
			}
				break;
			case LOG_DEBUG: {
#ifdef DEBUG
				std::string out = Utils::getFullPluginName() + " [DEBUG]: " + msg + "\n";
				XPLMDebugString(const_cast<char *>(out.c_str()));
#endif
			}
				break;
			default:
				break;
			}
		}
		static void log(int val) {
			log(Utils::intToString(val));
		}
		static void log(double val) {
			log(Utils::doubleToString(val));
		}

		static std::string getFullPluginName() {
#ifdef PLUGINNAME
#ifdef PLUGINVERSION
#ifdef DEBUG
			char cBuff[strlen(PLUGINNAME) + strlen(PLUGINVERSION) + 2 + 8];
			strcpy(cBuff, PLUGINNAME);
			strcat(cBuff, " v");
			strcat(cBuff, PLUGINVERSION);
			strcat(cBuff, " (DEBUG)");
#else
			char cBuff[strlen(PLUGINNAME) + strlen(PLUGINVERSION) + 2];
			strcpy(cBuff, PLUGINNAME);
			strcat(cBuff, " v");
			strcat(cBuff, PLUGINVERSION);
#endif
			return cBuff;
#else
			return "undefined";
#endif
#else
			return "undefined";
#endif
		}

		/// Sandy Barbour
		/// OSX Mach-O plugins need the path to be converted to the proper format.
		//---------------------------------------------------------------------------

#if APL && __MACH__
#include <Carbon/Carbon.h>
		static int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen)
		{
			CFStringRef inStr = CFStringCreateWithCString(kCFAllocatorDefault, inPath ,kCFStringEncodingMacRoman);
			if (inStr == NULL)
				return -1;
			CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, inStr, kCFURLHFSPathStyle,0);
			CFStringRef outStr = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
			if (!CFStringGetCString(outStr, outPath, outPathMaxLen, kCFURLPOSIXPathStyle))
				return -1;
			CFRelease(outStr);
			CFRelease(url);
			CFRelease(inStr);
			return 0;
		}
#endif
		//---------------------------------------------------------------------------
		/// Sandy Barbour

		/*
		 * http://www.dreamincode.net/code/snippet729.htm
		 */
		static char *BaseConversion(unsigned int number, int base) {
			static char buffer[50];
			char *ptr = buffer;

			//Checking for invalid base input
			if (base < 2 || base > 16)
				return NULL;

			//Going to the end of buffer
			ptr = &buffer[sizeof(buffer) - 1];
			*ptr = '\0';

			//Actual Conversion
			while (number != 0) {
				*--ptr = "0123456789abcdef"[number % base];
				number /= base;
			}
			return ptr;
		}

		static bool isBitSet(char * bitBuf, int bit) {
#if DEBUG
			std::cout << "input: " << bitBuf[bit] << std::endl;
#endif
			return bitBuf[bit] == '1';
		}

		static std::string getPrefKey(std::string line) {
			size_t pos = line.find("=");
			return line.substr(0, pos);
		}

		static std::string getPrefValue(std::string line) {
			size_t pos = line.find("=");
			return line.substr(pos + 1);
		}

	};
}
#endif /*UTILS_H_*/

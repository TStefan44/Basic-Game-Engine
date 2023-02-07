#pragma once

#include "../Utilities/Math.h"

#include <string>

class Globals
{
public:
	// Setter functions
	static int getScreenWidth();
	static int getScreenHeight();
	static std::string getWindowName();
	static Vector3 getColorBG();

	// Getters functions
	static void setScreenResolution(const int Width, const int Height);
	static void setWindowName(const std::string Name);
	static void setColorBG(const Vector3 color);

private:
	static int screenWidth;
	static int screenHeight;
	static std::string windowName;
	static Vector3 colorBG;
};
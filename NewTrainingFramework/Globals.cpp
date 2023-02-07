#pragma once

#include "stdafx.h"

#include "Globals.h"

int Globals::screenWidth = 960;
int Globals::screenHeight = 720;
std::string Globals::windowName = "Engine";
Vector3 Globals::colorBG = Vector3(0.f, 0.f, 0.f);

int Globals::getScreenWidth() {
	return screenWidth;
}

int Globals::getScreenHeight() {
	return screenHeight;
}

std::string Globals::getWindowName() {
	return windowName;
}

Vector3 Globals::getColorBG() {
	return colorBG;
}

void Globals::setScreenResolution(const int Width, const int Height) {
	screenHeight = Height;
	screenWidth = Width;
}

void Globals::setWindowName(const std::string Name) {
	windowName = Name;
}

void Globals::setColorBG(const Vector3 color) {
	colorBG = color;
}
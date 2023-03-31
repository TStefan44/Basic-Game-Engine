#pragma once
#include "stdafx.h"
#include "InputController.h"

using namespace manager;

InputController* InputController::instance = nullptr;

InputController::InputController() {}

/*
* Function used in singleton design pattern
*/
InputController* InputController::getInstace() {
	if (instance == nullptr) {
		instance = new InputController();
	}

	return instance;
}

/*
* Seek action in the intern map by the input key.
* If association exists, execute the action.
*/
void InputController::DoAction(char c) {
	Action action = inputController[c];
	if (action != nullptr)
		action();
}

/*
* Add new action in the inter map. We overwrite the old action
* if it exist.
*/
void InputController::AddAction(char c, Action newAction) {
	inputController[c] = newAction;
}

InputController::~InputController() {}
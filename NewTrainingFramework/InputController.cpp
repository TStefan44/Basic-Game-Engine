#pragma once
#include "stdafx.h"
#include "InputController.h"

using namespace manager;

InputController* InputController::instance = nullptr;

InputController::InputController() {}

InputController* InputController::getInstace() {
	if (instance == nullptr) {
		instance = new InputController();
	}

	return instance;
}

void InputController::DoAction(char c) {
	Action action = inputController[c];
	if (action != nullptr)
		action();
}

void InputController::AddAction(char c, Action newAction) {
	inputController[c] = newAction;
}

InputController::~InputController() {}
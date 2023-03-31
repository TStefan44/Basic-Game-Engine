#pragma once

#include <map>
#include <functional>

#include "../Utilities/utilities.h"

namespace manager {
	class InputController {
	private:
		InputController();

	public:
		// Define a void function with no arguments as
		// an action to execute when input is detected
		using Action = std::function<void()>;

		~InputController();

		static InputController* getInstace();

		void DoAction(char c);

		void AddAction(char c, Action newAction);

	private:
		static InputController* instance;

		std::map<char, Action> inputController;
	};
}
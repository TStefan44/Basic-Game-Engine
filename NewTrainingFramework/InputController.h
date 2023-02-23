#pragma once

#include <map>
#include <functional>

#include "../Utilities/utilities.h"

namespace manager {
	class InputController {

	private:
		InputController();

	public:
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
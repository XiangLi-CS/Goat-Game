#pragma once
#pragma once
#include <functional >
#include <string>

namespace NCL {
	namespace CSC8503 {
		typedef std::function <void(float)> StateUpdateFunction;

		class StateTwo {
		public:
			StateTwo() {}
			StateTwo(StateUpdateFunction someFunc, std::string n) {
				func = someFunc;
				name = n;
			}
			void Update(float dt) {
				if (func != nullptr) {
					func(dt);
				}
			}

			std::string GetName() { return name; }
		protected:
			std::string name;
			StateUpdateFunction func;
		};
	}
}
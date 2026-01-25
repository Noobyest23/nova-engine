#ifndef NOVA_SIGNAL
#define NOVA_SIGNAL

#include <functional>
template<typename... Args>
class Signal {
public:
	using Callback = std::function<void(Args...)>;

	int Connect(Callback cb) {
		cpp_listeners.push_back(cb);
		return cpp_listeners.size() - 1;
	}

	void Emit(Args... args) {
		for (auto& cb : cpp_listeners) {
			cb(args...);
		}
	}

	void Disconnect(int index) {
		cpp_listeners.erase(cpp_listeners.begin() + index);
	}

private:
	std::vector<Callback> cpp_listeners;
};

#endif
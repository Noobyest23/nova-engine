#ifndef NOVA_SIGNAL
#define NOVA_SIGNAL

#include <functional>
template<typename... Args>
class Signal {
public:
	using Callback = std::function<void(Args...)>;

	void Connect(Callback cb) {
		cpp_listeners.push_back(cb);
	}

	void Emit(Args... args) {
		for (auto& cb : cpp_listeners) {
			cb(args...);
		}
	}

private:
	std::vector<Callback> cpp_listeners;
};

#endif
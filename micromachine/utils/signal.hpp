#pragma once



namespace micromachine::system {

class signal {
private:
	bool _value;

public:
	signal(bool value)
		: _value(value) {}

	signal()
		: signal(false) {}

	void set() {
		_value = true;
	}

	void clear() {
		_value = false;
	}

	bool is_set() const {
		return _value;
	}

	operator bool () const {
		return is_set();
	}
};

}

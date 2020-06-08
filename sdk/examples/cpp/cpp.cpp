
#include <cstdio>
#include <vector>
#include <array>
#include <memory>

class Singleton {
public:
	static Singleton& get() {
		static Singleton instance;
		return instance;
	}
	int val() const {
		return _val;
	}
private:
	Singleton()
		: _val(42) {
		printf("Singleton::Singleton()\n");
	}
	~Singleton() {
		printf("Singleton::Singleton()\n");
	}
	int _val;
};

class Duck {
private:
	uint32_t _quack_count;

public:
	Duck()
		: _quack_count(0) {
		printf("Duck %08x : Hello, world.\n", this);
	}

	~Duck() {
		printf("Duck %08x : Goodbye, cruel world.\n", this);
	}

public:
	void quack() {
		_quack_count++;
		printf("Duck %08x : Quack %d!\n", this, _quack_count);
	}
};

extern "C" int main()  {
	printf("Singleton value = %i\n", Singleton::get().val());
	std::vector<uint64_t>* vector = new std::vector<uint64_t>(10);
	std::unique_ptr<std::array<uint32_t, 4>> ptr(new std::array<uint32_t, 4>());
	printf("vector address = %08x\n", vector);
	delete vector;
	printf("ptr address = %08x\n", ptr.get());
	std::unique_ptr<Duck> donald = std::unique_ptr<Duck>(new Duck());
	std::unique_ptr<Duck> daffy = std::unique_ptr<Duck>(new Duck());

	daffy->quack();
	donald->quack();
	daffy->quack();

}



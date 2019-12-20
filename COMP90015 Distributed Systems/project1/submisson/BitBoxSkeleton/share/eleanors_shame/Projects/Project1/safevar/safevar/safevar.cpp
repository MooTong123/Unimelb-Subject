#include <iostream>
#include <string>
class var {
	public:
		var(void): real(0), string("") {}
		var(const std::string str, const double rl): real(rl), string(str) {}
		var(const var& target): real(target.real), string(target.string) {}
		var& operator =(const var& target) { real = target.real; string = target.string; return *this; }
		var& operator =(const int target) { real = target; return *this; }
		var& operator =(const float target) { real = target; return *this; }
		var& operator =(const double target) { real = target; return *this; }
		var& operator =(const char *target) { string = target; return *this; }
		var& operator =(const std::string target) { string = target; return *this; }
		var& operator =(const char target) { string = target; return *this; }
		var& operator +=(const var& target) { real += target.real; string += target.string; return *this; }
		var& operator +=(const int target) { real += target; return *this; }
		var& operator +=(const float target) { real += target; return *this; }
		var& operator +=(const double target) { real += target; return *this; }
		var& operator +=(const char *target) { string += target; return *this; }
		var& operator +=(const std::string target) { string += target; return *this; }
		var& operator +=(const char target) { string += target; return *this; }
		var& operator -=(const var& target) { real -= target.real; return *this; }
		var& operator -=(const int target) { real -= target; return *this; }
		var& operator -=(const float target) { real -= target; return *this; }
		var& operator -=(const double target) { real -= target; return *this; }
		var& operator *=(const var& target) { real *= target.real; return *this; }
		var& operator *=(const int target) { real *= target; return *this; }
		var& operator *=(const float target) { real *= target; return *this; }
		var& operator *=(const double target) { real *= target; return *this; }
		const var operator +(const var& target)	{ return var(*this) += target; }
		const var operator +(const int target) { return var(*this) += target; }
		const var operator +(const float target) { return var(*this) += target; }
		const var operator +(const double target) { return var(*this) += target; }
		const var operator +(const char *target) { return var(*this) += target; }
		const var operator +(const std::string target) { return var(*this) += target; }
		const var operator +(const char target) { return var(*this) += target; }
		const var operator -(const var& target)	{ return var(*this) -= target; }
		const var operator -(const int target) { return var(*this) -= target; }
		const var operator -(const float target) { return var(*this) -= target; }
		const var operator -(const double target) { return var(*this) -= target; }
		const var operator *(const var& target)	{ return var(*this) *= target; }
		const var operator *(const int target) { return var(*this) *= target; }
		const var operator *(const float target) { return var(*this) *= target; }
		const var operator *(const double target) { return var(*this) *= target; }
		var& operator ++(void) { ++(*this).real; return *this; }
		var& operator ++(int unused) { ++*this; return *this; }
		var& operator --(void) { --(*this).real; return *this; }
		var& operator --(int unused) { --*this; return *this; }
		bool operator ==(const var& target) const { return real == target.real && string == target.string; }
		bool operator ==(const int target) const { return real == target; }
		bool operator ==(const float target) const { return real == target; }
		bool operator ==(const double target) const { return real == target; }
		bool operator ==(const char *target) const { return string == target; }
		bool operator ==(const std::string target) const { return string == target; }
		bool operator ==(const char target) const { return string == &target; }
		template <class T>
		bool operator !=(const T target) const { return !(*this == target); }
		double getv(void) { return real; }
		std::string gets(void) { return string; }
	private:
		double real;
		std::string string;
};
int main() {
	var myVar;
	myVar = "Hello World!";
	++myVar;
	std::cout << myVar.gets() << myVar.getv() << std::endl;
	std::cin.get();
}
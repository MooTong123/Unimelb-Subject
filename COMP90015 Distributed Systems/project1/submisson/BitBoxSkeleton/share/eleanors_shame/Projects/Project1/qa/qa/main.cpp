#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

template <class T>
T choose(T a, T b, T c, T d);
double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mult(double a, double b) { return a * b; }
double divd(double a, double b) { return a / b; }
double (__cdecl* const oparray[6]) (double, double) = { &mult, &add, 0, &sub, 0, &divd };
double opr(double a, double b, unsigned char op);
inline double roundto(double x, int places) { return floor(x * pow((double)10, (double)places)) / pow((double)10, (double)places); }

int main() {
	srand(static_cast<unsigned>(time(0)));
	int marks = 0, rep = 10;
	std::cout << "Welcome to the Maths Challenge." << std::endl << "Each correct answer is 10 marks, and each incorrect answer is -5 marks."
			  << std::endl;
	while(rep--) {
		char code = choose<char>('+', '-', '*', '/');
		int a = (rand() % 200) - 100, b = (rand() % 200) - 100;
		double u_ans = 0, c_ans = 0;
		std::cout << "\nQuestion " << (10 - rep) << "." << std::endl;
		std::cout << "Your question is: " << std::endl << a << " " << code << " " << b << std::endl
				  << ((code == '/')?("Round to 4 decimal places.\n"):(""));
		std::cin >> u_ans;
		while(!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Invalid input." << std::endl;
			std::cin >> u_ans;
		}
		u_ans = roundto(u_ans, 4);
		c_ans = roundto(opr(a, b, code), 4);
		std::cout << "You entered " << u_ans << ((code == '/')?(" (rounded to 4 places)"):("")) << "." << std::endl
				  << "The correct answer is " << c_ans << "." << std::endl;
		if(u_ans == c_ans) {
			std::cout << "You entered the correct answer! +10 marks!";
			marks += 10;
		}
		else {
			std::cout << "You entered the incorrect answer... -5 marks.";
			marks -= 5;
		}
		std::cout << std::endl;
	}
	std::cout << "Your overall score was " << marks << "." << std::endl;
	std::cin.ignore();
	std::cin.get();
	return 0;
}
double opr(double a, double b, unsigned char op) {
	op -= '*';
	if(op > 6) { 
		throw "Must specify a valid opcode.";
	}
	if(oparray[op]) {
		return oparray[op](a, b);
	}
	else {
		throw "Must specify a valid opcode.";
	}
}
template <class T>
T choose(T a, T b, T c, T d) {
	T ar[4] = { a, b, c, d };
	char choice = rand() % 4;
	return ar[choice];
}
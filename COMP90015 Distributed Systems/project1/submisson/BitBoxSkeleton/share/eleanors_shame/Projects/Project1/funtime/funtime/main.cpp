#include <cstdlib>
#include <iostream>
using namespace std;

class MATHS
{
	public:
		int add(int a, int b) { return a + b; }
		int multiply(int a, int b) { return a * b; }
		int subtract(int a, int b) { return a - b; }
		float divide(float a, float b) { if(b == 0) { return -1; } return a / b; }
} maths;

int main(int argc, char* argv[])
{
	int a, b;
	cout << "Enter first number." << endl;
	cin >> a;
	cout << "Enter second number." << endl;
	cin >> b;
	cout <<"a+b: " << maths.add(a, b) << "\na-b: " << maths.subtract(a, b)
		<< "\na*b: " << maths.multiply(a, b) << "\na/b: " << maths.divide((float)a, (float)b)
		<< endl;
	system("PAUSE");
	return 0;
}
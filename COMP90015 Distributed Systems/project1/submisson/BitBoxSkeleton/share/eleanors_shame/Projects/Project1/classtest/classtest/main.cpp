#include <iostream>
#include <cstdlib>
using namespace std;

class Person {
	private:
		char		*Name;
		int			Age;
	public:
		inline char* GetName(void) { return this -> Name; }
		inline int GetAge(void) { return this -> Age; }
		inline void SetName(char *NewName) { this -> Name = NewName; }
		inline void SetAge(int NewAge) { this -> Age = NewAge; }
		inline virtual void SayStory(void) {
			cout << "I am a person." << endl;
		}
} *People[4];

class Criminal : public Person {
	public:
		inline void SayStory(void) {
			cout << "I am a criminal." << endl;
		}
};

class Goodie : public Person {
	public:
		inline void SayStory(void) {
			cout << "I am a goodie." << endl;
		}
};

int main(void) {
	People[0] = new Criminal;
	People[1] = new Criminal;
	People[2] = new Goodie;
	People[3] = new Person;

	People[0] -> SayStory();
	People[1] -> SayStory();
	People[2] -> SayStory();
	People[3] -> SayStory();

	for(int i = 0; i < 4; i++) {
		delete People[i];
	}
	system("PAUSE");
	return 0;
}
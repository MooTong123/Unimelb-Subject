#include "tgrid.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>
#include <stack>

/*int function ()
{
	return rand() % 100;
}*/
int main ()
{
	srand(clock());
	TGrid<int> grid(10, 10);

	grid.setall([]()->int {return rand() % 100; });

	std::setfill(' ');
	
	std::string read;
	std::stack<char> commands;
	Pos old;
	bool run = true;
	while (run) {
		old = grid.pos();
		grid.move(0, 0);
		for (unsigned int i = 0; i < grid.size(); ++i) {
			if (grid.pos() == old) {
				std::cout << std::setw(2)
						  << "->";
				std::cout << std::setw(2);
			}
			else {
				std::cout << std::setw(4);
			}
			std::cout << grid.at();
			grid.forward();
			if (grid.nextln()) {
				std::cout << "\n";
			}
		}
		grid.move(old);
		std::cout << "At " << grid.at() << "\n"
				  << "At position (" << grid.pos().x << ", "
				  << grid.pos().y << ")\n";
		std::cout << "Enter a command: ";
		std::getline(std::cin, read);
		for (int i = read.length() - 1; i >= 0; --i) {
			commands.push(read.at(i));
		}
		while (!commands.empty()) {
			char input = commands.top();
			commands.pop();
			switch (input) {
				case 'a':
					grid.left();
					break;

				case 'd':
					grid.right();
					break;

				case 'w':
					grid.up();
					break;

				case 's':
					grid.down();
					break;

				case 'q':
					run = false;
					break;

				case 'm':
					if (commands.top() == 'r') {
						int x = rand() % 10;
						int y = rand() % 10;
						grid.move(x, y);
						commands.pop();
					}
					break;

				case 'r':
					grid.setall([]()->int {return rand() % 100; });
					break;
			}
		}
	}
	return 0;
}
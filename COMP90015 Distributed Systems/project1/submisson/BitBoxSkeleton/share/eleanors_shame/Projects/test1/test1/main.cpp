/*struct State {
	double x;
	double v;
};
struct Derivative {
	double dx;
	double dv;
};

double acceleration (const State& state, double t)
{
	const double k = 10;
	const double b = 1;
	return -k * state.x - b * state.v;
}
Derivative evaluate (const State& initial, double t, double dt, const Derivative& d)
{
	State state;
	state.x = initial.x + d.dx * dt;
	state.v = initial.v + d.dv * dt;

	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t + dt);

	return output;
}*/// Simple RK4 integration framework
// Copyright (c) 2004, Glenn Fiedler
// http://www.gaffer.org/articles

#include <iostream>
#include <cmath>
#include <windows.h>

void clear_screen ( void )
{
  DWORD n;                         /* Number of characters written */
  DWORD size;                      /* number of visible characters */
  COORD coord = {0};               /* Top left screen position */
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  /* Get a handle to the console */
  HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );

  GetConsoleScreenBufferInfo ( h, &csbi );

  /* Find the number of characters to overwrite */
  size = csbi.dwSize.X * csbi.dwSize.Y;

  /* Overwrite the screen buffer with whitespace */
  FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
  GetConsoleScreenBufferInfo ( h, &csbi );
  FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );

  /* Reset the cursor to the top left position */
  SetConsoleCursorPosition ( h, coord );
}

struct State
{
	float x;
	float v;
};

struct Derivative
{
	float dx;
	float dv;
};

float acceleration(const State &state, float t)
{
	return -5;
}

Derivative evaluate(const State &initial, float t)
{
	Derivative output;
	output.dx = initial.v;
	output.dv = acceleration(initial, t);
	return output;
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;
	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t+dt);
	return output;
}

void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);

	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const float dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);
	
	state.x = state.x + dxdt*dt;
	state.v = state.v + dvdt*dt;
}

int main() 
{
	State state;
	const float start = 80;
	state.x = start;
	state.v = 0;
	
	float t = 0;
	float dt = 0.001f;

	const char square = '#';
	
	while (t < 10)
	{
		for (int i = 0; i < abs(state.x); ++i) {
			if (abs(floor(state.x)) != i) {
				std::cout << " ";
			}
			else {
				std::cout << square;
			}
		}
		std::cout << "\n";
		integrate(state, t, dt);
		t += dt;
	}

	std::cin.get();
	
    return 0;
}

#include <iostream>

inline int max(int a, int b) { return (a>b)?a:b; }
int main ()
{
	int i, bools[300], a, b, c, d, e, f, g, answers[300][6], j = 0;
	a = 1;
	b = 1;
	c = 1;
	d = 1;
	e = 1;
	f = 1;
	bools[max(0, 0)] = 1;
	for (a = 1; a < 12; ++a) {
	for (b = 1; b < 12; ++b) {
	for (c = 1; c < 12; ++c) {
	for (d = 1; d < 12; ++d) {
	for (e = 1; e < 12; ++e) {
	for (f = 1; f < 12; ++f) {
		for (i = 1; i < 30; ++i) {
			bools[max(i, 0)] = 0;
		}
		bools[max(a, 0)] = 1;
		bools[max(b, 0)] = 1;
		bools[max(c, 0)] = 1;
		bools[max(d, 0)] = 1;
		bools[max(e, 0)] = 1;
		bools[f] = 1;

		bools[max(a + a, 0)] = 1;

		bools[max(b + a, 0)] = 1;
		bools[max(b + b, 0)] = 1;
		bools[max(b - a, 0)] = 1;

		bools[max(c + a, 0)] = 1;
		bools[max(c + b, 0)] = 1;
		bools[max(c + c, 0)] = 1;
		bools[max(c - a, 0)] = 1;
		bools[max(c - b, 0)] = 1;

		bools[max(d + a, 0)] = 1;
		bools[max(d + b, 0)] = 1;
		bools[max(d + c, 0)] = 1;
		bools[max(d + d, 0)] = 1;
		bools[max(d - a, 0)] = 1;
		bools[max(d - b, 0)] = 1;
		bools[max(d - c, 0)] = 1;

		bools[max(e + a, 0)] = 1;
		bools[max(e + b, 0)] = 1;
		bools[max(e + c, 0)] = 1;
		bools[e + d] = 1;
		bools[e + e] = 1;
		bools[max(e - a, 0)] = 1;
		bools[max(e - b, 0)] = 1;
		bools[max(e - c, 0)] = 1;
		bools[max(e - d, 0)] = 1;

		bools[max(f + a, 0)] = 1;
		bools[max(f + b, 0)] = 1;
		bools[max(f + c, 0)] = 1;
		bools[f + d] = 1;
		bools[f + e] = 1;
		bools[f + f] = 1;
		bools[max(f - a, 0)] = 1;
		bools[max(f - b, 0)] = 1;
		bools[max(f - c, 0)] = 1;
		bools[max(f - d, 0)] = 1;
		bools[max(f - e, 0)] = 1;

		g = 1;
		for (i = 1; i < 30; ++i) {
			if ((g = bools[max(i, 0)]) == 0) {
				break;
			}
		}
		
		if (g == 1) {
			answers[j][0] = a;
			answers[j][1] = b;
			answers[j][2] = c;
			answers[j][3] = d;
			answers[j][4] = e;
			answers[j][5] = f;
			++j;
		}
		std::cout << a << "-" << b << "-" << c << "-" << d << "-" << e << "-" << f << "\n";
	}}}}}}
	std::cout << "--ANSWERS--\n(" << j << " answers total)\n";
	for (i = 0; i < j; ++i) {
		std::cout << answers[i][0] << "-" << answers[i][1] << "-" << answers[i][2] << "-" << answers[i][3] << "-" << answers[i][4] << "-" << answers[i][5] << "\n";
	}
	std::cin.get();
	return 0;
}
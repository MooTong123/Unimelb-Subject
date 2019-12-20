#ifndef __TGRID
#define __TGRID

#include <vector>
#include <exception>

#ifndef _TGRID_NO_DEF
inline
int sign (int a) { if (a > 0) return 1; if (a < 0) return -1; return 0; }
struct Pos
{
	int x;
	int y;
	bool operator == (Pos p) { return x == p.x && y == p.y; }
	Pos (int _x, int _y): x(_x), y(_y) {}
	Pos (): x(0), y(0) {}
};
#endif

template <class _Type>
class TGrid
{
	public:
		typedef _Type (*F) ();
		TGrid (unsigned int width, unsigned int height):
		  _width(width), _height(height), _itx(0), _ity(0), _nextln(false)
		{
			_ver.resize(height);
			for (unsigned int i = 0; i < height; ++i) {
				_ver[i].resize(width, _Type(0));
			}
		}

		// directional iterating
		TGrid& up (int dist = 1) {
			_ity -= dist;
			if (_ity < 0) {
				_ity = 0;
			}
			if (_ity >= _height) {
				_ity = _height - 1;
			}
			return *this;
		}
		TGrid& down (int dist = 1) {
			return up(-dist);
		}
		TGrid& right (int dist = 1) {
			_itx += dist;
			if (_itx < 0) {
				_itx = 0;
			}
			if (_itx >= _width) {
				_itx = _width - 1;
			}
			return *this;
		}
		TGrid& left (int dist = 1) {
			return right(-dist);
		}

		// direct iterating
		TGrid& forward (int dist = 1);
		TGrid& backward(int dist = 1);

		unsigned int size () const {
			return (_width) * (_height);
		}

		void set (const _Type& val) {
			_ver.at(_ity)[_itx] = val;
		}
		// set a whole row (value)
		void setrow (int row, const _Type& val) {
			for (int i = 0; i < _width; ++i) {
				_ver.at(row)[i] = val;
			}
		}
		// function or function-like object (including funcs)
		template <class _Func>
		void setrow (int row, _Func func)
		{
			for (int i = 0; i < _width; ++i) {
				_ver[row][i] = func();
			}
		}
		// set a whole column (value)
		void setcol (int col, const _Type& val) {
			for (int i = 0; i < _height; ++i) {
				_ver[i][col] = val;
			}
		}
		template <class _Func>
		void setcol (int col, _Func func)
		{
			for (int i = 0; i < _height; ++i) {
				_ver[i][col] = func();
			}
		}
		// set entire grid (value)
		void setall (const _Type& val)
		{
			for (int i = 0; i < _width; ++i) {
				setcol(i, val);
			}
		}	
		template <class _Func>
		void setall (_Func func)
		{
			for (int i = 0; i < _width; ++i) {
				setcol(i, func);
			}
		}

		void move (int x, int y) {
			if (!(x < 0 || x > _width)) {
				_itx = x;
			}
			if (!(y < 0 || y > _height)) {
				_ity = y;
			}
		}
		void move (Pos pos) {
			if (!(pos.x < 0 || pos.x > _width)) {
				_itx = pos.x;
			}
			if (!(pos.y < 0 || pos.y > _height)) {
				_ity = pos.y;
			}
		}

		Pos pos () const { return Pos(_itx, _ity); }
		_Type at () const {
			return _ver.at(_ity).at(_itx);
		}

		bool nextln () const { return _nextln; }

	private:
		std::vector<std::vector<_Type>>				_ver;
		int											_itx;
		int											_ity;

		bool _nextln;

		const int _width;
		const int _height;
};

template <class _Type>
TGrid<_Type>& TGrid<_Type>::forward (int dist)
{
	_nextln = false;

	_itx += dist;
	while (_itx >= _width) {
		_itx -= _width;
		_ity++;
		_nextln = true;
	}
	return *this;
}
template <class _Type>
TGrid<_Type>& TGrid<_Type>::backward (int dist)
{
	_nextln = false;

	_itx -= dist;
	while (_itx < 0) {
		_itx += _width;
		_ity--;
		_nextln = true;
	}
	return *this;
}

#endif
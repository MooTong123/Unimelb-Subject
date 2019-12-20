#include <iostream>
using std::cout;

template <class _Type>
class Pool
{
	public:
		Pool (): _blockSize(1000), _sz(sizeof(_Type)), _blocks(0) { _first = new _Type[_blockSize]; }
		void alloc ()
		{
			if (_blocks * _sz > _blockSize) {
				_first = new _Type[_blockSize];
			}
			pointer = _first + _blocks * _sz;
			_blocks++;
		}

	private:
		const int _blockSize;
		const int _sz;

		unsigned int _blocks;
		_Type* _first;
};

int main ()
{
	Pool<int> intPool;
	int* p;
	intPool.alloc(
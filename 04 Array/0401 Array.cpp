#include <iostream>
#include <array> // array 사용

using namespace std;

int main() {
	/* array?
	C++11에서 새로 생긴 컨테이너.
	고정 길이 배열 자료구조를 사용.
	생성할 컨테이너의 크기를 지정해야함.
	*/

	cout << "array 생성 : std::array" << endl;
	{
		array<int, 2> arr = { 9 };
		cout << arr[0] << endl; // 9
		cout << arr[1] << endl; // 0
	}

	cout << "반복자 사용" << endl;
	{
		array<int, 3> arr = { 9, 1 };
		for (array<int, 3U>::iterator iter = arr.begin(); iter < arr.end(); iter++)
		{
			cout << *iter << endl; // 9, 1, 0
		}

		int a = 3u;
	}

	return 0;
}
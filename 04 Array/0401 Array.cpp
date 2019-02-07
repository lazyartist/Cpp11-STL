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

	cout << "반복자 사용 : std::array<...>::iterator" << endl;
	{
		array<int, 3> arr = { 9, 1 };
		for (array<int, 3U>::iterator iter = arr.begin(); iter < arr.end(); iter++)
		{
			cout << *iter << endl; // 9, 1, 0
		}
	}

	cout << "첫 번째 요소 : front()" << endl;
	cout << "지정한 인덱스에 접근 : at()" << endl;
	cout << "마지막 요소 : back()" << endl;
	cout << "배열 크기 : size()" << endl;
	cout << "배열 데이터에 직접 접근 : data()" << endl;
	{
		array<int, 3> arr = { 1, 2, 3 };
		cout << arr.front() << endl; // 1
		cout << arr.at(1) << endl; // 2
		cout << arr.back() << endl; // 3

		int* pArr = arr.data();
		for (size_t i = 0; i < arr.size(); i++)
		{
			cout << pArr[i] << endl; // 1, 2, 3
		}
	}

	cout << "특정 값으로 채우기 : fill()" << endl;
	{
		array<int, 3> arr = { 1, 2, 3 };
		arr.fill(9);
		for (array<int, 3U>::iterator iter = arr.begin(); iter < arr.end(); iter++)
		{
			cout << *iter << endl; // 9, 9, 9
		}
	}

	return 0;
}
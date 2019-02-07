#include <iostream>
#include <array> // array ���

using namespace std;

int main() {
	/* array?
	C++11���� ���� ���� �����̳�.
	���� ���� �迭 �ڷᱸ���� ���.
	������ �����̳��� ũ�⸦ �����ؾ���.
	*/

	cout << "array ���� : std::array" << endl;
	{
		array<int, 2> arr = { 9 };
		cout << arr[0] << endl; // 9
		cout << arr[1] << endl; // 0
	}

	cout << "�ݺ��� ��� : std::array<...>::iterator" << endl;
	{
		array<int, 3> arr = { 9, 1 };
		for (array<int, 3U>::iterator iter = arr.begin(); iter < arr.end(); iter++)
		{
			cout << *iter << endl; // 9, 1, 0
		}
	}

	cout << "ù ��° ��� : front()" << endl;
	cout << "������ �ε����� ���� : at()" << endl;
	cout << "������ ��� : back()" << endl;
	cout << "�迭 ũ�� : size()" << endl;
	cout << "�迭 �����Ϳ� ���� ���� : data()" << endl;
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

	cout << "Ư�� ������ ä��� : fill()" << endl;
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
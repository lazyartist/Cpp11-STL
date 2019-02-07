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

	cout << "�ݺ��� ���" << endl;
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
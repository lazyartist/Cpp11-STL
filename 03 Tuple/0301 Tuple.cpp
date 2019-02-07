#include <tuple> // tuple�� ����ϱ� ���� �ʿ�
#include <iostream>

using namespace std;

int main() {
	/* tuple?
	Ŭ����, ����ü�� ������ �ʰ� �����͸� �ϳ��� ��ü�� ������.
	Cpp11 �������� std::pair�� ���.
	*/

	// make tuple
	tuple<int, double, char> tp = tuple<int, double, char>(1, 2.2, 'c');

	// ����Ÿ��, ������ü, ���� �ٸ��� ������� ��� ����.
	//tuple<int, double, char> tp = tuple<int, double, int>(1.1, 2.1, 1); // ���	C4244	'�ʱ�ȭ ��': '_Ty'���� '_Ty'(��)�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� �ֽ��ϴ�.
	
	// use tuple
	cout << get<0>(tp) << endl; // 1
	cout << get<1>(tp) << endl; // 2.2
	cout << get<2>(tp) << endl; // c

	return 0;
}
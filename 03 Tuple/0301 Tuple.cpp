#include <tuple> // tuple�� ����ϱ� ���� �ʿ�
#include <iostream>

using namespace std;

int main() {
	/* tuple?
	Ŭ����, ����ü�� ������ �ʰ� �����͸� �ϳ��� ��ü�� ������.
	Cpp11 �������� std::pair�� ���.
	tuple�� ������ �� �ִ� ������ ������ Visual Studio 2012 ���� �������� _VARIADIC_MAX�� ���ǵ� 10�������̴�.
	*/

	cout << "tuple ����" << endl;
	{
		// std::tuple : tuple ����
		tuple<int, char> tp1 = tuple<int, char>(1, 'c'); // ���� �� �ʱ�ȭ ����

		// ����Ÿ��, ������ü, ���� �ٸ��� ������� ��� ����.
		//tuple<int, char> tp = tuple<int, double, int>(1.1, 1); // ���	C4244	'�ʱ�ȭ ��': '_Ty'���� '_Ty'(��)�� ��ȯ�ϸ鼭 �����Ͱ� �սǵ� �� �ֽ��ϴ�.

		// std::get : tuple �� ��������
		cout << get<0>(tp1) << endl; // 1
		cout << get<1>(tp1) << endl; // 2.2

		// std::get : tuple �� �����ϱ�
		get<0>(tp1) = 9;
		cout << get<0>(tp1) << endl; // 9
	}

	cout << "tuple �����ϰ� �����ϱ� : std::make_tuple" << endl;
	{
		tuple<int, char> tp = make_tuple(10, 't');
		cout << get<0>(tp) << endl; // 10
		cout << get<1>(tp) << endl; // t
	}

	cout << "tuple ������ ���� : std::tuple_size" << endl;
	{
		tuple<int, char> tp = make_tuple(10, 't');
		cout << tuple_size<decltype(tp)>::value << endl; // 2
	}

	cout << "���� �����͸� �����ϴ� tuple : std::tie" << endl;
	{
		int intValue = 20;
		tuple<int&> rtp = tie(intValue); // tie ��� tuple<int&>(intValue); ����
		get<0>(rtp) = 29;
		cout << get<0>(rtp) << endl;
		cout << intValue << endl;
	}

	cout << "tuple�� �����͸� �ϰ������� ������ ���� : std::tie" << endl;
	{
		tuple<int, double> tp = make_tuple(1, 2.2);
		int value1;
		double value2;
		tie(value1, value2) = tp;

		cout << value1 << endl; // 1
		cout << value2 << endl; // 2.2
	}

	cout << "tuple�� �����͸� �κ������� ������ ���� : std::tie, std::ignore" << endl;
	{
		tuple<int, double, char> tp = make_tuple(1, 1.1, 'c');
		int value1;
		char value3;
		tie(value1, ignore, value3) = tp;

		cout << value1 << endl; // 1
		cout << value3 << endl; // c
	}


	cout << "tuple ��ġ�� : std::tuple_cat" << endl;
	{
		tuple<int> tp1 = make_tuple(1);
		tuple<double> tp2 = make_tuple(2.2);
		tuple<int, double> tp_cat = tuple_cat(tp1, tp2); // cat : Concatenate (�罽���� �մ�; �����Ű��; <��� ����> ���[����]��Ű��, ������Ű��)

		cout << tuple_size<decltype(tp1)>::value << endl; // 1
		cout << tuple_size<decltype(tp2)>::value << endl; // 1
		cout << tuple_size<decltype(tp_cat)>::value << endl; // 2

		cout << get<0>(tp_cat) << endl; // 1
		cout << get<1>(tp_cat) << endl; // 2.2
	}

	return 0;
}
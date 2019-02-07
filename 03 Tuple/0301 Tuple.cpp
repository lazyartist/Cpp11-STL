#include <tuple> // tuple 사용
#include <iostream>

using namespace std;

/* tuple?
클래스, 구조체를 만들지 않고 데이터를 하나의 객체로 묶어줌.
C++98에서 제공하던 std::pair의 확장 버전.
컴파일 시간에 명시되거나 추론될 수 있는 데이터형 요소들을 가지는 이종 목록(heterogeneous list).
tuple에 저장할 수 있는 데이터 개수는 Visual Studio 2012 이하 버전에서 _VARIADIC_MAX로 정의된 10개까지이다.
*/
int main() {
	cout << "tuple 정의" << endl;
	{
		// std::tuple : tuple 생성
		tuple<int, char> tp1 = tuple<int, char>(1, 'c'); // 생성 후 초기화 가능

		// 변수타입, 실제객체, 값이 다르면 에러대신 경고가 난다.
		//tuple<int, char> tp = tuple<int, double, int>(1.1, 1); // 경고	C4244	'초기화 중': '_Ty'에서 '_Ty'(으)로 변환하면서 데이터가 손실될 수 있습니다.

		// std::get : tuple 값 가져오기
		cout << get<0>(tp1) << endl; // 1
		cout << get<1>(tp1) << endl; // 2.2

		// std::get : tuple 값 저장하기
		get<0>(tp1) = 9;
		cout << get<0>(tp1) << endl; // 9
	}

	cout << "tuple 간단하게 정의하기 : std::make_tuple" << endl;
	{
		tuple<int, char> tp = make_tuple(10, 't');
		cout << get<0>(tp) << endl; // 10
		cout << get<1>(tp) << endl; // t
	}

	cout << "tuple 데이터 개수 : std::tuple_size" << endl;
	{
		tuple<int, char> tp = make_tuple(10, 't');
		cout << tuple_size<decltype(tp)>::value << endl; // 2
	}

	cout << "참조 데이터를 저장하는 tuple : std::tie" << endl;
	{
		int intValue = 20;
		tuple<int&> rtp = tie(intValue); // tie 대신 tuple<int&>(intValue); 가능
		get<0>(rtp) = 29;
		cout << get<0>(rtp) << endl;
		cout << intValue << endl;
	}

	cout << "tuple의 데이터를 일괄적으로 변수에 저장 : std::tie" << endl;
	{
		tuple<int, double> tp = make_tuple(1, 2.2);
		int value1;
		double value2;
		tie(value1, value2) = tp;

		cout << value1 << endl; // 1
		cout << value2 << endl; // 2.2
	}

	cout << "tuple의 데이터를 부분적으로 변수에 저장 : std::tie, std::ignore" << endl;
	{
		tuple<int, double, char> tp = make_tuple(1, 1.1, 'c');
		int value1;
		char value3;
		tie(value1, ignore, value3) = tp;

		cout << value1 << endl; // 1
		cout << value3 << endl; // c
	}

	cout << "tuple 합치기 : std::tuple_cat" << endl;
	{
		tuple<int> tp1 = make_tuple(1);
		tuple<double> tp2 = make_tuple(2.2);
		tuple<int, double> tp_cat = tuple_cat(tp1, tp2); // cat : Concatenate ([kɑnkǽtənèit] 사슬같이 잇다; 연쇄시키다; <사건 등을> 결부[연결]시키다, 연관시키다)

		cout << tuple_size<decltype(tp1)>::value << endl; // 1
		cout << tuple_size<decltype(tp2)>::value << endl; // 1
		cout << tuple_size<decltype(tp_cat)>::value << endl; // 2

		cout << get<0>(tp_cat) << endl; // 1
		cout << get<1>(tp_cat) << endl; // 2.2
	}

	return 0;
}
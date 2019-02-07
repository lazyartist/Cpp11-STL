#include <tuple> // tuple을 사용하기 위해 필요
#include <iostream>

using namespace std;

int main() {
	/* tuple?
	클래스, 구조체를 만들지 않고 데이터를 하나의 객체로 묶어줌.
	Cpp11 이전에는 std::pair를 사용.
	*/

	// make tuple
	tuple<int, double, char> tp = tuple<int, double, char>(1, 2.2, 'c');

	// 변수타입, 실제객체, 값이 다르면 에러대신 경고가 난다.
	//tuple<int, double, char> tp = tuple<int, double, int>(1.1, 2.1, 1); // 경고	C4244	'초기화 중': '_Ty'에서 '_Ty'(으)로 변환하면서 데이터가 손실될 수 있습니다.
	
	// use tuple
	cout << get<0>(tp) << endl; // 1
	cout << get<1>(tp) << endl; // 2.2
	cout << get<2>(tp) << endl; // c

	return 0;
}
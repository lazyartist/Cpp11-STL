#include <iostream>
#include <vector>
#include <algorithm> // find_if

using namespace std;

int main() {
	cout << "컴파일 타임 때 타입을 정하는 auto" << endl;
	{
		/*
		auto는 변수 정의 시 명시적으로 타입을 정하지 않아도 초기화 값에 따라 컴파일 타임에 자동으로 타입을 정해준다.
		auto는 지역 변수에만 사용 가능
		포인터, 참조 타입도 사용 가능
		Iterator 사용 시 편리하다
		*/
		auto i = 0;
		auto* ii = &i;
		auto& iii = i;
		i = 1;

		cout << i << endl; // 1
		cout << (*ii) << endl; // 1
		cout << iii << endl; // 1

		vector<vector<int>> v;
		vector<vector<int>>::iterator iter = v.begin();
		auto aiter = v.begin(); // 이렇게 간단해진다.
	}

	cout << "이름없는 함수 람다" << endl;
	{
		// []: lamda capture, (): parameter, {}: body
		auto f1 = []() {}; // todo auto 안쓰고 람다 저장 가능?
		f1(); // function call

		// 반환값 명시적 지정
		auto f2 = []() -> int {return 0; };
		f2();

		// find_if 알고리즘에 람다 사용하기
		vector<int> v = { 1, 2, 3 };
		vector<int>::iterator iter = find_if(v.begin(), v.end(), [](int& i) {
			return i == 2;
		});
		cout << *iter << endl; // 2

		// 외부 변수 캡처
		int param = 0;
		auto f3 =
			[
				&iter/*iter 참조*/,
				&param/*param 참조*/
				/*&: 모든변수 참조, =: 모든변수 복사(&와 함께 쓸수없다)*/
			]() {
			cout << *(++iter) << endl; // 3
			cout << param << endl; // 0
		};
		f3();
	}

	cout << "range base for" << endl;
	{
		vector<int> v = { 1, 2, 3 };

		// 일반 for
		for (size_t i = 0; i < v.size(); i++) {}

		// VC++ 특화 for
		//for each (int var in v){} // 컴파일 안됨

		// range base for
		// for(for-range-declaration : expression) statement
		for (auto& const i : v) {} // 임시변수를 만드는 비용을 줄이고 값을 변경하지 못하게 하려면 &와 const를 사용한다.
	}

	cout << "unscoped enumeration, scoped enumeration" << endl;
	{
		// unscoped
		enum Unscoped : short { Unscoped1 = 1 }; // short는 enum의 크기를 지정한다. 안하면 int
		Unscoped1;

		// scoped
		enum class Scoped { Scoped1 = 2 }; // class 대신 struct 가능
		//Scoped1; // 컴파일 에러
		Scoped::Scoped1;

		cout << Unscoped1 << endl; // 1, 암묵적으로 정수로 변환
		cout << static_cast<int>(Scoped::Scoped1) << endl; // 2, 명시적으로 정수로 변환
	}

	cout << "nullptr" << endl;
	{
		/*
		NULL 매크로 상수, 0을 인자로 넘기면 int 타입으로 추론하는 문제가 있다.
		*/
		void f(int i);
		void f(double* i);

		f(static_cast<double*>(0)); // void f(double* i); 호출
		f(0); // void f(int i); 호출
		f(NULL); // void f(int i); 호출

		f(nullptr); // void f(double* i); 호출

		cout << sizeof(nullptr) << endl; // 4
		cout << typeid(nullptr).name() << endl; // std::nullptr_t
		//throw(nullptr); // 예외 발생

		// 다음은 모두 에러
		/*
		int n = nullptr;
		int n = 0;
		if (n == nullptr) {}
		if (nullptr) {}
		if (nullptr == 0) {}
		nullptr = 0;
		nullptr + 2;
		*/
	}

	return 0;
}

void f(int i) { cout << i << endl; }
void f(double* i) { cout << i << endl; }
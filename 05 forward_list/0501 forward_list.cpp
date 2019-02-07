#include <iostream>
#include <forward_list> // forward_list 사용

using namespace std;

/* forward_list?
forward_list를 만든 이유
	기존에 있던 std::list는 양방향 링크드 리스트이다
	단방향만 사용할 때는 메모리 사용량과 속도가 비효율적이다.
	따라서 C++11에서 단방향 리스트인 forward_list가 추가됨

forward_list의 설계 방침
	성능을 최우선한다.
	std::list의 insert, erase를 구현 복잡성 증가와 성능 하락을 막기위해 제공하지 않는다.
	불필요한 메모리 사용을 막기 위해 size() 함수를 제공하지 않는다.

std::list와 다른 점
	forward_list는 단방향이므로 포인터를 하나만 갖는다.
	따라서 각 요소마다 포인터 하나 사이즈인 4byte(32bit) or 8byte(64bit)씩 아낄 수 있다.
	forward_list는 한 방향으로만 이동한다.
*/

int main() {
	cout << "std::forward_list 생성" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };

		for (auto v : fl)
		{
			cout << v << endl; // 1, 2, 3
		}
	}

	cout << "리스트 앞에 데이터 추가 : push_front()" << endl;
	{
		forward_list<int> fl;
		fl.push_front(0);
		fl.push_front(1);

		for (auto v : fl)
		{
			cout << v << endl; // 1, 0
		}
	}

	cout << "특정 위치 뒤에 데이터 추가 : insert_after()" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.insert_after(fl.begin(), 9);

		for (auto v : fl)
		{
			cout << v << endl; // 1, 9, 2, 3
		}
	}

	cout << "임시객체를 생성/복사하지 않고 특정 위치 뒤에 데이터 추가 : emplace_after(), emplace_front()" << endl;
	{
		class A {
		public:
			A(int v) { this->v = v; }
			int v;
		};

		forward_list<A> fl = { 1, 2, 3 };
		fl.insert_after(fl.begin(), A(9)); // A객체를 '생성'하고 인수로 넘기며 '복사'된다.

		// C++11에서 생긴 emplace_로 시작하는 함수를 "Placement Insert"라 한다.
		// 추가할 객체를 생성해서 넘기지 않고 생성자의 인수값을 넘겨주기 때문에 인수전달 시 발생하는 복사 과정이 없다.
		fl.emplace_after(fl.begin(), 99); // 인수값으로 forward_list 내부에서 A객체를 바로 '생성'한다. '복사' 과정이 없다.
		fl.emplace_front(999); // emplace_after()와 같은 방식이지만 가장 앞에 생성한다.

		for (auto v : fl)
		{
			cout << v.v << endl; // 999, 1, 99, 9, 2, 3
		}
	}

	cout << "첫 번째 데이터 삭제 : pop_front()" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.pop_front();
		for (auto v : fl)
		{
			cout << v << endl; // 2, 3
		}
	}

	cout << "특정 위치 다음 데이터 삭제 : erase_after()" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.erase_after(fl.begin());
		for (auto v : fl)
		{
			cout << v << endl; // 1, 3
		}
	}

	cout << "범위 지정하여 데이터 삭제 : erase_after(범위지정)" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.erase_after(fl.begin(), fl.end()); // 첫 파람 뒤부터 ~ 두 번째 파람까지
		for (auto v : fl)
		{
			cout << v << endl; // 1
		}
	}

	cout << "정렬하기 : sort(정렬함수)" << endl;
	{
		class A {
		public:
			A(int v) { this->v = v; }
			int v;
		};

		forward_list<A> fl = { 1, 2, 3 };
		fl.sort([](A& l, A& r) -> bool {return l.v > r.v; }); // 정렬함수 인자 전달 시 복사를 막기위해 A&로 한다.

		for (auto& v : fl) // 복사를 막기위해 auto&로 한다.
		{
			cout << v.v << endl; // 3, 2, 1
		}
	}

	cout << "중복 제거(인접한 데이터의 중복만 제거 가능) : unique()" << endl;
	{
		// 정렬된 경우
		forward_list<int> fl = { 3, 3, 1, 3, 3, 2, 3, 3 };
		fl.sort();
		fl.unique();
		for (auto v : fl)
		{
			cout << v << endl; // 1, 2, 3
		}

		cout << "---" << endl;

		// 정렬 안된 경우
		fl = { 3, 3, 1, 3, 3, 2, 3, 3 };
		fl.unique();
		for (auto v : fl)
		{
			cout << v << endl; // 3, 1, 3, 2, 3
		}
	}

	cout << "두 forward_list 합치기 : merge()" << endl;
	{
		forward_list<int> fl1 = { 1, 2, 3 };
		forward_list<int> fl2 = { 11, 22, 33 };
		fl1.merge(fl2);
		for (auto v : fl1)
		{
			cout << v << endl; // 1, 2, 3, 11, 22, 33
		}
	}

	cout << "두 forward_list 정렬하여 합치기 : merge()" << endl;
	{
		forward_list<int> fl1 = { 1, 2, 3 };
		forward_list<int> fl2 = { 11, 22, 33 };

		// todo : 정렬함수를 전달하니 에러난다. 왜인지 모르겠다.
		//fl1.merge(fl2);
		//fl1.merge(fl2, std::greater<int>());
		//fl1.merge(fl2, [](auto l, auto r) -> bool {return l > r; });
		//fl1.merge(fl2, [](int& l, int& r) -> bool {return l > r; });
		//fl.sort([](A& l, A& r) -> bool {return l.v > r.v; }); // 정렬함수 인자 전달 시 복사를 막기위해 A&로 한다.
		//for (auto v : fl1)
		//{
		//	cout << v << endl; // 1, 2, 3, 11, 22, 33
		//}
	}

	cout << "두 forward_list 영역 지정하여 합치기 : splice_after()" << endl;
	{
		forward_list<int> fl1 = { 1, 2, 3 };
		forward_list<int> fl2 = { 11, 22, 33 };
		fl1.splice_after(fl1.begin(), fl2);
		for (auto v : fl1)
		{
			cout << v << endl; // 1, 11, 22, 33, 2, 3
		}

		cout << "---" << endl;

		fl1 = { 1, 2, 3 };
		fl2 = { 11, 22, 33 };
		fl1.splice_after(fl1.begin(), fl2, fl2.begin(), fl2.end());
		for (auto v : fl1)
		{
			cout << v << endl; // 1, 22, 33, 2, 3
		}
	}

	return 0;
}
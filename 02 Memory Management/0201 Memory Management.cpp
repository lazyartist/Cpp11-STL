#include <iostream>
#include <memory> // shared_ptr, unique_ptr 사용
#include <vector>

using namespace std;

/*
할당한 메모리를 자동으로 관리해주는 것을 '스마트 포인터'라고 하고 shared_ptr, unique_ptr 등을 말한다.
C++03에 auto_ptr이 있지만 사용이 까다로워 거의 사용하지 않았다.

성능상 손해가 거의 없으면서 메모리 관리의 편의성을 얻을 수 있다.
*/

int main() {
	cout << "shared_ptr" << endl;
	{
		class A {};

		shared_ptr<int> sp(new int(9));
		cout << sp.use_count() << endl; // 1, 참조 카운트
		{
			shared_ptr<int> sp2 = sp;
			cout << sp.use_count() << endl; // 2, 참조 카운트
		}
		cout << sp.use_count() << endl; // 1, 참조 카운트

		cout << (sp.get()) << endl; // 001D8A28, 관리 객체의 포인터 얻기
		cout << (*sp) << endl; // 9, 관리 객체의 참조 얻기
		//cout << (sp->...) << endl; // 1, 관리 객체의 멤버에 접근
	}

	cout << "shared_ptr 관리 객체 삭제 및 다른 객체로 바꾸기 : reset()" << endl;
	{
		shared_ptr<int> sp(new int(9));
		int* i = sp.get();
		cout << *i << endl; // 9

		sp.reset(new int(1));
		cout << *i << endl; // -572662307, reset 후 쓰레기값, 기존 객체 파괴
		i = sp.get();

		sp.reset();
		cout << *i << endl; // -572662307, reset 후 쓰레기값, 기존 객체 파괴
		cout << (sp.get()) << endl; // 00000000, reset()시 새 값을 넣지 않아서 기존 객체만 파괴됨
	}

	cout << "두 shared_ptr 끼리 객체 교환하기" << endl;
	{
		shared_ptr<int> sp1(new int(1));
		shared_ptr<int> sp2(new int(2));

		sp1.swap(sp2);

		cout << *sp1 << endl; // 2
		cout << *sp2 << endl; // 1
	}

	cout << "shared_ptr 배열 객체 다루기 및 삭제자 지정" << endl;
	{
		/*
		shared_ptr을 생성할 때 삭제자를 사용하여 배열임을 알려준다.
		*/
		class A {
		public:
			~A() { cout << "~A" << endl; }
		};

		{
			shared_ptr<A[]> sp(new A[2], default_delete<A[]>()); // 기본 삭제자 지정
			// sp객체가 파괴되며 A객체들도 파괴된다. ~A, ~A 두 번 출력됨

			//(*sp); // todo 이게 왜 안될까?
		}

		{
			shared_ptr<A[]> sp(new A[2], [](A* a) { // 람다식으로 사용자 정의 삭제자 지정
				delete[] a;
			});
			// sp객체가 파괴되며 A객체들도 파괴된다. ~A, ~A 두 번 출력됨
		}
	}

	cout << "간단하게 shared_ptr 만들기" << endl;
	{
		auto sp = make_shared<int>(1);
		cout << typeid(sp).name() << endl; // class std::shared_ptr<int>
	}

	cout << "shared_ptr 순환참조 발생" << endl;
	{
		class A {
		public:
			~A(){cout << "~A" << endl;}
			shared_ptr<A> other;
		};

		shared_ptr<A> sp1(new A());
		//auto sp1 = make_shared<A>(new A()); // todo 왜 이건 컴파일 에러날까?

		{
			shared_ptr<A> sp2(new A());
			//auto sp2 = make_shared<A>(new A());
			sp1->other = sp2;
			sp2->other = sp1;
		}
		// 순환참조가 발생하여 ~A, ~A가 출력되지 않는다.
	}

	cout << "unique_ptr" << endl;
	{
		/*
		shared_ptr과는 다르게 객체를 독점적으로 관리한다.
		unique_ptr은 원칙적으로 복사 생성자와 할당 연산자가 구현되어있지 않다. 즉, 복사를 할 수 없다. 단지 이동(move)만 가능하다.

		사용법은 shared_ptr과 거의 동일하고 use_count()가 없고, release()가 추가됐다.

		배열객체는 삭제자를 지정하지 안해도 된다.
		삭제자 지정하려면 템플릿 인자에 타입을 선언해줘야하는 점이 shared_ptr과 다르다.
		*/

		unique_ptr<int> ut1(new int(1));
		cout << ut1.get() << endl; // 012C4A20, 관리하는 객체가 있다.

		//unique_ptr<int> ut2 = ut1; // 복사 생성자가 없기 때문에 컴파일 에러.
		unique_ptr<int> ut2 = std::move(ut1); // move를 이용해 다른 객체에 할당

		cout << ut1.get() << endl; // 00000000, move했으므로 관리하는 객체가 없다.
		cout << ut2.get() << endl; // 012C4A20, ut1이 관리하던 객체를 관리한다.

		ut2.release();
		cout << ut2.get() << endl; // 00000000, 관리하던 객체를 해제했다.

		// 삭제자 지정하기
		void deleteArray(int* arr);
		unique_ptr<int[], decltype(&deleteArray)> arrut(new int[3], deleteArray);
	}

	return 0;
}

void deleteArray(int* arr) { delete[] arr; }

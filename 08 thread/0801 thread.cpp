#include <iostream>
#include <thread> // thread 사용
#include <mutex> // thread 사용
#include <chrono>
#include <Windows.h>
#include <string>

using namespace std;

/*
thread 라이브러리를 사용하면 복잡하고 플랫폼마다 달리 사용했던 기존의 스레드 프로그래밍을
단순하고 플랫폼 독립적으로 구현할 수 있다.

join()을 호출하면 해당 쓰레드가 종료될 때까지 기다렸다가 다음 코드를 실행한다.
따라서 스레드가 종료되지 않았고 join()을 호출하지 않은 상태에서 프로그램이 종료되면 에러가 발생한다.
또한 스레드를 순서대로 실행시키려면 join이후에 다른 스레드를 실행하면 된다.

만약 join(), detach()하지 않고 프로그램이 종료되면 thread 소멸자에서 terminate()가 호출되어 에러창이 뜬다.
따라서 프로그램 종료 전 join()로 스레드 종료까지 대기하거나
detach()로 스레드 객체와 커널 스레드를 분리하여 안전하게 종료되도록 해야한다.
*/

void fp(string tname) {
	cout << tname << ", f" << endl;
}

int main() {
	// thread 생성 방법 : http://snowdeer.github.io/c++/2017/08/18/cpp11-thread-example/
	cout << "Thread 생성: 함수 포인터" << endl;
	{
		thread t = thread(fp, "t"); // t1, f
		t.join();
	}

	cout << "Thread 생성: 람다 표현식" << endl;
	{
		thread t = thread([]() {
			cout << "t, lamda" << endl; // t1, lamda
		});
		t.join();
	}

	cout << "Thread 생성: 함수 객체" << endl;
	{
		// 함수 객체 정의
		class FO {
		public:
			FO() {}
			FO(int i) {} // thread t1(FO(0)); 를 사용하기 위해서 인자를 받는 생성자를 정의한다.
			void operator()() {
				cout << "FO()" << endl;
			}
		};

		// 클래스 생성자에 파라미터가 없을 경우 컴파일 에러가 난다.
		// 따라서 방법2 방식으로 생성하는 것이 좋다.
		// 방법 1.
		thread t1(FO(0)); // FO()

		// 방법 2.
		thread t2{FO()}; // FO()
		// todo : 클래스 변수명 {} : 이 문법이 뭔지 모르겠다.

		// 방법 3.
		FO fo;
		thread t3 = thread(fo); // FO()

		t1.join();
		t2.join();
		t3.join();
	}

	cout << "Thread 생성: 클래스 메소드" << endl;
	{
		// 클래스 정의
		class C {
		public:
			void f() {
				cout << "C.f()" << endl;
			}
		};

		// todo 왜 thread 생성자 정의가 thread(_Fn&& _Fx, _Args&&... _Ax)
		// &&를 사용했는지 모르겠다.
		C c;
		thread t = thread(&C::f, c); // C.f()
		//thread t = thread(&C::f, &c); // &c로 전달해도 결과는 동일, 왜 c, &c 둘다 전달이 가능할까?

		t.join();
	}

	cout << "스레드 식별자와 스레드 교환" << endl;
	{
		std::mutex mtx;

		int c1 = 0;
		thread t1 = thread([&/*범위내 모든 변수를 캡쳐*/]() {
			//chrono::system_clock::time_point start = chrono::system_clock::now();
			//Sleep(100); // 0.1초 대기

			//// 0.1초 대기가 맞는지 확인
			//chrono::system_clock::time_point end = chrono::system_clock::now();
			//chrono::milliseconds milli = chrono::duration_cast<chrono::milliseconds>(end - start); // 100
			//chrono::seconds sec = chrono::duration_cast<chrono::seconds>(end - start); // 0

			for (size_t i = 0; i < 5; i++)
			{
				++c1;
				mtx.lock();
				sqrt(123.456L);
				cout << "t1 count: " << c1 << ", id: " << t1.get_id() << endl; // get_id()로 스레드 아이디 출력
				mtx.unlock();
			}
		});

		int c2 = 0;
		thread t2 = thread([&]() {
			//Sleep(100); // 0.1초 대기
			for (size_t i = 0; i < 5; i++)
			{
				++c2;
				mtx.lock();
				sqrt(123.456L);
				cout << "t2 count: " << c2 << ", id: " << t2.get_id() << endl; // get_id()로 스레드 아이디 출력
				mtx.unlock();
			}
		});

		// t1과 t2가 스레드 id를 제대로 할당받은 후 swap하기 위해서 일시 대기
		while (c1 < 3 && c2 < 3)
		{
		}

		mtx.lock();
		cout << "swap t1:" << t1.get_id() << ", t2:" << t2.get_id() << endl;
		mtx.unlock();

		t1.swap(t2);

		/*
		swap 후 id가 바뀌는 것은 확인했지만 t2가 먼저 찍히고 t1이 나중에 찍힌다.
		그리고 t1의 id가 0이되는 현상이 있다. 왜인지는 모르겠다.
		t1, t2가 번갈아 찍히기도 하고 t2가 다 찍힌 후 t1이 찍히기도 한다. 그때그때 다르다.
		*/

		t1.join();
		t2.join();
	}

	// concurrency(~cy: 명서형 어미): 동시 실행
	cout << "하드웨어 스레드 개수: hardware_concurrency()" << endl;
	{
		/*
		권장되는 스레드 개수
		관례적으로 (하드웨어 스레드 수 * 2) + 1
		*/
		int hardware_concurrency = thread::hardware_concurrency();
		int tcnt = ((hardware_concurrency * 2) + 1);
		cout << hardware_concurrency << endl; // 6
		cout << tcnt << endl; // 13

		// 권장되는 스레드 개수만큰 스레드 생성
		mutex mut;
		thread *ts = new thread[tcnt];
		for (size_t i = 0; i < tcnt; i++)
		{
			ts[i] = thread([&mut](int id) {
				for (size_t j = 0; j < 3; j++)
				{
					mut.lock();
					sqrt(1.1L);
					cout << "thread: " << id << ", cnt: " << j << endl;
					mut.unlock();
				}
			}, i);
		}

		// 스레드 종료까지 대기
		for (size_t i = 0; i < tcnt; i++)
		{
			ts[i].join();
		}

		// 동적 할당된 스레드객체 제거
		delete[] ts;
	}

	cout << "스레드 떼어내기: detach()" << endl;
	{
		/*
		detach()는 스레드 객체와 실제 스레드의 연결을 끊는다.
		더 이상 스레드 객체로 스레드를 제어할 수 없다.
		단, 실제 스레드가 종료되는 것은 아니다.
		*/
		auto t = thread([](int c) {
			for (size_t i = 0; i < 10; i++)
			{
				cout << c++ << endl;
			}
		}, 1);

		t.detach();
		// detach() 이 후에도 스레드 함수는 계속 호출되지만 
		// 프로그램이 종료되면 함수가 더 이상 호출되지 않는다.
		// 프로그램 종료 시 관련 스레드를 운영체제가 함께 종료하는거 같다.

		if (t.joinable()) // t.join()을 호출할 수 있는지 검사한다.
		{
			t.join(); //detach() 호출 후 join을 호출하면 더 이상 t객체가 스레드를 관리하지 않으므로 실행 시 에러가난다.
		}
	}

	cout << "스레드 일시중지: sleep_for()" << endl;
	{
		// sleep_for()가 호출되는 스코프의 스레드를 일시정지 시킨다.
		chrono::seconds sec(0);

		thread t1 = thread([&]() {
			cout << "t1 pause start" << endl;
			this_thread::sleep_for(sec);
			cout << "t1 pause end" << endl;
		});

		cout << "메인 스레드 pause start" << endl;
		this_thread::sleep_for(sec);
		cout << "메인 스레드 pause end" << endl;

		thread t2 = thread([&]() {
			cout << "t2 pause start" << endl;
			this_thread::sleep_for(sec);
			cout << "t2 pause end" << endl;
		});

		/*
		메인 스레드 pause start
		t1 pause start
		메인 스레드 pause end
		t1 pause end
		t2 pause start
		t2 pause end
		*/

		t1.join();
		t2.join();
	}

	cout << "스레드 일시중지: sleep_until()" << endl;
	{
		// sleep_until()가 호출되는 스코프의 스레드를 일시정지 시킨다.
		chrono::system_clock::time_point now = chrono::system_clock::now();
		chrono::seconds sec(0);

		thread t1 = thread([&]() {
			cout << "t1 pause start" << endl;
			this_thread::sleep_until(now + sec);
			cout << "t1 pause end" << endl;
		});

		cout << "메인 스레드 pause start" << endl;
		this_thread::sleep_until(now + sec);
		cout << "메인 스레드 pause end" << endl;

		now = chrono::system_clock::now();
		thread t2 = thread([&]() {
			cout << "t2 pause start" << endl;
			this_thread::sleep_until(now + sec);
			cout << "t2 pause end" << endl;
		});

		/*
		sleep_for()와 다르게, "t1 pause start"가 먼저 출력된건 스레드 순서에 따른거라 크게 의미 없나?

		t1 pause start
		메인 스레드 pause start
		t1 pause end
		메인 스레드 pause end
		t2 pause start
		t2 pause end
		*/

		t1.join();
		t2.join();
	}

	cout << "스레드 양보: yield()" << endl;
	{
		// sleep_until()가 호출되는 스코프의 스레드를 일시정지 시킨다.
		chrono::system_clock::time_point now = chrono::system_clock::now();
		chrono::seconds sec(1);

		mutex mut;

		int cnt = 10;
		thread t1 = thread([&]() {
			for (size_t i = 0; i < cnt; i++)
			{
				this_thread::yield();
				mut.lock();
				cout << "1" << endl;
				mut.unlock();

			}
			cout << "1 end" << endl;
		});

		thread t2 = thread([&]() {
			for (size_t i = 0; i < cnt; i++)
			{
				mut.lock();
				cout << "2" << endl;
				mut.unlock();
			}
			cout << "2 end" << endl;
		});

		/*
		yield해도 1이 먼저 찍히고나서 2가 찍힌다.
		예제로 작동을 확인하기가 어렵다.
		*/

		t1.join();
		t2.join();
	}

	return 0;
}
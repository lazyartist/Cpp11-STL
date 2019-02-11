#include <iostream>
#include <thread> // thread 사용
#include <mutex> // thread 사용
#include <chrono>
#include <Windows.h>

using namespace std;

/*
*/

int main() {
	cout << "Thread 생성" << endl;
	{
		thread t1 = thread([]() {
			for (size_t i = 0; i < 5; i++)
			{
				cout << "t1: " << i << endl;
			}
		});

		/*
		join()을 호출하면 해당 쓰레드가 종료된 후 join()을 호출한 이후로 넘어간다.
		따라서 스레드가 종료되지 않았고 join()을 호출하지 않은 상태에서 프로그램이 종료되면 에러가 발생한다.
		또한 스레드를 순서대로 실행시키려면 join이후에 다른 스레드를 실행하면 된다.
		*/
		t1.join();

		thread t2 = thread([]() {
			for (size_t i = 10; i < 15; i++)
			{
				cout << "t2: " << i << endl;
			}
		});
		t2.join();
	}

	cout << "스레드 식별자와 스레드 교환" << endl;
	{
		std::mutex mtx;

		int c1 = 0;
		thread t1 = thread([&/*범위내 모든 변수를 캡쳐*/]() {
			chrono::system_clock::time_point start = chrono::system_clock::now();
			Sleep(100); // 0.1초 대기

			// 0.1초 대기가 맞는지 확인
			chrono::system_clock::time_point end = chrono::system_clock::now();
			chrono::milliseconds milli = chrono::duration_cast<chrono::milliseconds>(end - start); // 100
			chrono::seconds sec = chrono::duration_cast<chrono::seconds>(end - start); // 0

			for (size_t i = 0; i < 5; i++)
			{
				++c1;
				mtx.lock();
				cout << "t1 id: " << t1.get_id() << endl; // get_id()로 스레드 아이디 출력
				mtx.unlock();
			}
		});

		int c2 = 0;
		thread t2 = thread([&]() {
			Sleep(100); // 0.1초 대기
			for (size_t i = 0; i < 5; i++)
			{
				++c2;
				mtx.lock();
				cout << "t2 id: " << t2.get_id() << endl; // get_id()로 스레드 아이디 출력
				mtx.unlock();
			}
		});

		// t1과 t2가 스레드 id를 제대로 할당받은 후 swap하기 위해서 일시 대기
		while (c1 < 3 && c2 < 3)
		{
			//
		}

		mtx.lock();
		cout << "swap t1:" << t1.get_id() << ", t2:" << t2.get_id() << endl;
		mtx.unlock();

		t1.swap(t2);

		/*
		swap 후 t1의 id가 0이되는 현상이 있다. 왜인지는 모르겠다.

		todo : 일단 스레드 개념 숙지하자
		*/

		t1.join();
		t2.join();
	}

	cout << "" << endl;
	{
		auto t = thread();
	}

	cout << "" << endl;
	{
		auto t = thread();
	}


	return 0;
}
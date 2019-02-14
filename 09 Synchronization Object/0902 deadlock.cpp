#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex> // mutex 사용

using namespace std;

int deadlock() {
	cout << "데드락 발생 코드" << endl;
	{
		mutex m;

		thread t1 = thread([&]() {
			cout << "t1 0" << endl;
			m.lock();
			cout << "t1 1" << endl;
			//m.unlock();
		});

		thread t2 = thread([&]() {
			cout << "t2 0" << endl;
			m.lock(); // t1이 unlock하지 않았기 때문에 여기서 무한대기한다. 데드락 발생
			cout << "t2 1" << endl;
			//m.unlock();
		});

		t1.join();
		cout << "t1.join();" << endl;
		t2.join();
		cout << "t2.join();" << endl;
	}

	cout << "여러 mutex객체 동시에 lock 걸기: std::lock()" << endl;
	{
		/*
		lock_guard는 생성시에만 lock을 걸고 파괴시에만 unlock할 수 있다.

		unique_lock 객체 생성후에 lock/unlock을 할 수 있다.
		try_lock(), try_lock_for(), try_lock_until() 지원
		unique_lock끼리 mutex객체를 서로 교환 가능
		*/

		auto deadLock = [](string name, mutex* m1, mutex* m2) {
			for (size_t i = 0; i < 100; i++)
			{
				cout << name << " first " << i << endl;
				m1->lock();
				m2->lock();

				cout << name << " second " << i << endl;
				m1->unlock();
				m2->unlock();

				this_thread::sleep_for(chrono::milliseconds(10));
			}
		};

		auto noDeadLock = [](string name, mutex* m1, mutex* m2) {
			for (size_t i = 0; i < 100; i++)
			{
				{
					// defer_lock을 사용하니 std::lock 부분에서 에러난다. unlock이 안되는듯. std::lock 이후로 옮겨도 마찬가지.
					/*unique_lock<mutex> u1((*m1), defer_lock);
					unique_lock<mutex> u2((*m2), defer_lock);*/

					cout << name << " first " << i << endl;
					std::lock((*m1), (*m2));

					// adopt_lock는 정상 작동함.
					unique_lock<mutex> u1((*m1), adopt_lock);
					unique_lock<mutex> u2((*m2), adopt_lock);

					cout << name << " second " << i << endl;

					// 각각 unlock을 해줘도 되지만 unique_lock으로 더 안전하게 unlock한다.
					//m1->unlock();
					//m2->unlock();

					this_thread::sleep_for(chrono::milliseconds(10));
				}
			}
		};

		mutex m1, m2;
		//thread t1 = thread(deadLock, "t1", &m1, &m2);
		//thread t2 = thread(deadLock, "t2", &m2, &m1);

		thread t1 = thread(noDeadLock, "t1", &m1, &m2);
		thread t2 = thread(noDeadLock, "t2", &m2, &m1);

		t1.join();
		t2.join();
	}

	return 0;
}
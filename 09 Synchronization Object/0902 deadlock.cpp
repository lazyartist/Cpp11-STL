#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex> // mutex ���

using namespace std;

int deadlock() {
	cout << "����� �߻� �ڵ�" << endl;
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
			m.lock(); // t1�� unlock���� �ʾұ� ������ ���⼭ ���Ѵ���Ѵ�. ����� �߻�
			cout << "t2 1" << endl;
			//m.unlock();
		});

		t1.join();
		cout << "t1.join();" << endl;
		t2.join();
		cout << "t2.join();" << endl;
	}

	cout << "���� mutex��ü ���ÿ� lock �ɱ�: std::lock()" << endl;
	{
		/*
		lock_guard�� �����ÿ��� lock�� �ɰ� �ı��ÿ��� unlock�� �� �ִ�.

		unique_lock ��ü �����Ŀ� lock/unlock�� �� �� �ִ�.
		try_lock(), try_lock_for(), try_lock_until() ����
		unique_lock���� mutex��ü�� ���� ��ȯ ����
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
					// defer_lock�� ����ϴ� std::lock �κп��� ��������. unlock�� �ȵǴµ�. std::lock ���ķ� �Űܵ� ��������.
					/*unique_lock<mutex> u1((*m1), defer_lock);
					unique_lock<mutex> u2((*m2), defer_lock);*/

					cout << name << " first " << i << endl;
					std::lock((*m1), (*m2));

					// adopt_lock�� ���� �۵���.
					unique_lock<mutex> u1((*m1), adopt_lock);
					unique_lock<mutex> u2((*m2), adopt_lock);

					cout << name << " second " << i << endl;

					// ���� unlock�� ���൵ ������ unique_lock���� �� �����ϰ� unlock�Ѵ�.
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
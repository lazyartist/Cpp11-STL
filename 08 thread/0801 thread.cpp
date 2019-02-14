#include <iostream>
#include <thread> // thread ���
#include <mutex> // thread ���
#include <chrono>
#include <Windows.h>
#include <string>

using namespace std;

/*
thread ���̺귯���� ����ϸ� �����ϰ� �÷������� �޸� ����ߴ� ������ ������ ���α׷�����
�ܼ��ϰ� �÷��� ���������� ������ �� �ִ�.

join()�� ȣ���ϸ� �ش� �����尡 ����� ������ ��ٷȴٰ� ���� �ڵ带 �����Ѵ�.
���� �����尡 ������� �ʾҰ� join()�� ȣ������ ���� ���¿��� ���α׷��� ����Ǹ� ������ �߻��Ѵ�.
���� �����带 ������� �����Ű���� join���Ŀ� �ٸ� �����带 �����ϸ� �ȴ�.

���� join(), detach()���� �ʰ� ���α׷��� ����Ǹ� thread �Ҹ��ڿ��� terminate()�� ȣ��Ǿ� ����â�� ���.
���� ���α׷� ���� �� join()�� ������ ������� ����ϰų�
detach()�� ������ ��ü�� Ŀ�� �����带 �и��Ͽ� �����ϰ� ����ǵ��� �ؾ��Ѵ�.
*/

void fp(string tname) {
	cout << tname << ", f" << endl;
}

int main() {
	// thread ���� ��� : http://snowdeer.github.io/c++/2017/08/18/cpp11-thread-example/
	cout << "Thread ����: �Լ� ������" << endl;
	{
		thread t = thread(fp, "t"); // t1, f
		t.join();
	}

	cout << "Thread ����: ���� ǥ����" << endl;
	{
		thread t = thread([]() {
			cout << "t, lamda" << endl; // t1, lamda
		});
		t.join();
	}

	cout << "Thread ����: �Լ� ��ü" << endl;
	{
		// �Լ� ��ü ����
		class FO {
		public:
			FO() {}
			FO(int i) {} // thread t1(FO(0)); �� ����ϱ� ���ؼ� ���ڸ� �޴� �����ڸ� �����Ѵ�.
			void operator()() {
				cout << "FO()" << endl;
			}
		};

		// Ŭ���� �����ڿ� �Ķ���Ͱ� ���� ��� ������ ������ ����.
		// ���� ���2 ������� �����ϴ� ���� ����.
		// ��� 1.
		thread t1(FO(0)); // FO()

		// ��� 2.
		thread t2{FO()}; // FO()
		// todo : Ŭ���� ������ {} : �� ������ ���� �𸣰ڴ�.

		// ��� 3.
		FO fo;
		thread t3 = thread(fo); // FO()

		t1.join();
		t2.join();
		t3.join();
	}

	cout << "Thread ����: Ŭ���� �޼ҵ�" << endl;
	{
		// Ŭ���� ����
		class C {
		public:
			void f() {
				cout << "C.f()" << endl;
			}
		};

		// todo �� thread ������ ���ǰ� thread(_Fn&& _Fx, _Args&&... _Ax)
		// &&�� ����ߴ��� �𸣰ڴ�.
		C c;
		thread t = thread(&C::f, c); // C.f()
		//thread t = thread(&C::f, &c); // &c�� �����ص� ����� ����, �� c, &c �Ѵ� ������ �����ұ�?

		t.join();
	}

	cout << "������ �ĺ��ڿ� ������ ��ȯ" << endl;
	{
		std::mutex mtx;

		int c1 = 0;
		thread t1 = thread([&/*������ ��� ������ ĸ��*/]() {
			//chrono::system_clock::time_point start = chrono::system_clock::now();
			//Sleep(100); // 0.1�� ���

			//// 0.1�� ��Ⱑ �´��� Ȯ��
			//chrono::system_clock::time_point end = chrono::system_clock::now();
			//chrono::milliseconds milli = chrono::duration_cast<chrono::milliseconds>(end - start); // 100
			//chrono::seconds sec = chrono::duration_cast<chrono::seconds>(end - start); // 0

			for (size_t i = 0; i < 5; i++)
			{
				++c1;
				mtx.lock();
				sqrt(123.456L);
				cout << "t1 count: " << c1 << ", id: " << t1.get_id() << endl; // get_id()�� ������ ���̵� ���
				mtx.unlock();
			}
		});

		int c2 = 0;
		thread t2 = thread([&]() {
			//Sleep(100); // 0.1�� ���
			for (size_t i = 0; i < 5; i++)
			{
				++c2;
				mtx.lock();
				sqrt(123.456L);
				cout << "t2 count: " << c2 << ", id: " << t2.get_id() << endl; // get_id()�� ������ ���̵� ���
				mtx.unlock();
			}
		});

		// t1�� t2�� ������ id�� ����� �Ҵ���� �� swap�ϱ� ���ؼ� �Ͻ� ���
		while (c1 < 3 && c2 < 3)
		{
		}

		mtx.lock();
		cout << "swap t1:" << t1.get_id() << ", t2:" << t2.get_id() << endl;
		mtx.unlock();

		t1.swap(t2);

		/*
		swap �� id�� �ٲ�� ���� Ȯ�������� t2�� ���� ������ t1�� ���߿� ������.
		�׸��� t1�� id�� 0�̵Ǵ� ������ �ִ�. �������� �𸣰ڴ�.
		t1, t2�� ������ �����⵵ �ϰ� t2�� �� ���� �� t1�� �����⵵ �Ѵ�. �׶��׶� �ٸ���.
		*/

		t1.join();
		t2.join();
	}

	// concurrency(~cy: ���� ���): ���� ����
	cout << "�ϵ���� ������ ����: hardware_concurrency()" << endl;
	{
		/*
		����Ǵ� ������ ����
		���������� (�ϵ���� ������ �� * 2) + 1
		*/
		int hardware_concurrency = thread::hardware_concurrency();
		int tcnt = ((hardware_concurrency * 2) + 1);
		cout << hardware_concurrency << endl; // 6
		cout << tcnt << endl; // 13

		// ����Ǵ� ������ ������ū ������ ����
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

		// ������ ������� ���
		for (size_t i = 0; i < tcnt; i++)
		{
			ts[i].join();
		}

		// ���� �Ҵ�� �����尴ü ����
		delete[] ts;
	}

	cout << "������ �����: detach()" << endl;
	{
		/*
		detach()�� ������ ��ü�� ���� �������� ������ ���´�.
		�� �̻� ������ ��ü�� �����带 ������ �� ����.
		��, ���� �����尡 ����Ǵ� ���� �ƴϴ�.
		*/
		auto t = thread([](int c) {
			for (size_t i = 0; i < 10; i++)
			{
				cout << c++ << endl;
			}
		}, 1);

		t.detach();
		// detach() �� �Ŀ��� ������ �Լ��� ��� ȣ������� 
		// ���α׷��� ����Ǹ� �Լ��� �� �̻� ȣ����� �ʴ´�.
		// ���α׷� ���� �� ���� �����带 �ü���� �Բ� �����ϴ°� ����.

		if (t.joinable()) // t.join()�� ȣ���� �� �ִ��� �˻��Ѵ�.
		{
			t.join(); //detach() ȣ�� �� join�� ȣ���ϸ� �� �̻� t��ü�� �����带 �������� �����Ƿ� ���� �� ����������.
		}
	}

	cout << "������ �Ͻ�����: sleep_for()" << endl;
	{
		// sleep_for()�� ȣ��Ǵ� �������� �����带 �Ͻ����� ��Ų��.
		chrono::seconds sec(0);

		thread t1 = thread([&]() {
			cout << "t1 pause start" << endl;
			this_thread::sleep_for(sec);
			cout << "t1 pause end" << endl;
		});

		cout << "���� ������ pause start" << endl;
		this_thread::sleep_for(sec);
		cout << "���� ������ pause end" << endl;

		thread t2 = thread([&]() {
			cout << "t2 pause start" << endl;
			this_thread::sleep_for(sec);
			cout << "t2 pause end" << endl;
		});

		/*
		���� ������ pause start
		t1 pause start
		���� ������ pause end
		t1 pause end
		t2 pause start
		t2 pause end
		*/

		t1.join();
		t2.join();
	}

	cout << "������ �Ͻ�����: sleep_until()" << endl;
	{
		// sleep_until()�� ȣ��Ǵ� �������� �����带 �Ͻ����� ��Ų��.
		chrono::system_clock::time_point now = chrono::system_clock::now();
		chrono::seconds sec(0);

		thread t1 = thread([&]() {
			cout << "t1 pause start" << endl;
			this_thread::sleep_until(now + sec);
			cout << "t1 pause end" << endl;
		});

		cout << "���� ������ pause start" << endl;
		this_thread::sleep_until(now + sec);
		cout << "���� ������ pause end" << endl;

		now = chrono::system_clock::now();
		thread t2 = thread([&]() {
			cout << "t2 pause start" << endl;
			this_thread::sleep_until(now + sec);
			cout << "t2 pause end" << endl;
		});

		/*
		sleep_for()�� �ٸ���, "t1 pause start"�� ���� ��µȰ� ������ ������ �����Ŷ� ũ�� �ǹ� ����?

		t1 pause start
		���� ������ pause start
		t1 pause end
		���� ������ pause end
		t2 pause start
		t2 pause end
		*/

		t1.join();
		t2.join();
	}

	cout << "������ �纸: yield()" << endl;
	{
		// sleep_until()�� ȣ��Ǵ� �������� �����带 �Ͻ����� ��Ų��.
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
		yield�ص� 1�� ���� �������� 2�� ������.
		������ �۵��� Ȯ���ϱⰡ ��ƴ�.
		*/

		t1.join();
		t2.join();
	}

	return 0;
}
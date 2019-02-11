#include <iostream>
#include <thread> // thread ���
#include <mutex> // thread ���
#include <chrono>
#include <Windows.h>

using namespace std;

/*
*/

int main() {
	cout << "Thread ����" << endl;
	{
		thread t1 = thread([]() {
			for (size_t i = 0; i < 5; i++)
			{
				cout << "t1: " << i << endl;
			}
		});

		/*
		join()�� ȣ���ϸ� �ش� �����尡 ����� �� join()�� ȣ���� ���ķ� �Ѿ��.
		���� �����尡 ������� �ʾҰ� join()�� ȣ������ ���� ���¿��� ���α׷��� ����Ǹ� ������ �߻��Ѵ�.
		���� �����带 ������� �����Ű���� join���Ŀ� �ٸ� �����带 �����ϸ� �ȴ�.
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

	cout << "������ �ĺ��ڿ� ������ ��ȯ" << endl;
	{
		std::mutex mtx;

		int c1 = 0;
		thread t1 = thread([&/*������ ��� ������ ĸ��*/]() {
			chrono::system_clock::time_point start = chrono::system_clock::now();
			Sleep(100); // 0.1�� ���

			// 0.1�� ��Ⱑ �´��� Ȯ��
			chrono::system_clock::time_point end = chrono::system_clock::now();
			chrono::milliseconds milli = chrono::duration_cast<chrono::milliseconds>(end - start); // 100
			chrono::seconds sec = chrono::duration_cast<chrono::seconds>(end - start); // 0

			for (size_t i = 0; i < 5; i++)
			{
				++c1;
				mtx.lock();
				cout << "t1 id: " << t1.get_id() << endl; // get_id()�� ������ ���̵� ���
				mtx.unlock();
			}
		});

		int c2 = 0;
		thread t2 = thread([&]() {
			Sleep(100); // 0.1�� ���
			for (size_t i = 0; i < 5; i++)
			{
				++c2;
				mtx.lock();
				cout << "t2 id: " << t2.get_id() << endl; // get_id()�� ������ ���̵� ���
				mtx.unlock();
			}
		});

		// t1�� t2�� ������ id�� ����� �Ҵ���� �� swap�ϱ� ���ؼ� �Ͻ� ���
		while (c1 < 3 && c2 < 3)
		{
			//
		}

		mtx.lock();
		cout << "swap t1:" << t1.get_id() << ", t2:" << t2.get_id() << endl;
		mtx.unlock();

		t1.swap(t2);

		/*
		swap �� t1�� id�� 0�̵Ǵ� ������ �ִ�. �������� �𸣰ڴ�.

		todo : �ϴ� ������ ���� ��������
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
#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex> // mutex ���
#include <condition_variable> // condition_variable ���

using namespace std;

/*
���� �����忡�� �ϳ��� �����ڿ��� ����� �� ������ ����(data-race)�� �Ͼ��.
Ÿ�ֿ̹� ���� ������� ����Ҽ��� �׷��� ���� ���� �ִ�.

lock���¸� �����ϰ� ���� ������� ���� mutex ��ü�� ����ؾ��Ѵ�.
lock ȣ�� �� �ٸ� �����尡 lock�� �����̸� lock ȣ���� ������ ����ϰ� �ִٰ�
�ٸ� �����尡 unlock ȣ���ϸ� �׶����� lock ȣ�� ���� �ڵ���� ����ȴ�.

mutex ��� ������ �����ڿ��� ������ �Ͼ�� �ʰ��ϴ� ��������
�����ڿ��� ��뿩�ο� ������� mutex.lock���� �ٸ� �������� ������ ���� �� �ִ�.

�������� �������� mutex ��ü�� �ı��Ǳ� ���� unlock���� ������ ��������.
*/

int synchronization_object() {
	cout << "���� �ڿ��� ��ױ�: mutex.lock(), mutex.unlock()" << endl;
	{
		/*
		���� �ڿ��� ����� �� lock�� �ɾ� �ٸ� �����尡 �������� ���ϰ� �ϰ� �� ����� �� unlock���� �����Ѵ�.
		unlock���� ������ ������� �����Ƿ� �ݵ�� unlock������Ѵ�.
		���� �ʹ� ���� lock/unlock�� ������ ��� �ð��� �þ�Ƿ� ó���ð��� �������.
		*/

		mutex m;
		cout << "main" << endl;

		m.lock();
		//m.lock(); // �ϳ��� �����忡�� lock�� �ι� ȣ���ϸ� ��������.

		thread t = thread([&]() {
			m.lock(); // ���� �����忡�� lock�߱� ������ unlock�� ������ ���⼭ ����ϰ� �ȴ�.
			cout << "t" << endl;
			m.unlock(); // lock�� ������ ����� ���� unlock�ؾ��Ѵ�. �ȱ׷��� ��ü �Ҹ� �� "mutex destroyed while busy" ��Ÿ�� ������ ����
			// �� �ۿ��� unlock�� ȣ���ص� ���� ������������ lock�� �ƴ϶� unlock���� �ʴ´�.
		});

		Sleep(1000);
		m.unlock();

		t.join();

		/* Result
		main t
		t // 0.5�� ��
		*/
	}

	cout << "���� �ڿ��� ��ױ�: mutex.try_lock()" << endl;
	{
		/*
		�ٸ� �����尡 lock�� �ɾ� �۾��� �� ���� �� lock�� Ǯ�������� �ٸ� ���� �ϰ� ���� ��� ���
		*/

		mutex m;
		cout << "main" << endl;

		m.lock();
		//m.lock(); // �ϳ��� �����忡�� lock�� �ι� ȣ���ϸ� ��������.

		thread t = thread([&]() {
			while (true)
			{
				if (m.try_lock()) // lock�� �ɼ� ������ lock�� �ɰ� true ��ȯ
				{
					cout << "t" << endl; // lock�� �ɷ����� �����ڿ� ó��
					m.unlock();
					break;
				}
				else {
					cout << "..." << endl; // lock�� ���ɾ����� �ٸ��� ó��
				}
				Sleep(100);
			}
		});

		Sleep(100);
		m.unlock();

		t.join();

		/* Result
		main t
		...
		t
		*/
	}

	cout << "unlock���� �ʴ� �Ǽ� ����: lock_guard Ŭ����" << endl;
	{
		/*
		�Ǽ��� unlock���� �ʾƵ� �ڵ� ���� �������� �� unlock�� ȣ�����ش�.
		mutex�� ������ ������ ���� RAII(���ҽ� Ȯ�� �� �ʱ�ȭ) ������ ������ ���̴�.
		todo raii ã�ƺ���
		*/

		mutex m;
		cout << "main" << endl;

		m.lock();
		//m.lock(); // �ϳ��� �����忡�� lock�� �ι� ȣ���ϸ� ��������.

		thread t = thread([&]() {
			lock_guard<mutex> lg(m); // lock_guard ��ü ���� �� m�� lock()ȣ��
			cout << "t" << endl;
			// �׳� ���������� �ڵ����� unlock()�� ȣ��ȴ�.
		});

		Sleep(500);
		m.unlock();

		t.join();

		/* Result
		main t
		t // 0.5�� ��
		*/
	}


	cout << "�ݺ��Ͽ� lock �ɱ�: recursive_mutex Ŭ����" << endl;
	{
		/*
		unlock���� �ʰ� ���� �����忡�� lock�� �ٽ� �� ��� ������ �߻��Ѵ�.
		recursive_mutex ��ü�� ������ lock�Ŵ� ���� ����Ѵ�.
		��, lockȽ����ŭ unlock�� ������Ѵ�.
		*/

		recursive_mutex rm;

		rm.lock();
		rm.lock();
		rm.unlock();
		rm.unlock(); // lock Ƚ���� unlock Ƚ���� ���� ������ ���α׷�(Ȥ�� �ڵ��) ���� �� ��������.
	}

	cout << "������ �ð� ���ȸ� lock �õ��ϱ�: timed_mutex.try_lock_for(), timed_mutex.try_lock_until(), recursive_timed_mutex" << endl;
	{
		/*
		Ư�� �ð����ȸ� lock�� �õ��ϰ� �ð����� lock���� ���ϸ� �׸��д�.
		mutex�� ��ɰ� �����ϰ� try_lock_for(), try_lock_until()�Լ��� �߰��ƴ�.

		recursive_timed_mutex: recursive_mutex�� timed_mutex ����� �߰��Ȱ�.
		*/

		timed_mutex tm;
		tm.lock();
		cout << "main locked" << endl;

		thread t = thread([&]() {
			//if (tm.try_lock_for(chrono::milliseconds(110))) { // �����ð����� 
			//if (tm.try_lock_until(chrono::system_clock::now() + chrono::milliseconds(10))) { // �����ð�����
			if (tm.try_lock_until(chrono::system_clock::now() + chrono::milliseconds(10))) { // �����ð�����
				cout << "t locked" << endl;
				tm.unlock();
			}
			else {
				cout << "t lock failed" << endl;
			}

			cout << "t end" << endl;
		});

		Sleep(100);
		cout << "main unlocked" << endl;
		tm.unlock();

		t.join();

		/* Result
		// 100�и��� �̻�
		main locked
		main unlocked
		t locked
		t end

		// 100�и��� ����
		main locked
		t lock failed // main �����尡 Sleep�ϴ� ���� lock�� �����ؼ� ���� �ڵ尡 ����ƴ�.
		t end
		main unlocked
		*/
	}

	cout << "���� ����� lock_guard: unique_lock Ŭ����" << endl;
	{
		/*
		lock_guard�� �����ÿ��� lock�� �ɰ� �ı��ÿ��� unlock�� �� �ִ�.

		unique_lock ��ü �����Ŀ� lock/unlock�� �� �� �ִ�.
		try_lock(), try_lock_for(), try_lock_until() ����
		unique_lock���� mutex��ü�� ���� ��ȯ ����

		*/

		timed_mutex tm;

		//unique_lock<timed_mutex> ul1(tm, adopt_lock); // unique_lock ������ ���ÿ� mutex�� �������� ���´�.
		//ul1.lock(); // �����Ǹ� lock�Ʊ� ������ lock()�ϸ� ����. �׷��� ������ lock���� �ʴ´�.
		//tm.lock(); // �̷��� mutex��ü�� lock����� lock�� �ȴ�. ������ �𸣰ڴ�.

		unique_lock<timed_mutex> ul1(tm, defer_lock); // unique_lock�� ���������� mutex�� �������� ���� �ʴ´�.
		ul1.lock(); // �����ϸ� lock�� ���� �ʱ� ������ �ʿ������ lock���ش�.

		cout << "main locked" << endl;

		thread t = thread([&]() {
			unique_lock<timed_mutex> ul2(tm, defer_lock);
			if (ul2.try_lock_for(chrono::milliseconds(1100))) {
				cout << "t locked" << endl;
				//uld.unlock(); // unlock���� �ʾƵ� �����ȳ���.
			}
			else {
				cout << "t lock failed" << endl;
			}

			cout << "t end" << endl;
		});

		this_thread::sleep_for(chrono::seconds(1));
		cout << "main unlocked" << endl;
		ul1.unlock();

		t.join();

		/* Result
		// 1�� �̻�
		main locked
		main unlocked
		t locked
		t end

		// 1�� ����
		main locked
		t lock failed // main �����尡 Sleep�ϴ� ���� lock�� �����ؼ� ���� �ڵ尡 ����ƴ�.
		t end
		main unlocked
		*/
	}

	cout << "�� ���� �����ϱ�: std::call_once, std::once_flag" << endl;
	{
		/*
		��Ƽ ������ ȯ�濡�� �Լ��� �ѹ��� ȣ���ϰ� �Ѵ�.
		*/
		auto f = []() {
			cout << "once f" << endl;
		};

		once_flag once;
		for (size_t i = 0; i < 10; i++)
		{
			call_once(once, f); // once f �� ���� ��µȴ�.
		}
	}

	cout << "�����帶�� ������ �����ϱ�: thread_local" << endl;
	{
		/*
		�� �����尡 ������ ���� �޸𸮿� �����͸� �����ϵ��� �Ѵ�.
		���� �ڿ��� �������Ƿ� ���۵��� ���� �� �ִ�.
		*/

		//int cnt = 0; // ���ڰ� 1�� �ȴ�.
		thread_local int cnt = 0; // ���ڰ� 0�̴�.
		mutex m;
		auto f = [&]() {
			m.lock();
			cout << "f: " << cnt++ << endl;
			m.unlock();
		};

		thread t1 = thread(f);
		thread t2 = thread(f);

		t1.join();
		t2.join();
	}

	cout << "�̺�Ʈ�� ���� ����ȭ ��ü: thread_local" << endl;
	{
		/*
		������ �� Ư�� ������ ������ ������ ����ϰ� �ִٰ� �̺�Ʈ�� ������ ��⸦ Ǯ���� �� ����Ѵ�.
		unique_lock_any�� unique_lock<mutex> �̿��� �͵� ����� �� �ִ�.
		�ҽ�����: https://en.cppreference.com/w/cpp/thread/condition_variable
		*/

		condition_variable cv;

		int cnt = 0;
		mutex m;
		auto fstart = [&]() {
			this_thread::sleep_for(chrono::milliseconds(10));

			cout << "fstart: " << cnt++ << endl;
			cv.notify_one();
		};

		auto fchain = [&]() {
			// defer_lock, adopt_lock�� �����ϸ� condition_variable�� �۵����� �ʴ´�.
			// ����ϰ� �ִٰ� lock�� �ɾ���ؼ� �׷����ϴ�.
			unique_lock<mutex> u(m);
			cv.wait(u);
			//cv.wait(u, []() {cout << "wake up" << endl; return false; }); // return false;�ϸ� wake up ���� �ʴ´�.
			cout << "fchain: " << cnt++ << endl;

			// �۾��� �޷�ǰ� notify�� ȣ������ ������ ������� �ɸ� �� �ִ�.
			cv.notify_one();
		};

		thread t1 = thread(fstart);
		thread t2 = thread(fchain);
		thread t3 = thread(fchain);

		t1.join();
		t2.join();
		t3.join();
	}

	return 0;
}
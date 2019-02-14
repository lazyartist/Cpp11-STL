#include <iostream>
#include <chrono>
#include <future> // async/future ���
#include <time.h>
#include <vector>

using namespace std;

/*
C++11�� �߰��� async/future�� ����Ͽ� �����ϰ� �񵿱� ���α׷����� �� �� �ִ�.
*/

int main() {
	time_t time;
	cout << sizeof(time_t) << endl;
	cout << sizeof(long) << endl;
	cout << sizeof(int) << endl;
	cout << "���� �� ����" << endl;
	{
		class A {
		public:
			A() {
				cout << "A" << endl;
			}
		};

		auto func = []() {
			cout << "func" << endl;
			return A();
		};

		// �׸��� launch::async�� ���� �Լ��� �ٷ� ����ǹǷ� func, A �� ���ʷ� ������.
		future<A> ft = async(launch::async, func);

		// sleep���� ������ main�� ���� ������.
		this_thread::sleep_for(chrono::milliseconds(10));
		cout << "main" << endl;
		A r = ft.get(); // �����°� ����.
		/* Result
		A
		func
		A
		main
		*/
		cout << "=====================" << endl;

		// future ��ü �������ߴµ� "A"�� ������. ���������� A�� �����س��µ�?
		//async(launch::deferred, func); 

		// �׸��� launch::deferred�� ���� �Լ� ����ϹǷ� �ƹ��͵� ������ �ʴ´�.
		ft = async(launch::deferred, func);

		// sleep���� ������ main�� ���� ������.
		this_thread::sleep_for(chrono::milliseconds(10));
		cout << "main" << endl;
		r = ft.get(); // get()ȣ��� �Լ��� ����ǹǷ� "func", "A"�� ������.
		/* Result
		A
		main
		func
		A
		*/
	}

	cout << "std::thread���� �񵿱�� �Լ� ����: std::promise, std::future" << endl;
	{
		/*
		std::promise, std::future�� ����ϸ� �ٸ� thread���� ������ ����� ���� �� �ִ�.
		*/

		auto func = [](promise<vector<int>>& pv) {
			cout << "thread id: " << this_thread::get_id() << endl;

			vector<int> vt;
			for (size_t i = 0; i < 5; i++)
			{
				vt.push_back(i);
			}

			pv.set_value(vt);
			//pv->set_value(vt);
		};

		promise<vector<int>> pv;
		future<vector<int>> ft = pv.get_future();

		// todo std::reference_wrapper https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
		thread t = thread(func, std::ref(pv)); // todo std::ref: pv�� rvalue�� ������ش�?
		//thread t = thread(func, std::ref(pv)); // func�� ���� �Լ��ΰ�� &func�� �����ؾ��Ѵ�.
		t.detach();
		auto r = ft.get();

		for (size_t i = 0; i < r.size(); i++)
		{
			cout << r.at(i) << endl;
		}
		cout << "main thread id: " << this_thread::get_id() << endl;
		/* Result
		thread id: 15408
		0
		1
		2
		3
		4
		main thread id: 17664
		*/
	}

	cout << "�񵿱� task" << endl;
	{
		/*
		VC10������ MS�� ���� ���� ���α׷��� ���̺귯���� Concurrency Runtime�� task ���̺귯���� ����ϸ� �����ϰ� �񵿱� ���α׷����� �� �� �ִ�.
		C++11������ std::package_task Ŭ������ �̿��ϸ� Concurrency Runtime�� task�� ����� ���� ���� �� �ִ�.

		package_task�� �����ϸ鼭 �񵿱�� ������ �Լ��� �ѱ�� ����� future�� ���ؼ� �޵��� �� ��, 
		thread ���̺귯���� std::package_task ��ü�� ���� ������ �־��ָ� �ȴ�.
		*/

		auto func = [](int tmp) {
			cout << "thread id: " << this_thread::get_id() << ", tmp: " <<tmp << endl;

			vector<int> vt;
			for (size_t i = 0; i < 5; i++)
			{
				vt.push_back(i);
			}

			return vt;
		};

		// vector iterator ����
		//auto r = func(1);
		//auto iter = r.begin();
		//while (iter != r.end())
		//{
		//	cout << (*iter++) << endl;
		//}
		//for (auto i : r) { // i�� iterator::value_type
		//	cout << i << endl;
		//}

		packaged_task<vector<int>(int)> task(func); // todo (int)�� �� ������?
		future<vector<int>> ft = task.get_future();
		thread t = thread(move(task), 99); // todo move Ŭ���� ����?
		t.detach();

		vector<int> r = ft.get();
		for (size_t i = 0; i < r.size(); i++)
		{
			cout << r.at(i) << endl;
		}
		cout << "main thread id: " << this_thread::get_id() << endl;

		/* Result
		thread id: 29216, tmp: 99
		0
		1
		2
		3
		4
		main thread id: 17664
		*/
	}

	return 0;
}
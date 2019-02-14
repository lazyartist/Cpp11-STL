#include <iostream>
#include <chrono>
#include <future> // async/future 사용
#include <time.h>
#include <vector>

using namespace std;

/*
C++11에 추가된 async/future를 사용하여 간단하게 비동기 프로그래밍을 할 수 있다.
*/

int main() {
	time_t time;
	cout << sizeof(time_t) << endl;
	cout << sizeof(long) << endl;
	cout << sizeof(int) << endl;
	cout << "생성 및 실행" << endl;
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

		// 그리고 launch::async에 의해 함수가 바로 실행되므로 func, A 가 차례로 찍힌다.
		future<A> ft = async(launch::async, func);

		// sleep하지 않으면 main이 먼저 찍힌다.
		this_thread::sleep_for(chrono::milliseconds(10));
		cout << "main" << endl;
		A r = ft.get(); // 찍히는게 없다.
		/* Result
		A
		func
		A
		main
		*/
		cout << "=====================" << endl;

		// future 객체 생성만했는데 "A"가 찍혔다. 내부적으로 A를 생성해놓는듯?
		//async(launch::deferred, func); 

		// 그리고 launch::deferred에 의해 함수 대기하므로 아무것도 찍히지 않는다.
		ft = async(launch::deferred, func);

		// sleep하지 않으면 main이 먼저 찍힌다.
		this_thread::sleep_for(chrono::milliseconds(10));
		cout << "main" << endl;
		r = ft.get(); // get()호출시 함수가 실행되므로 "func", "A"가 찍힌다.
		/* Result
		A
		main
		func
		A
		*/
	}

	cout << "std::thread에서 비동기로 함수 실행: std::promise, std::future" << endl;
	{
		/*
		std::promise, std::future를 사용하면 다른 thread에서 실행한 결과를 얻을 수 있다.
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
		thread t = thread(func, std::ref(pv)); // todo std::ref: pv를 rvalue로 만들어준다?
		//thread t = thread(func, std::ref(pv)); // func가 전역 함수인경우 &func로 전달해야한다.
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

	cout << "비동기 task" << endl;
	{
		/*
		VC10에서는 MS의 독자 병렬 프로그래밍 라이브러리인 Concurrency Runtime의 task 라이브러리를 사용하면 간단하게 비동기 프로그래밍을 할 수 있다.
		C++11에서도 std::package_task 클래스를 이용하면 Concurrency Runtime의 task와 비슷한 것을 만들 수 있다.

		package_task를 생성하면서 비동기로 실행할 함수를 넘기고 결과는 future를 통해서 받도록 한 후, 
		thread 라이브러리에 std::package_task 객체를 인자 값으로 넣어주면 된다.
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

		// vector iterator 사용법
		//auto r = func(1);
		//auto iter = r.begin();
		//while (iter != r.end())
		//{
		//	cout << (*iter++) << endl;
		//}
		//for (auto i : r) { // i는 iterator::value_type
		//	cout << i << endl;
		//}

		packaged_task<vector<int>(int)> task(func); // todo (int)는 뭘 뜻하지?
		future<vector<int>> ft = task.get_future();
		thread t = thread(move(task), 99); // todo move 클래스 뭐지?
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
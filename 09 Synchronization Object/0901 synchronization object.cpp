#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex> // mutex 사용
#include <condition_variable> // condition_variable 사용

using namespace std;

/*
여러 스레드에서 하나의 공유자원을 사용할 때 데이터 경합(data-race)이 일어난다.
타이밍에 의해 순서대로 사용할수도 그렇지 않을 수도 있다.

lock상태를 공유하고 싶은 스레드는 같은 mutex 객체를 사용해야한다.
lock 호출 시 다른 스레드가 lock한 상태이면 lock 호출한 곳에서 대기하고 있다가
다른 스레드가 unlock 호출하면 그때서야 lock 호출 다음 코드부터 실행된다.

mutex 사용 이유가 공유자원의 경합이 일어나지 않게하는 것이지만
공유자원의 사용여부와 관계없이 mutex.lock으로 다른 스레드의 실행을 막을 수 있다.

스코프를 빠져나가 mutex 객체가 파괴되기 전에 unlock하지 않으면 에러난다.
*/

int synchronization_object() {
	cout << "공유 자원을 잠그기: mutex.lock(), mutex.unlock()" << endl;
	{
		/*
		공유 자원을 사용할 때 lock을 걸어 다른 스레드가 접근하지 못하게 하고 다 사용한 후 unlock으로 해제한다.
		unlock하지 않으면 데드락에 빠지므로 반드시 unlock해줘야한다.
		또한 너무 잦은 lock/unlock은 스레드 대기 시간이 늘어나므로 처리시간이 길어진다.
		*/

		mutex m;
		cout << "main" << endl;

		m.lock();
		//m.lock(); // 하나의 스레드에서 lock을 두번 호출하면 에러난다.

		thread t = thread([&]() {
			m.lock(); // 메인 스레드에서 lock했기 때문에 unlock할 때까지 여기서 대기하게 된다.
			cout << "t" << endl;
			m.unlock(); // lock한 영역을 벗어나기 전에 unlock해야한다. 안그러면 객체 소멸 시 "mutex destroyed while busy" 런타임 에러가 나고
			// 블럭 밖에서 unlock을 호출해도 현재 스코프에대한 lock이 아니라서 unlock되지 않는다.
		});

		Sleep(1000);
		m.unlock();

		t.join();

		/* Result
		main t
		t // 0.5초 뒤
		*/
	}

	cout << "공유 자원을 잠그기: mutex.try_lock()" << endl;
	{
		/*
		다른 스레드가 lock을 걸어 작업할 수 없을 때 lock이 풀릴때까지 다른 일을 하고 싶은 경우 사용
		*/

		mutex m;
		cout << "main" << endl;

		m.lock();
		//m.lock(); // 하나의 스레드에서 lock을 두번 호출하면 에러난다.

		thread t = thread([&]() {
			while (true)
			{
				if (m.try_lock()) // lock을 걸수 있으면 lock을 걸고 true 반환
				{
					cout << "t" << endl; // lock이 걸렸으니 공유자원 처리
					m.unlock();
					break;
				}
				else {
					cout << "..." << endl; // lock을 못걸었으니 다른일 처리
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

	cout << "unlock하지 않는 실수 방지: lock_guard 클래스" << endl;
	{
		/*
		실수로 unlock하지 않아도 코드 블럭을 빠져나올 때 unlock을 호출해준다.
		mutex의 소유와 해제에 대한 RAII(리소스 확보 및 초기화) 패턴을 구현한 것이다.
		todo raii 찾아보기
		*/

		mutex m;
		cout << "main" << endl;

		m.lock();
		//m.lock(); // 하나의 스레드에서 lock을 두번 호출하면 에러난다.

		thread t = thread([&]() {
			lock_guard<mutex> lg(m); // lock_guard 객체 생성 시 m의 lock()호출
			cout << "t" << endl;
			// 그냥 빠져나가도 자동으로 unlock()이 호출된다.
		});

		Sleep(500);
		m.unlock();

		t.join();

		/* Result
		main t
		t // 0.5초 뒤
		*/
	}


	cout << "반복하여 lock 걸기: recursive_mutex 클래스" << endl;
	{
		/*
		unlock하지 않고 같은 스레드에서 lock을 다시 걸 경우 에러가 발생한다.
		recursive_mutex 객체는 여러번 lock거는 것을 허용한다.
		단, lock횟수만큼 unlock을 해줘야한다.
		*/

		recursive_mutex rm;

		rm.lock();
		rm.lock();
		rm.unlock();
		rm.unlock(); // lock 횟수와 unlock 횟수가 같지 않으면 프로그램(혹은 코드블럭) 종료 시 에러난다.
	}

	cout << "지정한 시간 동안만 lock 시도하기: timed_mutex.try_lock_for(), timed_mutex.try_lock_until(), recursive_timed_mutex" << endl;
	{
		/*
		특정 시간동안만 lock을 시도하고 시간내에 lock하지 못하면 그만둔다.
		mutex의 기능과 동일하고 try_lock_for(), try_lock_until()함수가 추가됐다.

		recursive_timed_mutex: recursive_mutex에 timed_mutex 기능이 추가된것.
		*/

		timed_mutex tm;
		tm.lock();
		cout << "main locked" << endl;

		thread t = thread([&]() {
			//if (tm.try_lock_for(chrono::milliseconds(110))) { // 지정시간동안 
			//if (tm.try_lock_until(chrono::system_clock::now() + chrono::milliseconds(10))) { // 지정시간까지
			if (tm.try_lock_until(chrono::system_clock::now() + chrono::milliseconds(10))) { // 지정시간까지
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
		// 100밀리초 이상
		main locked
		main unlocked
		t locked
		t end

		// 100밀리초 이하
		main locked
		t lock failed // main 스레드가 Sleep하는 동안 lock이 실패해서 다음 코드가 실행됐다.
		t end
		main unlocked
		*/
	}

	cout << "향상된 기능의 lock_guard: unique_lock 클래스" << endl;
	{
		/*
		lock_guard는 생성시에만 lock을 걸고 파괴시에만 unlock할 수 있다.

		unique_lock 객체 생성후에 lock/unlock을 할 수 있다.
		try_lock(), try_lock_for(), try_lock_until() 지원
		unique_lock끼리 mutex객체를 서로 교환 가능

		*/

		timed_mutex tm;

		//unique_lock<timed_mutex> ul1(tm, adopt_lock); // unique_lock 생성과 동시에 mutex의 소유권을 갖는다.
		//ul1.lock(); // 생성되며 lock됐기 때문에 lock()하면 에러. 그런데 실제로 lock되지 않는다.
		//tm.lock(); // 이렇게 mutex객체를 lock해줘야 lock이 된다. 왜인지 모르겠다.

		unique_lock<timed_mutex> ul1(tm, defer_lock); // unique_lock을 생성하지만 mutex의 소유권은 갖지 않는다.
		ul1.lock(); // 생성하며 lock이 되지 않기 때문에 필요시점에 lock해준다.

		cout << "main locked" << endl;

		thread t = thread([&]() {
			unique_lock<timed_mutex> ul2(tm, defer_lock);
			if (ul2.try_lock_for(chrono::milliseconds(1100))) {
				cout << "t locked" << endl;
				//uld.unlock(); // unlock하지 않아도 에러안난다.
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
		// 1초 이상
		main locked
		main unlocked
		t locked
		t end

		// 1초 이하
		main locked
		t lock failed // main 스레드가 Sleep하는 동안 lock이 실패해서 다음 코드가 실행됐다.
		t end
		main unlocked
		*/
	}

	cout << "한 번만 실행하기: std::call_once, std::once_flag" << endl;
	{
		/*
		멀티 스레드 환경에서 함수를 한번만 호출하게 한다.
		*/
		auto f = []() {
			cout << "once f" << endl;
		};

		once_flag once;
		for (size_t i = 0; i < 10; i++)
		{
			call_once(once, f); // once f 한 번만 출력된다.
		}
	}

	cout << "스레드마다 데이터 저장하기: thread_local" << endl;
	{
		/*
		각 스레드가 고유의 스택 메모리에 데이터를 저장하도록 한다.
		공유 자원이 없어지므로 오작동을 막을 수 있다.
		*/

		//int cnt = 0; // 숫자가 1이 된다.
		thread_local int cnt = 0; // 숫자가 0이다.
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

	cout << "이벤트를 통한 동기화 객체: thread_local" << endl;
	{
		/*
		스레드 간 특정 조건을 만족할 때까지 대기하고 있다가 이벤트를 보내어 대기를 풀어줄 때 사용한다.
		unique_lock_any는 unique_lock<mutex> 이외의 것도 사용할 수 있다.
		소스참고: https://en.cppreference.com/w/cpp/thread/condition_variable
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
			// defer_lock, adopt_lock을 설정하면 condition_variable이 작동하지 않는다.
			// 대기하고 있다가 lock을 걸어야해서 그런듯하다.
			unique_lock<mutex> u(m);
			cv.wait(u);
			//cv.wait(u, []() {cout << "wake up" << endl; return false; }); // return false;하면 wake up 되지 않는다.
			cout << "fchain: " << cnt++ << endl;

			// 작업이 왼료되고 notify를 호출하지 않으면 데드락이 걸릴 수 있다.
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
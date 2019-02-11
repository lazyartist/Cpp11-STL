#include <iostream>
#include <chrono> // chrono 사용

using namespace std;

/*
chrono는 C++에 새로 추가된 시간 라이브러리
기존 C런타임에서 제공하는 time 함수보다 다양한 기능을 제공하고 사용이 쉽고 정밀도가 높다.
time 함수는 초 단위만 측정, chrono는 나노밀리초 단위까지 측정 가능.

C++03에서는 초 보다 더 높은 정밀도로 시간을 측정해야할 경우
OS에서 제공하는 QueryPerformanceFrequency, QueryPerformanceCounter 등의 함수를 사용해야하지만
chrono를 사용하면 OS와 독립적으로 정밀도가 높은 시간을 측정 가능.

시간끼리 연산도 가능.

Windows에서는 고해상도가 아닌 시간은 GetTickCount()함수를 사용하는데 chrono로 대체가능하다.
*/

int main() {
	cout << "시간 경과 출력" << endl;
	{
		// time_point 타입으로 1970.1.1부터 지금까지 경과 시간을 알아온다.
		chrono::system_clock::time_point start = chrono::system_clock::now(); 
		for (size_t i = 0; i < 10000000; i++)
		{
			std::sqrt(123.456L);
		}
		chrono::system_clock::time_point end = chrono::system_clock::now();

		//auto dur = end - start;
		std::chrono::duration<__int64, ratio<1, 10000000>> dur = end - start;

		chrono::duration<double> defaultSec = end - start;
		chrono::nanoseconds nano = dur;
		chrono::microseconds micro = chrono::duration_cast<chrono::microseconds>(dur);
		chrono::milliseconds milli = chrono::duration_cast<chrono::milliseconds>(dur);
		chrono::seconds sec = chrono::duration_cast<chrono::seconds>(dur);
		chrono::minutes min = chrono::duration_cast<chrono::minutes>(dur);
		chrono::hours hour = chrono::duration_cast<chrono::hours>(dur);

		cout << defaultSec.count() << endl; // 0.334331
		cout << nano.count() << endl; // 334330700
		cout << micro.count() << endl; // 334330
		cout << milli.count() << endl; // 334
		cout << sec.count() << endl; // 0
		cout << min.count() << endl; // 0
		cout << hour.count() << endl; // 0
	}

	cout << "시간 연산" << endl;
	{
		// 시간 생성
		chrono::seconds sec(1);
		chrono::milliseconds milli(1);

		cout << sec.count() << endl; // 1 
		cout << milli.count() << endl; // 1

		// 시간 형 변환
		cout << chrono::duration_cast<chrono::milliseconds>(sec).count() << endl; // 1000

		// 시간 연산
		chrono::milliseconds m2 = sec + milli;
		cout << m2.count() << endl; // 1001

		// 변수형이 seconds기 때문에 밀리초 부분을 잃어버리므로 컴파일 에러나므로 
		// 명시적 형 변환 시켜준다.(명시적 형변환해도 밀리초 부분은 잃어버린다.)
		chrono::seconds s2 = chrono::duration_cast<chrono::seconds>(sec + milli); 
		cout << s2.count() << endl; // 1
	}

	cout << "system_clock, steady_clock, high_resolution_clock 클래스" << endl;
	{
		/*
		system_clock은 시스템 시간을 표현하기 위한 시간
		time_t와 호환됨
		멤버함수 : to_time_t(), from_time_t()
		*/
		chrono::system_clock::time_point now = chrono::system_clock::now(); // 15498745859352777
		time_t t = chrono::system_clock::to_time_t(now);
		cout << t << endl; // 1549874585, time_point에서 밀리초가 제거된다.

		chrono::system_clock::time_point now2 = chrono::system_clock::from_time_t(t); // 15498750620000000, time_t로 갔다오니 밀리초부분 데이터가 없어졌다.

		/*
		steady_clock은 time_point를 얻은 후 OS의 시간을 뒤로 돌려도 원래 시간의 흐름에서 시간을 반환한다.
		*/
		chrono::steady_clock::time_point snow = chrono::steady_clock::now(); // 355085140206966, system_clock의 값과 차이가 큰데 왜인지는 모르겠다.

		/*
		high_resolution_clock 플랫폼에서 제공하는 정밀도가 가장 높은 시간
		system_clock이나 steady_clock의 다른 별칭으로 정의되기도 한다.
		*/
		chrono::high_resolution_clock::time_point hnow = chrono::high_resolution_clock::now(); // 355400732487804, steady_clock의 값과 거의 동일하다.
	}

	return 0;
}
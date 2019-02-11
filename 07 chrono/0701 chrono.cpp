#include <iostream>
#include <chrono> // chrono ���

using namespace std;

/*
chrono�� C++�� ���� �߰��� �ð� ���̺귯��
���� C��Ÿ�ӿ��� �����ϴ� time �Լ����� �پ��� ����� �����ϰ� ����� ���� ���е��� ����.
time �Լ��� �� ������ ����, chrono�� ����и��� �������� ���� ����.

C++03������ �� ���� �� ���� ���е��� �ð��� �����ؾ��� ���
OS���� �����ϴ� QueryPerformanceFrequency, QueryPerformanceCounter ���� �Լ��� ����ؾ�������
chrono�� ����ϸ� OS�� ���������� ���е��� ���� �ð��� ���� ����.

�ð����� ���굵 ����.

Windows������ ���ػ󵵰� �ƴ� �ð��� GetTickCount()�Լ��� ����ϴµ� chrono�� ��ü�����ϴ�.
*/

int main() {
	cout << "�ð� ��� ���" << endl;
	{
		// time_point Ÿ������ 1970.1.1���� ���ݱ��� ��� �ð��� �˾ƿ´�.
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

	cout << "�ð� ����" << endl;
	{
		// �ð� ����
		chrono::seconds sec(1);
		chrono::milliseconds milli(1);

		cout << sec.count() << endl; // 1 
		cout << milli.count() << endl; // 1

		// �ð� �� ��ȯ
		cout << chrono::duration_cast<chrono::milliseconds>(sec).count() << endl; // 1000

		// �ð� ����
		chrono::milliseconds m2 = sec + milli;
		cout << m2.count() << endl; // 1001

		// �������� seconds�� ������ �и��� �κ��� �Ҿ�����Ƿ� ������ �������Ƿ� 
		// ����� �� ��ȯ �����ش�.(����� ����ȯ�ص� �и��� �κ��� �Ҿ������.)
		chrono::seconds s2 = chrono::duration_cast<chrono::seconds>(sec + milli); 
		cout << s2.count() << endl; // 1
	}

	cout << "system_clock, steady_clock, high_resolution_clock Ŭ����" << endl;
	{
		/*
		system_clock�� �ý��� �ð��� ǥ���ϱ� ���� �ð�
		time_t�� ȣȯ��
		����Լ� : to_time_t(), from_time_t()
		*/
		chrono::system_clock::time_point now = chrono::system_clock::now(); // 15498745859352777
		time_t t = chrono::system_clock::to_time_t(now);
		cout << t << endl; // 1549874585, time_point���� �и��ʰ� ���ŵȴ�.

		chrono::system_clock::time_point now2 = chrono::system_clock::from_time_t(t); // 15498750620000000, time_t�� ���ٿ��� �и��ʺκ� �����Ͱ� ��������.

		/*
		steady_clock�� time_point�� ���� �� OS�� �ð��� �ڷ� ������ ���� �ð��� �帧���� �ð��� ��ȯ�Ѵ�.
		*/
		chrono::steady_clock::time_point snow = chrono::steady_clock::now(); // 355085140206966, system_clock�� ���� ���̰� ū�� �������� �𸣰ڴ�.

		/*
		high_resolution_clock �÷������� �����ϴ� ���е��� ���� ���� �ð�
		system_clock�̳� steady_clock�� �ٸ� ��Ī���� ���ǵǱ⵵ �Ѵ�.
		*/
		chrono::high_resolution_clock::time_point hnow = chrono::high_resolution_clock::now(); // 355400732487804, steady_clock�� ���� ���� �����ϴ�.
	}

	return 0;
}
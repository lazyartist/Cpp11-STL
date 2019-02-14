#include <iostream>
#include <vector>
#include <algorithm> // find_if

using namespace std;

int main() {
	cout << "������ Ÿ�� �� Ÿ���� ���ϴ� auto" << endl;
	{
		/*
		auto�� ���� ���� �� ��������� Ÿ���� ������ �ʾƵ� �ʱ�ȭ ���� ���� ������ Ÿ�ӿ� �ڵ����� Ÿ���� �����ش�.
		auto�� ���� �������� ��� ����
		������, ���� Ÿ�Ե� ��� ����
		Iterator ��� �� ���ϴ�
		*/
		auto i = 0;
		auto* ii = &i;
		auto& iii = i;
		i = 1;

		cout << i << endl; // 1
		cout << (*ii) << endl; // 1
		cout << iii << endl; // 1

		vector<vector<int>> v;
		vector<vector<int>>::iterator iter = v.begin();
		auto aiter = v.begin(); // �̷��� ����������.
	}

	cout << "�̸����� �Լ� ����" << endl;
	{
		// []: lamda capture, (): parameter, {}: body
		auto f1 = []() {}; // todo auto �Ⱦ��� ���� ���� ����?
		f1(); // function call

		// ��ȯ�� ����� ����
		auto f2 = []() -> int {return 0; };
		f2();

		// find_if �˰��� ���� ����ϱ�
		vector<int> v = { 1, 2, 3 };
		vector<int>::iterator iter = find_if(v.begin(), v.end(), [](int& i) {
			return i == 2;
		});
		cout << *iter << endl; // 2

		// �ܺ� ���� ĸó
		int param = 0;
		auto f3 =
			[
				&iter/*iter ����*/,
				&param/*param ����*/
				/*&: ��纯�� ����, =: ��纯�� ����(&�� �Բ� ��������)*/
			]() {
			cout << *(++iter) << endl; // 3
			cout << param << endl; // 0
		};
		f3();
	}

	cout << "range base for" << endl;
	{
		vector<int> v = { 1, 2, 3 };

		// �Ϲ� for
		for (size_t i = 0; i < v.size(); i++) {}

		// VC++ Ưȭ for
		//for each (int var in v){} // ������ �ȵ�

		// range base for
		// for(for-range-declaration : expression) statement
		for (auto& const i : v) {} // �ӽú����� ����� ����� ���̰� ���� �������� ���ϰ� �Ϸ��� &�� const�� ����Ѵ�.
	}

	cout << "unscoped enumeration, scoped enumeration" << endl;
	{
		// unscoped
		enum Unscoped : short { Unscoped1 = 1 }; // short�� enum�� ũ�⸦ �����Ѵ�. ���ϸ� int
		Unscoped1;

		// scoped
		enum class Scoped { Scoped1 = 2 }; // class ��� struct ����
		//Scoped1; // ������ ����
		Scoped::Scoped1;

		cout << Unscoped1 << endl; // 1, �Ϲ������� ������ ��ȯ
		cout << static_cast<int>(Scoped::Scoped1) << endl; // 2, ��������� ������ ��ȯ
	}

	cout << "nullptr" << endl;
	{
		/*
		NULL ��ũ�� ���, 0�� ���ڷ� �ѱ�� int Ÿ������ �߷��ϴ� ������ �ִ�.
		*/
		void f(int i);
		void f(double* i);

		f(static_cast<double*>(0)); // void f(double* i); ȣ��
		f(0); // void f(int i); ȣ��
		f(NULL); // void f(int i); ȣ��

		f(nullptr); // void f(double* i); ȣ��

		cout << sizeof(nullptr) << endl; // 4
		cout << typeid(nullptr).name() << endl; // std::nullptr_t
		//throw(nullptr); // ���� �߻�

		// ������ ��� ����
		/*
		int n = nullptr;
		int n = 0;
		if (n == nullptr) {}
		if (nullptr) {}
		if (nullptr == 0) {}
		nullptr = 0;
		nullptr + 2;
		*/
	}

	return 0;
}

void f(int i) { cout << i << endl; }
void f(double* i) { cout << i << endl; }
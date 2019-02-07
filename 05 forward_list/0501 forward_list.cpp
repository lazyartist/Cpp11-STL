#include <iostream>
#include <forward_list> // forward_list ���

using namespace std;

/* forward_list?
forward_list�� ���� ����
	������ �ִ� std::list�� ����� ��ũ�� ����Ʈ�̴�
	�ܹ��⸸ ����� ���� �޸� ��뷮�� �ӵ��� ��ȿ�����̴�.
	���� C++11���� �ܹ��� ����Ʈ�� forward_list�� �߰���

forward_list�� ���� ��ħ
	������ �ֿ켱�Ѵ�.
	std::list�� insert, erase�� ���� ���⼺ ������ ���� �϶��� �������� �������� �ʴ´�.
	���ʿ��� �޸� ����� ���� ���� size() �Լ��� �������� �ʴ´�.

std::list�� �ٸ� ��
	forward_list�� �ܹ����̹Ƿ� �����͸� �ϳ��� ���´�.
	���� �� ��Ҹ��� ������ �ϳ� �������� 4byte(32bit) or 8byte(64bit)�� �Ƴ� �� �ִ�.
	forward_list�� �� �������θ� �̵��Ѵ�.
*/

int main() {
	cout << "std::forward_list ����" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };

		for (auto v : fl)
		{
			cout << v << endl; // 1, 2, 3
		}
	}

	cout << "����Ʈ �տ� ������ �߰� : push_front()" << endl;
	{
		forward_list<int> fl;
		fl.push_front(0);
		fl.push_front(1);

		for (auto v : fl)
		{
			cout << v << endl; // 1, 0
		}
	}

	cout << "Ư�� ��ġ �ڿ� ������ �߰� : insert_after()" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.insert_after(fl.begin(), 9);

		for (auto v : fl)
		{
			cout << v << endl; // 1, 9, 2, 3
		}
	}

	cout << "�ӽð�ü�� ����/�������� �ʰ� Ư�� ��ġ �ڿ� ������ �߰� : emplace_after(), emplace_front()" << endl;
	{
		class A {
		public:
			A(int v) { this->v = v; }
			int v;
		};

		forward_list<A> fl = { 1, 2, 3 };
		fl.insert_after(fl.begin(), A(9)); // A��ü�� '����'�ϰ� �μ��� �ѱ�� '����'�ȴ�.

		// C++11���� ���� emplace_�� �����ϴ� �Լ��� "Placement Insert"�� �Ѵ�.
		// �߰��� ��ü�� �����ؼ� �ѱ��� �ʰ� �������� �μ����� �Ѱ��ֱ� ������ �μ����� �� �߻��ϴ� ���� ������ ����.
		fl.emplace_after(fl.begin(), 99); // �μ������� forward_list ���ο��� A��ü�� �ٷ� '����'�Ѵ�. '����' ������ ����.
		fl.emplace_front(999); // emplace_after()�� ���� ��������� ���� �տ� �����Ѵ�.

		for (auto v : fl)
		{
			cout << v.v << endl; // 999, 1, 99, 9, 2, 3
		}
	}

	cout << "ù ��° ������ ���� : pop_front()" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.pop_front();
		for (auto v : fl)
		{
			cout << v << endl; // 2, 3
		}
	}

	cout << "Ư�� ��ġ ���� ������ ���� : erase_after()" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.erase_after(fl.begin());
		for (auto v : fl)
		{
			cout << v << endl; // 1, 3
		}
	}

	cout << "���� �����Ͽ� ������ ���� : erase_after(��������)" << endl;
	{
		forward_list<int> fl = { 1, 2, 3 };
		fl.erase_after(fl.begin(), fl.end()); // ù �Ķ� �ں��� ~ �� ��° �Ķ�����
		for (auto v : fl)
		{
			cout << v << endl; // 1
		}
	}

	cout << "�����ϱ� : sort(�����Լ�)" << endl;
	{
		class A {
		public:
			A(int v) { this->v = v; }
			int v;
		};

		forward_list<A> fl = { 1, 2, 3 };
		fl.sort([](A& l, A& r) -> bool {return l.v > r.v; }); // �����Լ� ���� ���� �� ���縦 �������� A&�� �Ѵ�.

		for (auto& v : fl) // ���縦 �������� auto&�� �Ѵ�.
		{
			cout << v.v << endl; // 3, 2, 1
		}
	}

	cout << "�ߺ� ����(������ �������� �ߺ��� ���� ����) : unique()" << endl;
	{
		// ���ĵ� ���
		forward_list<int> fl = { 3, 3, 1, 3, 3, 2, 3, 3 };
		fl.sort();
		fl.unique();
		for (auto v : fl)
		{
			cout << v << endl; // 1, 2, 3
		}

		cout << "---" << endl;

		// ���� �ȵ� ���
		fl = { 3, 3, 1, 3, 3, 2, 3, 3 };
		fl.unique();
		for (auto v : fl)
		{
			cout << v << endl; // 3, 1, 3, 2, 3
		}
	}

	cout << "�� forward_list ��ġ�� : merge()" << endl;
	{
		forward_list<int> fl1 = { 1, 2, 3 };
		forward_list<int> fl2 = { 11, 22, 33 };
		fl1.merge(fl2);
		for (auto v : fl1)
		{
			cout << v << endl; // 1, 2, 3, 11, 22, 33
		}
	}

	cout << "�� forward_list �����Ͽ� ��ġ�� : merge()" << endl;
	{
		forward_list<int> fl1 = { 1, 2, 3 };
		forward_list<int> fl2 = { 11, 22, 33 };

		// todo : �����Լ��� �����ϴ� ��������. ������ �𸣰ڴ�.
		//fl1.merge(fl2);
		//fl1.merge(fl2, std::greater<int>());
		//fl1.merge(fl2, [](auto l, auto r) -> bool {return l > r; });
		//fl1.merge(fl2, [](int& l, int& r) -> bool {return l > r; });
		//fl.sort([](A& l, A& r) -> bool {return l.v > r.v; }); // �����Լ� ���� ���� �� ���縦 �������� A&�� �Ѵ�.
		//for (auto v : fl1)
		//{
		//	cout << v << endl; // 1, 2, 3, 11, 22, 33
		//}
	}

	cout << "�� forward_list ���� �����Ͽ� ��ġ�� : splice_after()" << endl;
	{
		forward_list<int> fl1 = { 1, 2, 3 };
		forward_list<int> fl2 = { 11, 22, 33 };
		fl1.splice_after(fl1.begin(), fl2);
		for (auto v : fl1)
		{
			cout << v << endl; // 1, 11, 22, 33, 2, 3
		}

		cout << "---" << endl;

		fl1 = { 1, 2, 3 };
		fl2 = { 11, 22, 33 };
		fl1.splice_after(fl1.begin(), fl2, fl2.begin(), fl2.end());
		for (auto v : fl1)
		{
			cout << v << endl; // 1, 22, 33, 2, 3
		}
	}

	return 0;
}
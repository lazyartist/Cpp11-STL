#include <iostream>
#include <memory> // shared_ptr, unique_ptr ���
#include <vector>

using namespace std;

/*
�Ҵ��� �޸𸮸� �ڵ����� �������ִ� ���� '����Ʈ ������'��� �ϰ� shared_ptr, unique_ptr ���� ���Ѵ�.
C++03�� auto_ptr�� ������ ����� ��ٷο� ���� ������� �ʾҴ�.

���ɻ� ���ذ� ���� �����鼭 �޸� ������ ���Ǽ��� ���� �� �ִ�.
*/

int main() {
	cout << "shared_ptr" << endl;
	{
		class A {};

		shared_ptr<int> sp(new int(9));
		cout << sp.use_count() << endl; // 1, ���� ī��Ʈ
		{
			shared_ptr<int> sp2 = sp;
			cout << sp.use_count() << endl; // 2, ���� ī��Ʈ
		}
		cout << sp.use_count() << endl; // 1, ���� ī��Ʈ

		cout << (sp.get()) << endl; // 001D8A28, ���� ��ü�� ������ ���
		cout << (*sp) << endl; // 9, ���� ��ü�� ���� ���
		//cout << (sp->...) << endl; // 1, ���� ��ü�� ����� ����
	}

	cout << "shared_ptr ���� ��ü ���� �� �ٸ� ��ü�� �ٲٱ� : reset()" << endl;
	{
		shared_ptr<int> sp(new int(9));
		int* i = sp.get();
		cout << *i << endl; // 9

		sp.reset(new int(1));
		cout << *i << endl; // -572662307, reset �� �����Ⱚ, ���� ��ü �ı�
		i = sp.get();

		sp.reset();
		cout << *i << endl; // -572662307, reset �� �����Ⱚ, ���� ��ü �ı�
		cout << (sp.get()) << endl; // 00000000, reset()�� �� ���� ���� �ʾƼ� ���� ��ü�� �ı���
	}

	cout << "�� shared_ptr ���� ��ü ��ȯ�ϱ�" << endl;
	{
		shared_ptr<int> sp1(new int(1));
		shared_ptr<int> sp2(new int(2));

		sp1.swap(sp2);

		cout << *sp1 << endl; // 2
		cout << *sp2 << endl; // 1
	}

	cout << "shared_ptr �迭 ��ü �ٷ�� �� ������ ����" << endl;
	{
		/*
		shared_ptr�� ������ �� �����ڸ� ����Ͽ� �迭���� �˷��ش�.
		*/
		class A {
		public:
			~A() { cout << "~A" << endl; }
		};

		{
			shared_ptr<A[]> sp(new A[2], default_delete<A[]>()); // �⺻ ������ ����
			// sp��ü�� �ı��Ǹ� A��ü�鵵 �ı��ȴ�. ~A, ~A �� �� ��µ�

			//(*sp); // todo �̰� �� �ȵɱ�?
		}

		{
			shared_ptr<A[]> sp(new A[2], [](A* a) { // ���ٽ����� ����� ���� ������ ����
				delete[] a;
			});
			// sp��ü�� �ı��Ǹ� A��ü�鵵 �ı��ȴ�. ~A, ~A �� �� ��µ�
		}
	}

	cout << "�����ϰ� shared_ptr �����" << endl;
	{
		auto sp = make_shared<int>(1);
		cout << typeid(sp).name() << endl; // class std::shared_ptr<int>
	}

	cout << "shared_ptr ��ȯ���� �߻�" << endl;
	{
		class A {
		public:
			~A(){cout << "~A" << endl;}
			shared_ptr<A> other;
		};

		shared_ptr<A> sp1(new A());
		//auto sp1 = make_shared<A>(new A()); // todo �� �̰� ������ ��������?

		{
			shared_ptr<A> sp2(new A());
			//auto sp2 = make_shared<A>(new A());
			sp1->other = sp2;
			sp2->other = sp1;
		}
		// ��ȯ������ �߻��Ͽ� ~A, ~A�� ��µ��� �ʴ´�.
	}

	cout << "unique_ptr" << endl;
	{
		/*
		shared_ptr���� �ٸ��� ��ü�� ���������� �����Ѵ�.
		unique_ptr�� ��Ģ������ ���� �����ڿ� �Ҵ� �����ڰ� �����Ǿ����� �ʴ�. ��, ���縦 �� �� ����. ���� �̵�(move)�� �����ϴ�.

		������ shared_ptr�� ���� �����ϰ� use_count()�� ����, release()�� �߰��ƴ�.

		�迭��ü�� �����ڸ� �������� ���ص� �ȴ�.
		������ �����Ϸ��� ���ø� ���ڿ� Ÿ���� ����������ϴ� ���� shared_ptr�� �ٸ���.
		*/

		unique_ptr<int> ut1(new int(1));
		cout << ut1.get() << endl; // 012C4A20, �����ϴ� ��ü�� �ִ�.

		//unique_ptr<int> ut2 = ut1; // ���� �����ڰ� ���� ������ ������ ����.
		unique_ptr<int> ut2 = std::move(ut1); // move�� �̿��� �ٸ� ��ü�� �Ҵ�

		cout << ut1.get() << endl; // 00000000, move�����Ƿ� �����ϴ� ��ü�� ����.
		cout << ut2.get() << endl; // 012C4A20, ut1�� �����ϴ� ��ü�� �����Ѵ�.

		ut2.release();
		cout << ut2.get() << endl; // 00000000, �����ϴ� ��ü�� �����ߴ�.

		// ������ �����ϱ�
		void deleteArray(int* arr);
		unique_ptr<int[], decltype(&deleteArray)> arrut(new int[3], deleteArray);
	}

	return 0;
}

void deleteArray(int* arr) { delete[] arr; }

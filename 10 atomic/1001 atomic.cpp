#include <iostream>
#include <atomic> // atomic ���

using namespace std;

/*
mutex�� ������� �ʰ� �⺻ Ÿ���� ���� �ڿ� ����ȭ�� �� �ִ�.
lock free�� �����ϱ� ������ ����ȭ ��ü�� ���� ������带 ���� �� �ִ�.

��ũ�θ� ����Ͽ� Ȯ���ϰ� ����ؾ��Ѵ�.

Win32 api�� atomic�� ������ ����� ���� ���� interlocked_XXX �̴�.

// todo dead lock ���� �� ���� �ٽ� �ۼ� https://ozt88.tistory.com/38
*/

// # ���ڿ��� ��ȯ, ## ��ū ����
#define IsLockFree(lockFreeType) cout << #lockFreeType; checkLockFree(lockFreeType);

void checkLockFree(int lockFreeType) {
	if (lockFreeType == 2) {
		cout << "" << ": ok" << endl;
	}
	else {
		cout << "" << ": no - " << lockFreeType << endl;
	}
}

int main() {
	cout << "�� ���� ���� ����" << endl;
	{
		IsLockFree(ATOMIC_BOOL_LOCK_FREE);
		IsLockFree(ATOMIC_CHAR_LOCK_FREE);
		IsLockFree(ATOMIC_CHAR16_T_LOCK_FREE);
		IsLockFree(ATOMIC_CHAR32_T_LOCK_FREE);
		IsLockFree(ATOMIC_WCHAR_T_LOCK_FREE);
		IsLockFree(ATOMIC_SHORT_LOCK_FREE);
		IsLockFree(ATOMIC_INT_LOCK_FREE);
		IsLockFree(ATOMIC_LONG_LOCK_FREE);
		IsLockFree(ATOMIC_LLONG_LOCK_FREE);
		IsLockFree(ATOMIC_POINTER_LOCK_FREE);
	}

	cout << "atomic ��ü �ʱ�ȭ, �а� ����" << endl;
	{
		atomic<int> a(0); // ����
		cout << a.load() << endl; // 0, �б�

		a.store(1); // �ʱ�ȭ, or atomic_store(&a, 1);
		cout << a.load() << endl; // 1

		int b = a; // �Ϲ��� �б�
		a = 9; // �Ϲ��� ����
		cout << a << endl; // 9
		cout << b << endl; // 1
	}

	cout << "�� ���� ����" << endl;
	{
		atomic<int> a(0);

		int b = atomic_fetch_add(&a, 1); // ��� �� ���� ��ȯ
		cout << a << endl; // 1

		b = atomic_fetch_sub(&a, 1);
		cout << a << endl; // 0
		
		b = atomic_fetch_and(&a, 1);
		cout << a << endl; // 0

		b = atomic_fetch_or(&a, 1);
		cout << a << endl; // 1

		b = atomic_fetch_xor(&a, 1);
		cout << a << endl; // 0
	}

	cout << "�ٲٱ�, �� �� �ٲٱ�" << endl;
	{
		atomic<int> a(0);

		int b = atomic_exchange(&a, 1); // a�� ���� 1�� �ٲٰ� �ٲٱ� ���� ���� ��ȯ
		cout << a << ", " << b << endl; // 1, 0

		// atomic_compare_exchange_weak CAS ������ ������ ���ɼ��� �־ �������� ������ ������ ����.
		int e = 1; // expected
		bool r = atomic_compare_exchange_weak(&a, &e, 9); // a�� ���� e�� ������ a�� 9�� ��ȯ
		cout << a << ", " << r << endl; // 9, 1

		// atomic_compare_exchange_weak CAS ������ ������ ���ɼ��� ��� �������� ������ ������ ����.
		e = 9; // expected
		r = atomic_compare_exchange_strong(&a, &e, 9); // a�� ���� e�� ������ a�� 9�� ��ȯ
		cout << a << ", " << r << endl; // 9, 1
	}

	return 0;
}
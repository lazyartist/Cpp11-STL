#include <iostream>
#include <atomic> // atomic 사용

using namespace std;

/*
mutex를 사용하지 않고 기본 타입을 공유 자원 동기화할 수 있다.
lock free로 동작하기 때문에 동기화 객체로 인한 오버헤드를 막을 수 있다.

매크로를 사용하여 확인하고 사용해야한다.

Win32 api에 atomic과 동일한 기능을 가진 것이 interlocked_XXX 이다.

// todo dead lock 숙지 후 예제 다시 작성 https://ozt88.tistory.com/38
*/

// # 문자열로 변환, ## 토큰 결합
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
	cout << "락 프리 여부 조사" << endl;
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

	cout << "atomic 객체 초기화, 읽고 쓰기" << endl;
	{
		atomic<int> a(0); // 생성
		cout << a.load() << endl; // 0, 읽기

		a.store(1); // 초기화, or atomic_store(&a, 1);
		cout << a.load() << endl; // 1

		int b = a; // 암묵적 읽기
		a = 9; // 암묵적 쓰기
		cout << a << endl; // 9
		cout << b << endl; // 1
	}

	cout << "락 프리 연산" << endl;
	{
		atomic<int> a(0);

		int b = atomic_fetch_add(&a, 1); // 계산 전 값을 반환
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

	cout << "바꾸기, 비교 후 바꾸기" << endl;
	{
		atomic<int> a(0);

		int b = atomic_exchange(&a, 1); // a의 값을 1로 바꾸고 바꾸기 전의 값을 반환
		cout << a << ", " << b << endl; // 1, 0

		// atomic_compare_exchange_weak CAS 조작이 실패할 가능성이 있어서 안정성이 낮지만 성능이 높다.
		int e = 1; // expected
		bool r = atomic_compare_exchange_weak(&a, &e, 9); // a의 값이 e와 같으면 a를 9로 교환
		cout << a << ", " << r << endl; // 9, 1

		// atomic_compare_exchange_weak CAS 조작이 실패할 가능성이 없어서 안정성이 높지만 성능이 낮다.
		e = 9; // expected
		r = atomic_compare_exchange_strong(&a, &e, 9); // a의 값이 e와 같으면 a를 9로 교환
		cout << a << ", " << r << endl; // 9, 1
	}

	return 0;
}
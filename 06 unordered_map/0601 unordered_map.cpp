#include <iostream>
#include <unordered_map> // unordered_map 사용
#include <string>

using namespace std;

/*
unordered_map은 C++11 이전에 있던 hash_map과 거의 동일
hash_map은 표준 컨테이너는 아님
*/

int main() {
	class A {
	public:
		A() {} // operator[]로 검색 시 값이 없으면 기본값을 추가해서 반환하기 때문에 umordered_map에 추가할 클래스는 기본 생성자가 반드시 있어야한다.
		A(int v) {
			this->v = v;
		}
		A(string k1, string k2) {
			this->k1 = k1;
			this->k2 = k2;
		}
		int v;
		string k1;
		string k2;
	};

	cout << "unordered_map 생성 및 요소 추가 : insert(), pair(), operator[]" << endl;
	{
		unordered_map<int, string> um;

		// 기본
		um.insert(unordered_map<int, string>::value_type(1, "a"));

		// std::pair()
		um.insert(pair<int, string>(2, "b"));

		// operator[]
		//um[1] = "b"; // 같은 키가 이미 있으면 덮어쓴다.
		um[3] = "c";

		for (auto v : um) {
			// string을 include하지 않았으면 c_str()로 char 배열을 꺼내서 출력, string을 include했으면 << 연산자가 오버로딩 되어있으므로  v.second 바로 사용가능.
			cout << v.first << ": " << v.second.c_str() << endl; // 1: a, 2: b, 3: c
		}
	}

	cout << "순회, 요소 수 : size()" << endl;
	{
		unordered_map<int, A> um;
		um.insert(unordered_map<int, A>::value_type(1, A(10)));
		um.insert(unordered_map<int, A>::value_type(2, A(20)));
		um.insert(unordered_map<int, A>::value_type(3, A(30)));

		for (auto v : um) {
			cout << v.first << ": " << v.second.v << endl; // 1: 10, 2: 20, 3: 30
		}

		for (unordered_map<int, A>::iterator iter = um.begin(); iter != um.end(); ++iter) {
			cout << iter->first << ": " << iter->second.v << endl; // 1: 10, 2: 20, 3: 30
		}

		cout << um.size() << endl; // 3
	}

	cout << "요소 찾기, 삭제 : find(), operator[], at(), erase()" << endl;
	{
		unordered_map<int, A> um;
		um[1] = A(10);
		um[2] = A(20);
		um[3] = A(30);

		cout << "요소 찾기: find()" << endl;
		auto iter = um.find(2); // 요소 찾기
		cout << iter->first << ": " << iter->second.v << endl; // 2: 20

		cout << "못찾으면 end()가 반환됨" << endl;
		auto iter_notFound = um.find(4);
		cout << (iter_notFound == um.end()) << endl; // 1: 키 4가 없어서 못 찾음

		cout << "요소 찾기: operator[]" << endl;
		cout << um[1].v << endl; // 10
		cout << um[9].v << endl; // -842150451(쓰레기값): 요소를 못 찾으면 기본값을 추가해서 반환하기 때문에 A클래스에 기본 생성자를 명시적으로 생성해줘야한다.

		cout << "요소 찾기: at()" << endl;
		cout << um.at(1).v << endl;
		try
		{
			cout << um.at(99).v << endl; // 없는 키 99 찾아서 에러남. "invalid unordered_map<K, T> key"
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
		}

		cout << "요소 찾기: at() - count()로 at() 안전하게 사용하기" << endl;
		if (um.count(99)) {
			cout << um.at(99).v << endl; // 
		}

		cout << "요소 삭제: erase()" << endl;
		um.erase(2);
		for (auto iter : um)
		{
			cout << iter.first << ": " << iter.second.v << endl; // 1: 10, 3: 30
		}

		cout << "모든 요소 삭제: erase()" << endl;
		um.erase(um.begin(), um.end());
		for (auto iter : um)
		{
			cout << iter.second.v << endl; // 
		}

		cout << (um.empty()) << endl; // 1: 비어있음.
	}


	cout << "검색하고 iterator 반환: lower_bound(), upper_bound()" << endl;
	{
		unordered_map<int, string> um;
		um[2] = "b";
		um[1] = "a";
		um[9] = "i";

		for (auto iter : um)
		{
			// HashKey 생성 알고리즘에 의해 추가된 순서와 상관없이 저장된다.
			cout << iter.first << ": " << iter.second << endl; // 2: b, 9: i, 1 : a
		}

		auto iter = um.lower_bound(1); // 검색된 위치 반환
		if (iter != um.end()) { // 검색된 값이 없으면 end(쓰레기값)를 반환하기 때문에 꼭 검사해준다.
			cout << iter->first << ": " << iter->second << endl; // 1: a
		}

		iter = um.upper_bound(1);
		if (iter != um.end()) { // 검색된 값이 없거나 검색된 값이 끝 요소인 경우 end(쓰레기값)를 반환하기 때문에 꼭 검사해준다.
			cout << iter->first << ": " << iter->second << endl; // 1이 끝 요소이기 때문에 값이 없다.
		}
	}

	cout << "검색하고 없으면 추가하고 반환: emplace_hint()" << endl;
	{
		unordered_map<int, string> um;
		um[1] = "a";

		// 힌트 직전의 위치에 최대한 가깝게 컨테이너에 새 요소를 삽입합니다. 요소가 제자리에서 생성됩니다. 즉, 복사 또는 이동 작업이 수행되지 않습니다.
		auto iter = um.emplace_hint(um.begin(), 2, "b");
		cout << iter->first << ": " << iter->second << endl; // 2: b

		for (auto iter : um)
		{
			cout << iter.first << ": " << iter.second << endl; // 1: a, 2: b
		}
	}

	cout << "클래스(구조체)를 key로 사용하기" << endl;
	{
		/* std::hash
		https://en.cppreference.com/w/cpp/utility/hash
		https://github.com/atgchan/2015_Jagual2/wiki/std::hash-%EC%A1%B0%EC%82%AC

		hash 템플릿은 hash function을 구현하는 함수 오브젝트를 정의한다.
		hash 템플릿은 DefaultConstructible, CopyAssignable, Swappable, Destructible 성질을 가지고 있다.
		이 함수로 인해 생성된 인스턴스는 Hash 구조를 만족한다. 조금 더 구체적으로 이야기하면, 다음을 만족하는 operator()를 정의한다.

		1. Key 타입의 단일 파라미터를 받는다.
		2. size_t 크기의 값을 리턴하고 이 값은 해당 파라미터에 대한 hash value 이다.
		3. 호출 시 exception을 부르지 않는다.
		4. 동일한 두 개의 파라미터 k1, k2에 대해서 std::hash()(k1) == std::hash()(k2)를 만족한다.
		5. 서로 다른 두 개의 파라미터 k1, k2에 대해서 std::hash()(k1) == std::hash()(k2)를 만족할 확률은 매우 작다. 1.0/std::numeric_limits<size_t>::max()에 수렴한다.

		실제의 hash function들은 독립적으로 구현되어있으며 위에서 언급한 것 외의 어떤 성질도 만족할 필요가 없다.
		특히 몇 몇 구현들은 trivial([ˈtrɪviəl] 사소한, 하찮은) hash functios(데이터를 (거의)그대로 해시값으로 사용)들을 사용하여 integer를 그대로 맵핑한다.
		다시 말하면, 예를 들어 이러한 hash functions들은 정렬되지 않은 연합 컨테이너들과 동작하지만 암호화 된 해시들처럼 동작하지는 않는다.

		Hash functions 들은 단일 프로그램 실행에 대해서만 같은 input이 있을 때 같은 result가 나오도록 동작한다.
		이러한 동작은 Dos 공격으로부터의 위험을 막아준다.

		C string 전용은 없다. std::hash<const char*> 는 포인터의 값(메로리 주소)의 해시를 생성한다. character array를 읽어서 생성하지 않는다.
		*/

		// 해시 함수 오브젝트 정의: 인수 객체로 해시코드(값)을 반환한다.
		struct KeyHash {
			std::size_t operator()(const A& k) const { // const 지정하지 않으면 에러: 'const _Hasher' 형식의 식에서 'size_t main::KeyHash::operator ()(const main::A &)'을(를) 호출하면 const - volatile 한정자가 손실됩니다.
				//auto a = hash<string>()(k.k1);
				//auto b = hash<string>()(k.k2);
				//auto c = hash<string>()(k.k2) << 1;
				//auto d = hash<string>()(k.k1) ^ (hash<string>()(k.k2) << 1);
				// ^: XOR 배타적 논리합, 하나만 참일 경우 참
				return hash<string>()(k.k1) ^ (hash<string>()(k.k2) << 1);
			}
		};

		// 비교용 함수 객체를 만드는 것이기 때문에 operator==를 오버로딩하지 않고 l, r 값을 받는 함수를 정의한다.
		struct KeyEqual {
			bool operator()(const A& l, const A& r) const {
				return l.k1 == r.k1 && l.k2 == r.k2;
			}
		};

		unordered_map<A, string, KeyHash, KeyEqual> um; // 해시함수와 이퀄함수를 제공하여 unordered_map을 정의한다.
		A a = A("a.k1", "a.k2");
		A b = A("b.k1", "b.k2");

		// 요소 추가
		um[a] = "a value";
		um[b] = "b value";
		string v = um[a];
		cout << v << endl; // a value

		// 덮어쓰기
		um[a] = "a value2";
		string v2 = um[a];
		cout << v2 << endl; // a value2

		for (auto iter : um)
		{
			cout << iter.first.k1 << ", " << iter.first.k2 << ": " << iter.second << endl; // a.k1, a.k2: a value2  , b.k1, b.k2: b value
		}
	}

	return 0;
}
#include <iostream>

// 0902 deadlock.cpp 파일의 코드가 이 파일에 그대로 들어오기 때문에 0902 deadlock.cpp 파일의 코드가 중복 정의되게 된다.
// 따라서 선언만 있는 헤더파일을 작성하여 include하거나 여기서 개별 함수이름을 선언하고 사용한다.(나중에 링커가 연결시켜준다)
//#include "0902 deadlock.cpp"

using namespace std;

int synchronization_object();
int deadlock();

int main() {
	synchronization_object();
	//deadlock();

	return 0;
}

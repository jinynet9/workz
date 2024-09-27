#include <iostream>
#include <iomanip>  // std::hex 선언
#include <cstring>

using namespace std;

int main_inline (void);
int main_namespace (void);
int main_bool (void);
int main_reference (void);
int main_c (void);
int main_struct (void);
int main_class (void);
int main_class2 (void);
int main_class3 (void);
int main_encap (void);
int main_encap2 (void);
int main_selfref (void);

int main ( int argc, char **argv, char **envp )
{
    main_selfref ();

    return 0;
}

//----

class SelfRef
{
private:
    int num;
public:
    SelfRef(int n) : num(n)
    {
        cout << "객체 생성" << endl;
    }
    SelfRef& Adder(int n)
    {
        num += n;
        return *this;
    }
    SelfRef& ShowNumber()
    {
        cout << num << endl;
        return *this;
    }
};

int main_selfref(void)
{
    SelfRef obj(3);
    SelfRef& ref = obj.Adder(2);

    obj.ShowNumber();
    ref.ShowNumber();

    cout << "&obj: " << &obj << endl;
    cout << "&ref: " << &ref << endl;

    ref.Adder(1).ShowNumber().Adder(2).ShowNumber();

    return 0;
}

//----

class SnivelCap
{
public:
    void Take() const // 클래스 내부에서 클래스 멤버 함수의 선언과 정의(구현)를 한꺼번에 시행  
    {
        cout << "콧물이 치료됩니다." << endl;
    }
};

class SneezeCap
{
public:
    void Take() const;  // 클래스 내부에서 클래스 멤버 함수 선언
};

void SneezeCap::Take() const  // 클래스 외부에서 클래스 멤버 함수 정의(구현)
{
    cout << "재채기가 멎습니다." << endl;
}

class SnuffleCap
{
public:
    void Take() const
    {
        cout << "코가 뻥 뚤립니다." << endl;
    }
};

class Contact600
{
private:
    SnivelCap scap;
    SneezeCap zcap;
    SnuffleCap ncap;
public:
    void Take() const
    {
        scap.Take();
        zcap.Take();
        ncap.Take();
    }
};


class ColdPatient
{
public:
    void TakeContact600(const Contact600& cap) const
    {
        cap.Take();
    }
};


int main_encap2(void)
{
    Contact600 cap;
    ColdPatient sufferer;

    sufferer.TakeContact600(cap);

    return 0;
}

#if 0
class SnivelCap
{
public:
    void Take() const // 클래스 내부에서 클래스 멤버 함수의 선언과 정의(구현)를 한꺼번에 시행  
    {
        cout << "콧물이 치료됩니다." << endl;
    }
};

class SneezeCap
{
public:
    void Take() const;  // 클래스 내부에서 클래스 멤버 함수 선언
};

void SneezeCap::Take() const  // 클래스 외부에서 클래스 멤버 함수 정의(구현)
{
    cout << "재채기가 멎습니다." << endl;
}

class SnuffleCap
{
public:
    void Take() const
    {
        cout << "코가 뻥 뚤립니다." << endl;
    }
};

class ColdPatient
{
public:
    void TakeSnivelCap(const SnivelCap& cap) const
    {
        cap.Take();
    }
    void TakeSneezeCap(const SneezeCap& cap) const
    {
        cap.Take();
    }
    void TakeSnuffleCap(const SnuffleCap& cap) const;
};

void ColdPatient::TakeSnuffleCap(const SnuffleCap& cap) const
{
    cap.Take();
}

int main_encap(void)
{
    SnivelCap scap;  //  SnivelCap의 객체(또는  인스턴스), 즉 메모리에 생성
    SneezeCap zcap;  //  SneezeCap의 객체(또는  인스턴스), 즉 메모리에 생성
    SnuffleCap ncap;  // SnuffleCap의 객체(또는  인스턴스), 즉 메모리에 생성

    ColdPatient sufferer;  // ColdPatient의 객체(또는  인스턴스), 즉 메모리에 생성

    sufferer.TakeSnivelCap(scap);
    sufferer.TakeSneezeCap(zcap);
    sufferer.TakeSnuffleCap(ncap);

    return 0;
}
#endif

//----

class FruitSeller
{
private:
    const int APPLE_PRICE;
    int numOfApples;
    int myMoney;
public:
    FruitSeller(int price, int num, int money)
        : APPLE_PRICE(price), numOfApples(num), myMoney(money) // 멤버 이니셜라이저
    {
    }
    int  SaleApples(int money)
    {
        int num = money / APPLE_PRICE;
        numOfApples -= num;
        myMoney += money;
        return num;
    }
    void ShowSalesResult() const
    {
        cout << "남은 사과: " << numOfApples << endl;
        cout << "판매 수익: " << myMoney << endl << endl;
    }
};

class FruitBuyer
{
private:
    int myMoney;
    int numOfApples;
public:
    FruitBuyer(int money)
        : myMoney(money), numOfApples(0) // 멤버 이니셜라이저
    {
    }
    void BuyApples(FruitSeller& seller, int money)
    {
        numOfApples += seller.SaleApples(money);  // 메시지 전달(Message Passing)
        myMoney -= money;
    }
    void ShowBuyResult() const
    {
        cout << "현재 잔액: " << myMoney << endl;
        cout << "사과 개수: " << numOfApples << endl << endl;
    }
};


int main_class3(void)
{
    FruitSeller seller(1000, 20, 0);
    FruitBuyer buyer(5000);
    buyer.BuyApples(seller, 2000);

    cout << "과일 판매자의 현황: " << endl;
    seller.ShowSalesResult();
    cout << "과일 구매자의 현황: " << endl;
    buyer.ShowBuyResult();

    return 0;
}

#if 0
class FruitSeller
{
private:
    int applePrice;
    int numOfApples;
    int myMoney;
public:
    void InitMembers(int price, int num, int money)
    {
        applePrice = price;
        numOfApples = num;
        myMoney = money;
    }
    int  SaleApples(int money)
    {
        int num = money / applePrice;
        numOfApples -= num;
        myMoney += money;
        return num;
    }
    void ShowSalesResult()
    {
        cout << "남은 사과: " << numOfApples << endl;
        cout << "판매 수익: " << myMoney << endl << endl;
    }
};

class FruitBuyer
{
private:
    int myMoney;
    int numOfApples;
public:
    void InitMembers(int money)
    {
        myMoney = money;
        numOfApples = 0;
    }
    void BuyApples(FruitSeller& seller, int money)
    {
        numOfApples += seller.SaleApples(money);  // 메시지 전달(Message Passing)
        myMoney -= money;
    }
    void ShowBuyResult()
    {
        cout << "현재 잔액: " << myMoney << endl;
        cout << "사과 개수: " << numOfApples << endl << endl;
    }
};

int main_class2 ( void )
{
    FruitSeller seller;
    seller.InitMembers(1000, 20, 0);
    FruitBuyer buyer;
    buyer.InitMembers(5000);
    buyer.BuyApples(seller, 2000);

    cout << "과일 판매자의 현황: " << endl;
    seller.ShowSalesResult();
    cout << "과일 구매자의 현황: " << endl;
    buyer.ShowBuyResult();
}
#endif

class Car
{

private:
    // 멤버 상수
    enum
    {
        ID_LEN = 20,
        MAX_SPD = 200,
        FUEL_STEP = 2,
        ACC_STEP = 10,
        BRK_STEP = 10
    };
    // 맴버 변수
    char gamerID[ID_LEN];
    int fuelGauge;
    int curSpeed;
public:
    // 맴버 함수
    void InitMembers(const char* ID, int fuel);
    void ShowCarState();
    void Accel();
    void Break();
};

void Car::InitMembers(const char* ID, int fuel)
{
    //strcpy_s(gamerID, ID);  // strcpy는 안전하지 않은 함수이므로 strcpy_s 사용
    strcpy(gamerID, ID);
    fuelGauge = fuel;
    curSpeed = 0;
}

void Car::ShowCarState()
{
    cout << "소유자 ID: " << gamerID << endl;
    cout << "연료량: " << fuelGauge << "%" << endl;
    cout << "현재 속도: " << curSpeed << "km/h" << endl << endl;
}

void Car::Accel()
{
    if (fuelGauge <= 0)
        return;
    else
        fuelGauge -= FUEL_STEP;

    if (curSpeed + ACC_STEP >= MAX_SPD)
    {
        curSpeed = MAX_SPD;
        return;
    }
    curSpeed += ACC_STEP;
}

void Car::Break()
{
    if (curSpeed < BRK_STEP)
    {
        curSpeed = 0;
        return;
    }
    curSpeed -= BRK_STEP;
}

class A
{
public:
    A() { std::cout << "A\n"; }
};

class B
{
private:
    A m_a; // B contains A as a member variable

public:
    B() { std::cout << "B\n"; }
};

int main_class ( void )
{
    //B* b = new B();

    Car run99;  // 클래스 Car의 객체 run99
    run99.InitMembers("run99", 100);  // 클래스는 선언과 초기화를 분리해야 함
    run99.Accel(); // run99의 맴버 함수
    run99.Accel();
    run99.ShowCarState();
    run99.Break();
    run99.ShowCarState();

    Car sped77;  // 클래스 Car의 객체 sped77
    sped77.InitMembers("sped77", 100);
    sped77.Accel();  // sped77의 맴버 함수
    sped77.Break();
    sped77.ShowCarState();

    return 0;
}

//----

#if 0
struct Car
{
    enum
    {
        ID_LEN = 20,
        MAX_SPD = 200,
        FUEL_STEP = 2,
        ACC_STEP = 10,
        BRK_STEP = 10
    };

    char gamerID[ID_LEN];
    int fuelGauge;
    int curSpeed;

    inline void ShowCarState();
    inline void Accel();
    inline void Break();
};

void Car::ShowCarState()
{
    cout << "소유자 ID: " << gamerID << endl;
    cout << "연료량: " << fuelGauge << "%" << endl;
    cout << "현재 속도: " << curSpeed << "km/h" << endl << endl;
}

void Car::Accel()
{
    if (fuelGauge <= 0)
        return;
    else
        fuelGauge -= FUEL_STEP;

    if (curSpeed + ACC_STEP >= MAX_SPD)
    {
        curSpeed = MAX_SPD;
        return;
    }
    curSpeed += ACC_STEP;
}

void Car::Break()
{
    if (curSpeed < BRK_STEP)
    {
        curSpeed = 0;
        return;
    }
    curSpeed -= BRK_STEP;
}

int main_struct(void)
{
    Car run99 = { "run99", 100, 0 };
    run99.Accel(); // run99의 맴버 함수
    run99.Accel();
    run99.ShowCarState();
    run99.Break();
    run99.ShowCarState();

    Car sped77 = { "sped77", 100, 0 };
    sped77.Accel();  // sped77의 맴버 함수
    sped77.Break();
    sped77.ShowCarState();

    return 0;
} 
#endif

#if 0
#define ID_LEN      20
#define MAX_SPD     200
#define FUEL_STEP   2
#define ACC_STEP    10
#define BRK_STEP    10

struct Car
{
    char gamerID[ID_LEN];
    int fuelGauge;
    int curSpeed;

    void ShowCarState()
    {
        cout << "소유자 ID: " << gamerID << endl;
        cout << "연료량: " << fuelGauge << "%" << endl;
        cout << "현재 속도: " << curSpeed << "km/h" << endl << endl;
    }

    void Accel()
    {
        if (fuelGauge <= 0)
            return;
        else
            fuelGauge -= FUEL_STEP;

        if (curSpeed + ACC_STEP >= MAX_SPD)
        {
            curSpeed = MAX_SPD;
            return;
        }
        curSpeed += ACC_STEP;
    }

    void Break()
    {
        if (curSpeed < BRK_STEP)
        {
            curSpeed = 0;
            return;
        }
        curSpeed -= BRK_STEP;
    }
};

int main_struct(void)
{
    Car run99 = { "run99", 100, 0 };
    run99.Accel(); // run99의 맴버 함수
    run99.Accel();
    run99.ShowCarState();
    run99.Break();
    run99.ShowCarState();

    Car sped77 = { "sped77", 100, 0 };
    sped77.Accel();  // sped77의 맴버 함수
    sped77.Break();
    sped77.ShowCarState();

    return 0;
} 
#endif

#if 0
struct Car
{
    char gamerID[ID_LEN];
    int fuelGauge;
    int curSpeed;
};

void ShowCarState(const Car& car)
{
    cout << "소유자 ID: " << car.gamerID << endl;
    cout << "연료량: " << car.fuelGauge << "%" << endl;
    cout << "현재 속도: " << car.curSpeed << "km/h" << endl << endl;
}

void Accel(Car& car)
{
    if (car.fuelGauge <= 0)
        return;
    else
        car.fuelGauge -= FUEL_STEP;

    if (car.curSpeed + ACC_STEP >= MAX_SPD)
    {
        car.curSpeed = MAX_SPD;
        return;
    }
    car.curSpeed += ACC_STEP;
}

void Break(Car& car)
{
    if (car.curSpeed < BRK_STEP)
    {
        car.curSpeed = 0;
        return;
    }

    car.curSpeed -= BRK_STEP;
}

int main_struct(void)
{
    Car run99 = { "run99", 100, 0 };
    Accel(run99);
    Accel(run99);
    ShowCarState(run99);
    Break(run99);
    ShowCarState(run99);

    Car sped77 = { "sped77", 100, 0 };
    Accel(sped77);
    Break(sped77);
    ShowCarState(sped77);

    return 0;
} 
#endif

//----

#include <cstdio>   // stdio.h 에 대응
#include <cstring>  // string.h 에 대응
#include <cmath>    // math.h 에 대응

int main_c(void)
{
    char str1[] = "Result";
    char str2[30];

    //strcpy_s(str2, str1);  // strcpy_s는 오버로딩되어 있음(즉, 크기가 정해진 배열을 두 입력으로 사용할 수 있음)
    strcpy(str2, str1);
    printf("%s: %f \n", str1, sin(0.14));
    printf("%s: %f \n", str2, fabs(-1.25));

    return 0;
}

//----

void SwapByRef2(int& ref1, int& ref2)
{
    int temp = ref1;
    ref1 = ref2;
    ref2 = temp;
}

int& RefRetFunc1(int& ref)
{
    ref++;
    return ref;
}

int main_reference(void)
{
#if 1
    int* ip = new int;  // new int[1]와 같은 표현
    int& ref = *ip;  // 참조자를 통한 힙 영역 접근. 즉, int ref를 스택이 아닌 힙에서 생성한 효과

    *ip = 100;

    cout << std::hex << "ip: " << ip << endl;  // 주소가 16진수로 표시
    cout << "ip: " << ip << endl;  // <iomanip>에 의해 주소가 자동으로 16진수로 표시
    cout << "*ip: " << *ip << endl;
    cout << "ref: " << ref << endl;
    cout << "&ref: " << &ref << endl;
    cout << "&ip : " << &ip << endl;

    delete ip;
#endif

#if 0
    int num1 = 1;
    int& num2 = RefRetFunc1(num1);
//    int num2 = RefRetFunc1(num1);

    cout << "num1: " << num1 << endl;
    cout << "num2: " << num2 << endl;

    num1 *= 2;
    num2 *= 3;

    cout << "num1: " << num1 << endl;
    cout << "num2: " << num2 << endl;
    cout << "&num1: " << &num1 << endl;
    cout << "&num2: " << &num2 << endl;
#endif

#if 0
    int val1 = 10;
    int val2 = 20;

    cout << "---before swapping---" << endl;
    cout << "val1: " << val1 << endl;
    cout << "val2: " << val2 << endl;

    SwapByRef2(val1, val2);  // 함수 호출 시 함수의 매개 변수 참조자들은 초기화 된다.
      // 즉, int& ref1 = val1, int& ref2 = val2로 초기화 됨.

    cout << "---after swapping---" << endl;
    cout << "val1: " << val1 << endl;
    cout << "val2: " << val2 << endl;
#endif

#if 0
    int num1 = 20;
    int& num2 = num1; // num1과 같은 메모리 공간을 가리키는 또 하나의 변수 이름. 
                      //즉, 참조자(reference)는 변수에 대해서만 선언이 가능하고, 
                      // 선언과 동시에 참조할 변수로 초기화 해야 한다.
    int* num3 = &num1; // num1의 시작 주소를 가리키는 포인터

    cout << "num1의 주소: " << &num1 << endl;
    cout << "num2의 주소: " << &num2 << endl;
    cout << "num3의 주소: " << &num3 << endl;

    cout << "sizeof num1: " << sizeof(num1) << endl;
    cout << "sizeof num2: " << sizeof(num2) << endl;
    cout << "sizeof num3: " << sizeof(num3) << endl;

    num2 = 3047;  // int형 변수 num1 처럼 int 참조자로 선언된 num2에 정수값을 할당할 수 있다.
    cout << "num1 = " << num1 << endl;
    cout << "num2 = " << num2 << endl;
    cout << "*num3 = " << *num3 << endl;
#endif

#if 0
    int arr[3] = {1, 2, 3};

    int& ref1 = arr[0];
    int& ref2 = arr[1];
    int& ref3 = arr[2];

    int* ptr1 = &arr[0];
    int* ptr2 = &arr[1];
    int* ptr3 = &arr[2];

    cout << ref1 << endl;
    cout << ref2 << endl;
    cout << ref3 << endl;

    cout << ptr1 << endl;
    cout << ptr2 << endl;
    cout << ptr3 << endl;

    cout << *ptr1 << endl;
    cout << *ptr2 << endl;
    cout << *ptr3 << endl;
#endif

#if 0
    int num = 12;
    int* ptr = &num;
    int** dptr = &ptr;

    int& ref = num;  // int &ref = num; 같은 표기
    int*& pref = ptr;  // int* (&pref) = ptr; 같은 표기
    int**& dpref = dptr;  // int** (&dpref) = dptr; 같은 표기

    cout << ref << endl;
    cout << *pref << endl;
    cout << **dpref << endl; 
#endif

    return 0;
}

//----

using namespace std;

int main_bool(void)
{
    int num = 10;
    int i = 0;

    cout << "true: " << true << endl;
    cout << "false: " << false << endl;

    while(true)
    {
        cout << i++ << ' ';
        if(i > num)
        {
            break;
        }
    }
    cout << endl;

    cout << "size of 1: " << sizeof(1) << endl;
    cout << "size of 0: " << sizeof(0) << endl;
    cout << "size of true: " << sizeof(true) << endl;
    cout << "size of false: " << sizeof(false) << endl;
    cout << "size of bool: " << sizeof(bool) << endl;

    return 0;
}

//----

namespace Parent
{
    int num = 0;
    namespace Son
    {
        int num = 1;
    }
    namespace Daughter
    {
        int num = 2;
    }
}

namespace Hybrid
{
    void HybFunc(void)
    {
        std::cout << "So simple function!" << std::endl;
        std::cout << "In namespace Hybrid" << std::endl;
    }
}

namespace AAA
{
    namespace BBB
    {
        namespace CCC
        {
            int num1;
            int num2;
        }
    }
}

int val=100;

int main_namespace(void)
{
#if 0
    std::cout << Parent::num << std::endl;
    std::cout << Parent::Son::num << std::endl;
    std::cout << Parent::Daughter::num << std::endl;
#endif

#if 0
    using Hybrid::HybFunc; // 이 라인 실행후 namespace Hybrid는 생략 가능
    HybFunc();
#endif

#if 0
using std::cin;
using std::cout;
using std::endl;

    cout << "Hello World!" << endl;
#endif

#if 0
using namespace std;

    cout << "Hello World!" << endl;
#endif

#if 0
using namespace std;

    AAA::BBB::CCC::num1 = 20;
    AAA::BBB::CCC::num2 = 30;

    namespace ABC = AAA::BBB::CCC;

    cout << ABC::num1 << endl;
    cout << ABC::num2 << endl;
#endif

#if 1
using namespace std;

    int val = 20; // 지역 변수
    val += 3;  // 지역 변수 val의 값 증가
    ::val += 3; // 전역변수 val의 값 증가

    cout << "지역변수 val의 값: " << val << endl;
    cout << "전역변수 val의 값: " << ::val << endl;
#endif

    return 0;
}

//----

template <typename T>
inline T SQUARE(T x)
{
    return x*x;
}

inline int SQUARE_(int x)
{
    return x*x;
}

int main_inline (void)
{
    std::cout << SQUARE_(5) << std::endl; 
    std::cout << SQUARE_(7.15) << std::endl; // 7.15를 7로 간주 데이터 손실

    std::cout << SQUARE(5) << std::endl; 
    std::cout << SQUARE(7.15) << std::endl;

    return 0;
}

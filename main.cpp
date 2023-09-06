#include <iostream>
#include <concepts>

// �������� �������� ComplexConcept, ������������� ��������� ���������� � ��������� ������
template<typename T>
concept HasHashCalculationConvertibleToLong = requires(T v)
{
    { v.hash() }
    -> std::convertible_to<long>;
};

template<typename T>
concept HasToStringMethod = requires(T v)
{
    { v.toString() }
    -> std::same_as<std::string>;
};

template<typename T>
concept ComplexConcept = HasHashCalculationConvertibleToLong<T> && HasToStringMethod<T> && !std::has_virtual_destructor<T>::value;

// ������� ����������� ���������� �������� � ���� ����������,
// � ������� �������� '�������' ������� � ������� ������ ��� �������, ������� ����� ��� � ������.
void printHash(ComplexConcept auto& t)
{
    std::cout << "Hash(\"" << t.toString() << "\") = " << t.hash() << std::endl;
}

// �����, ��������������� ����������� �������� ComplexConcept
class TestClassSuitable
{
public:
    TestClassSuitable(const std::string& value) : _value(value) {};
    ~TestClassSuitable() {};    // ����������� �������� �� ������������� ����������
    size_t hash() const { return std::hash<std::string>{}(_value); } // ����������� ������� ���������������� � long
    std::string toString() const { return _value; } // ����������� ������� �� ������� std::string
private:
    std::string _value;
};

// �����, �� ��������������� ����������� �������� ComplexConcept
class TestClassUnSuitable
{
public:
    TestClassUnSuitable(const std::string& value) {};
    virtual ~TestClassUnSuitable() {};  // ����������� ����������
    std::string hash() const { return std::string(); }  // �� �������������� � long
    int toString() const { return 1; }  // �� �������� std::string
};

int main()
{
    setlocale(LC_ALL, "");
    TestClassSuitable correct("Test1234567890");
    TestClassUnSuitable incorrect("Pass1234");

    printHash(correct); // �������� ����������
    printHash(incorrect); // ������ ����������
}
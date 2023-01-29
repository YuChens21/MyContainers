#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include "../headers/myArray.h"
#include "../headers/myVector.h"

struct myStruct
{
    int* arr = nullptr;
    std::string str;

    myStruct(const std::string& s) :str(s)
    {
        arr = new int[10];
        std::cout << "create" << std::endl;
    }
    myStruct(const myStruct& ms) :str(ms.str)
    {
        arr = new int[10];
        std::cout << "copy" << std::endl;
    }
    myStruct(myStruct&& vec) noexcept
    {
        arr = vec.arr;
        delete[] vec.arr;
        str = std::move(vec.str);
        std::cout << "move" << std::endl;
    }
    ~myStruct()
    {
        delete[] arr;
        std::cout << "destroy" << std::endl;
    }
    myStruct& operator=(const myStruct& ms)
    {
        arr = new int[10];
        str = ms.str;
        std::cout << "copy" << std::endl;
    }
    myStruct& operator=(myStruct&& vec) noexcept
    {
        arr = vec.arr;
        delete[] vec.arr;
        str = std::move(vec.str);
        std::cout << "move" << std::endl;
    }
};

template<class T>
void PrintVector(const Vector<T>& vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << std::endl;
    }
}

template<class T>
void PrintVector(const Vector<myStruct>& vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i].x << " " << vec[i].y << " " << vec[i].z << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
}

int main()
{
    std::vector<int> str = { 0,1,2,5 };
    std::vector<int> b = { 0,1,2 };
    std::cout << (str < b) << std::endl;
    return 0;
}
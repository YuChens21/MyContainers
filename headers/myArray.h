#ifndef __myArray__
#define __myArray__
#include <stdexcept>

template<class T, size_t m_Size>
class Array
{
public:
    /*** Capacity ***/

    [[nodiscard]] constexpr size_t size() const noexcept { return m_Size; }
    [[nodiscard]] constexpr size_t max_size() const noexcept { return m_Size; }
    [[nodiscard]] constexpr bool empty() const noexcept { return false; }

    /*** Element Access ***/

    [[nodiscard]] T& operator[](size_t index) noexcept
    {
#ifdef _DEBUG
        if (index >= m_Size)
        {
            throw std::out_of_range("Invalid index!");
        }
#endif // _DEBUG
        return m_Data[index];
    }

    [[nodiscard]] const T& operator[](size_t index) const noexcept
    {
#ifdef _DEBUG
        if (index >= m_Size)
        {
            throw std::out_of_range("Invalid index!");
        }
#endif // _DEBUG
        return m_Data[index];
    }

    [[nodiscard]] T& at(size_t index)
    {
        if (index >= m_Size)
        {
            throw std::out_of_range("Invalid index!");
        }
        return m_Data[index];
    }

    [[nodiscard]] const T& at(size_t index) const
    {
        if (index >= m_Size)
        {
            throw std::out_of_range("Invalid index!");
        }
        return m_Data[index];
    }

    [[nodiscard]] T& front() noexcept { return m_Data[0]; }
    [[nodiscard]] const T& front() const noexcept { return m_Data[0]; }
    [[nodiscard]] T& back() noexcept { return m_Data[m_Size - 1]; }
    [[nodiscard]] const T& back() const noexcept { return m_Data[m_Size - 1]; }
    [[nodiscard]] T* data() noexcept { return m_Data; }
    [[nodiscard]] const T* data() const noexcept { return m_Data; }

    /*** modifiers ***/

    void fill(const T& val)
    {
        for (size_t i = 0; i < m_Size; ++i)
        {
            m_Data[i] = val;
        }
    }

    void swap(Array<T, m_Size>& other) noexcept
    {
        for (size_t i = 0; i < m_Size; ++i)
        {
            T temp = std::move(m_Data[i]);
            m_Data[i] = std::move(other.m_Data[i]);
            other.m_Data[i] = std::move(temp);
        }
    }
private:
    T m_Data[m_Size];
};

template<class T>
class Array<T, 0> // empty array
{
public:
    /*** Capacity ***/

    [[nodiscard]] constexpr size_t size() const noexcept { return 0; }
    [[nodiscard]] constexpr size_t max_size() const noexcept { return 0; }
    [[nodiscard]] constexpr bool empty() const noexcept { return true; }

    /*** Element Access***/

    [[nodiscard]] T& operator[](size_t index) noexcept { return *data(); }
    [[nodiscard]] const T& operator[](size_t index) const noexcept { return *data(); }
    [[noreturn]] T& at(size_t index) { throw std::out_of_range("Invalid index!"); }
    [[noreturn]] const T& at(size_t index) const { throw std::out_of_range("Invalid index!"); }
    [[nodiscard]] T& front()noexcept { return *data(); }
    [[nodiscard]] const T& front() const noexcept { return *data(); }
    [[nodiscard]] T& back() noexcept { return *data(); }
    [[nodiscard]] const T& back() const noexcept { return *data(); }
    [[nodiscard]] T* data() noexcept { return nullptr; }
    [[nodiscard]] const T* data() const noexcept { return nullptr; }

    /*** modifiers ***/

    void fill(const T&) {}
    void swap(const Array<T, 0>&) noexcept {}
};

/****** non-member functions ******/

template<size_t index, class T, size_t m_Size>
[[nodiscard]] constexpr T& get(Array<T, m_Size>& arr) noexcept
{
    static_assert (index < m_Size, "Invalid index!");
    return arr[index];
}

template<size_t index, class T, size_t m_Size>
[[nodiscard]] constexpr const T& get(const Array<T, m_Size>& arr) noexcept
{
    static_assert (index < m_Size, "Invalid index!");
    return arr[index];
}

template<size_t index, class T, size_t m_Size>
[[nodiscard]] constexpr T&& get(Array<T, m_Size>&& arr) noexcept
{
    static_assert (index < m_Size, "Invalid index!");
    return std::move(arr[index]);
}

template<class T, size_t m_Size>
[[nodiscard]] bool operator==(const Array<T, m_Size>& lhs, const Array<T, m_Size>& rhs)
{
    if (&lhs == &rhs) { return true; }
    for (int i = 0; i < m_Size; ++i)
    {
        if (lhs[i] != rhs[i]) { return false; }
    }
    return true;
}

template<class T, size_t m_Size>
[[nodiscard]] bool operator!=(const Array<T, m_Size>& lhs, const Array<T, m_Size>& rhs)
{
    return !(lhs == rhs);
}

template<class T, size_t m_Size>
[[nodiscard]] bool operator<(const Array<T, m_Size>& lhs, const Array<T, m_Size>& rhs)
{
    if (&lhs == &rhs) { return false; }
    for (int i = 0; i < m_Size; ++i)
    {
        if (lhs[i] < rhs[i]) { return true; }
        if (lhs[i] > rhs[i]) { return false; }
    }
    return false;
}

template<class T, size_t m_Size>
[[nodiscard]] bool operator>(const Array<T, m_Size>& lhs, const Array<T, m_Size>& rhs)
{
    if (&lhs == &rhs) { return false; }
    for (int i = 0; i < m_Size; ++i)
    {
        if (lhs[i] > rhs[i]) { return true; }
        if (lhs[i] < rhs[i]) { return false; }
    }
    return false;
}

template<class T, size_t m_Size>
[[nodiscard]] bool operator<=(const Array<T, m_Size>& lhs, const Array<T, m_Size>& rhs)
{
    return !(lhs > rhs);
}

template<class T, size_t m_Size>
[[nodiscard]] bool operator>=(const Array<T, m_Size>& lhs, const Array<T, m_Size>& rhs)
{
    return !(lhs < rhs);
}
#endif // __Array__

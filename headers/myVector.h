#ifndef __myVector__
#define __myVector__
#include<stdexcept>

template<class T>
class Vector
{
public:
    Vector() :m_Data(nullptr), m_Size(0), m_Capacity(0) {}
    Vector(size_t arr);
    Vector(size_t arr, const T& val);
    Vector(const Vector<T>& vec);
    Vector(Vector<T>&& vec) noexcept
        :m_Data(vec.m_Data), m_Size(vec.m_Size), m_Capacity(vec.m_Capacity)
    {
        vec.m_Data = nullptr;
    }
    ~Vector();
    Vector<T>& operator=(const Vector<T>& rhs);
    Vector<T>& operator=(Vector<T>&& rhs) noexcept;

    /*** Capacity ***/

    [[nodiscard]] size_t size() const noexcept { return m_Size; }
    void resize(size_t newSize);
    void resize(size_t newSize, const T& val);
    [[nodiscard]] size_t capacity() const noexcept { return m_Capacity; }
    [[nodiscard]] bool empty() const noexcept { return m_Size == 0; }
    void reserve(size_t newCapacity);
    void shrink_to_fit();

    /*** Element Access ***/

    [[nodiscard]] T& operator[](size_t index) noexcept;
    [[nodiscard]] const T& operator[](size_t index) const noexcept;
    [[nodiscard]] T& at(size_t index);
    [[nodiscard]] const T& at(size_t index) const;
    [[nodiscard]] T& front() { return m_Data[0]; }
    [[nodiscard]] const T& front() const { return m_Data[0]; }
    [[nodiscard]] T& back() { return m_Data[m_Size - 1]; }
    [[nodiscard]] const T& back() const { return m_Data[m_Size - 1]; }
    [[nodiscard]] T* data() noexcept { return m_Data; }
    [[nodiscard]] const T* data() const noexcept { return m_Data; }

    /*** Modifiers ***/

    void assign(size_t arr, const T& val);
    void push_back(const T& val) { emplace_back(val); }
    void push_back(T&& val) { emplace_back(std::move(val)); }
    void pop_back() noexcept;
    void swap(Vector<T>& other);
    void clear() noexcept;

    template<class... Args>
    void emplace_back(Args&&... args);

private:
    void checkIndex(size_t index) const;
    void reAllocate(size_t newCapacity); // not fill blanks

    T* m_Data = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
};

template<class T>
inline void Vector<T>::checkIndex(size_t index) const
{
    if (index >= m_Size)
    {
        throw std::out_of_range("Invalid index!");
    }
}

template<class T>
inline void Vector<T>::reAllocate(size_t newCapacity)
{
    if (m_Capacity == newCapacity) { return; }
    T* newData = nullptr;
    size_t newSize = newCapacity < m_Size ? newCapacity : m_Size;
    if (newCapacity != 0)
    {
        newData = (T*)::operator new(newCapacity * sizeof(T));
        for (size_t i = 0; i < newSize; ++i)
        {
            new(&newData[i]) T(std::move(m_Data[i]));
        }
    }

    for (size_t i = 0; i < m_Size; ++i)
    {
        m_Data[i].~T();
    }
    ::operator delete(m_Data, m_Capacity * sizeof(T));

    m_Data = newData;
    m_Size = newSize;
    m_Capacity = newCapacity;
}

template<class T>
inline Vector<T>::Vector(size_t arr) : m_Size(arr), m_Capacity(arr)
{
    m_Data = (T*)::operator new(arr * sizeof(T));
    for (size_t i = 0; i < m_Size; ++i)
    {
        new(&m_Data[i]) T;
    }
}

template<class T>
inline Vector<T>::Vector(size_t arr, const T& val) : m_Size(arr), m_Capacity(arr)
{
    m_Data = (T*)::operator new(arr * sizeof(T));
    for (size_t i = 0; i < m_Size; ++i)
    {
        new(&m_Data[i]) T(val);
    }
}

template<class T>
inline Vector<T>::Vector(const Vector<T>& vec)
    :m_Size(vec.m_Size), m_Capacity(vec.m_Capacity)
{
    m_Data = (T*)::operator new(vec.m_Capacity * sizeof(T));
    for (size_t i = 0; i < vec.m_Size; ++i)
    {
        new(&m_Data[i]) T(vec.m_Data[i]);
    }
}

template<class T>
inline Vector<T>::~Vector()
{
    for (size_t i = 0; i < m_Size; ++i)
    {
        m_Data[i].~T();
    }
    ::operator delete(m_Data, m_Capacity * sizeof(T));
}

template<class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
{
    if (this != &rhs)
    {
        for (size_t i = 0; i < m_Size; ++i)
        {
            m_Data[i].~T();
        }
        ::operator delete(m_Data, m_Capacity * sizeof(T));

        m_Data = (T*)::operator new(rhs.m_Capacity * sizeof(T));
        for (size_t i = 0; i < rhs.m_Size; ++i)
        {
            new(&m_Data[i]) T(rhs.m_Data[i]);
        }
        m_Size = rhs.m_Size;
        m_Capacity = rhs.m_Capacity;
    }
    return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_Size = rhs.m_Size;
        m_Capacity = rhs.m_Capacity;
        m_Data = rhs.m_Data;
        rhs.m_Data = nullptr;
    }
    return *this;
}

template<class T>
void Vector<T>::resize(size_t newSize)
{
    if (newSize == m_Size) { return; }

    size_t oldSize = m_Size;
    if (newSize < oldSize)
    {
        for (size_t i = newSize; i < oldSize; ++i)
        {
            m_Data[i].~T();
        }
    }
    else if (newSize > m_Capacity)
    {
        reAllocate(newSize);
    }

    for (size_t i = oldSize; i < newSize; ++i)
    {
        new(&m_Data[i]) T;
    }
    m_Size = newSize;
}

template<class T>
void Vector<T>::resize(size_t newSize, const T& val)
{
    if (newSize == m_Size) { return; }

    size_t oldSize = m_Size;
    if (newSize < oldSize)
    {
        for (size_t i = newSize; i < oldSize; ++i)
        {
            m_Data[i].~T();
        }
    }
    else if (newSize > m_Capacity)
    {
        reAllocate(newSize);
    }

    for (size_t i = oldSize; i < newSize; ++i)
    {
        new(&m_Data[i]) T(val);
    }
    m_Size = newSize;
}

template<class T>
void Vector<T>::reserve(size_t newCapacity)
{
    if (newCapacity > m_Capacity) { reAllocate(newCapacity); }
}

template<class T>
void Vector<T>::shrink_to_fit()
{
    if (m_Size != m_Capacity) { reAllocate(m_Size); }
}

template<class T>
inline [[nodiscard]] T& Vector<T>::operator[](size_t index) noexcept
{
#ifdef _DEBUG
    checkIndex(index);
#endif // _DEBUG
    return m_Data[index];
}

template<class T>
inline [[nodiscard]] const T& Vector<T>::operator[](size_t index) const noexcept
{
#ifdef _DEBUG
    checkIndex(index);
#endif // _DEBUG
    return m_Data[index];
}

template<class T>
inline [[nodiscard]] T& Vector<T>::at(size_t index)
{
    checkIndex(index);
    return m_Data[index];
}

template<class T>
inline [[nodiscard]] const T& Vector<T>::at(size_t index) const
{
    checkIndex(index);
    return m_Data[index];
}

template<class T>
inline void Vector<T>::assign(size_t newSize, const T& val)
{
    for (size_t i = 0; i < m_Size; ++i) { m_Data[i].~T(); }
    if (newSize > m_Capacity)
    {
        ::operator delete(m_Data, m_Capacity * sizeof(T));
        m_Data = (T*)::operator new(newSize * sizeof(T));
        m_Capacity = newSize;
    }

    m_Size = newSize;
    for (size_t i = 0; i < newSize; ++i)
    {
        new(&m_Data[i]) T(val);
    }
}

template<class T>
void Vector<T>::pop_back() noexcept
{
#ifdef _DEBUG
    if (m_Size == 0)
    {
        throw std::range_error("Empty vector before pop!");
    }
#endif // _DEBUG
    --m_Size;
    m_Data[m_Size].~T();
}

template<class T>
inline void Vector<T>::swap(Vector<T>& other)
{
    std::swap(m_Data, other.m_Data);
    std::swap(m_Size, other.m_Size);
    std::swap(m_Capacity, other.m_Capacity);
}

template<class T>
inline void Vector<T>::clear() noexcept
{
    for (size_t i = 0; i < m_Size; ++i)
    {
        m_Data[i].~T();
    }
    m_Size = 0;
}

template<class T>
template<class... Args>
inline void Vector<T>::emplace_back(Args&&... args)
{
    if (m_Size >= m_Capacity)
    {
        reAllocate(std::max(m_Capacity + m_Capacity / 2, m_Size + 1));
    }
    new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
    ++m_Size;
}

/****** non-member functions ******/

template<class T>
void swap(Vector<T>& lhs, Vector<T>& rhs)
{
    lhs.swap(rhs);
}

template<class T>
[[nodiscard]] bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (&lhs == &rhs) { return true; }
    if (lhs.size() != rhs.size()) { return false; }
    for (size_t i = 0; i < lhs.size(); ++i)
    {
        if (lhs[i] != rhs[i]) { return false; }
    }
    return true;
}

template<class T>
[[nodiscard]] bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs == rhs);
}

template<class T>
[[nodiscard]] bool operator<(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (&lhs == &rhs) { return false; }
    size_t minSize = std::min(lhs.size(), rhs.size());
    for (size_t i = 0; i < minSize; ++i)
    {
        if (lhs[i] < rhs[i]) { return true; }
        else if (lhs[i] > rhs[i]) { return false; }
    }
    return lhs.size() < rhs.size();
}

template<class T>
[[nodiscard]] bool operator>(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (&lhs == &rhs) { return false; }
    size_t minSize = std::min(lhs.size(), rhs.size());
    for (size_t i = 0; i < minSize; ++i)
    {
        if (lhs[i] > rhs[i]) { return true; }
        else if (lhs[i] < rhs[i]) { return false; }
    }
    return lhs.size() > rhs.size();
}

template<class T>
[[nodiscard]] bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs > rhs);
}

template<class T>
[[nodiscard]] bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs < rhs);
}
#endif // __myVector__

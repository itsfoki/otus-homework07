#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iostream>


template<typename T>
class VecIterator;

template<typename T>
class Vector {

    typedef VecIterator<T> iterator;
    typedef VecIterator<const T> const_iterator;

public:
    Vector() { // конструктор без параметров
        m_capacity = 0;
        m_size = 0;
        m_end = nullptr;
        m_begin = nullptr;
        m_end_capacity = nullptr;
    }

    Vector(const Vector<T> &other) { // конструктор для копирования
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_begin = new T[m_capacity]();
        copy_elements_from(other);
        m_end = m_begin + m_size;
        m_end_capacity = m_begin + m_capacity;
    }


    Vector(const Vector<T> &&other) noexcept {  // конструктор перемещения
        delete_elements();                          
        this->m_begin = other.m_begin;
        this->m_end = other.m_end;
        this->m_end_capacity = other.m_end_capacity;
        this->m_size = other.m_size;
        this->m_capacity = other.m_capacity;
    }


    //деструктор
    ~Vector() {
        delete_elements();
    }


    Vector<T> &operator=(const Vector<T> &other) { // присваивание копированием
        //проверка на присваивание самому себе
        if (other.m_begin == this->m_begin) { return *this; }

        if (this->m_capacity >= other.m_size) {
            copy_elements_from(other);
            m_size = other.m_size;
            return *this;
        } else {
            // если другой вектор не помещается в наш, то удаляем
            delete_elements();

            this->m_capacity = other.m_capacity;
            m_begin = new T[m_capacity]();
            copy_elements_from(other);
            m_size = other.m_size;
            return *this;
        }
    }


    Vector<T> &operator=(Vector<T> &&other) noexcept { // оператор присваивания перемещением
        // проверка
        if (other.m_begin == this->m_begin) { return *this; }
        delete_elements();
        // присваиваем чужие указатели и поля
        this->m_begin = other.m_begin;
        this->m_end = other.m_end;
        this->m_end_capacity = other.m_end_capacity;
        this->m_size = other.m_size;
        this->m_capacity = other.m_capacity;
        other.m_begin = nullptr;
        return *this;
    };


    [[nodiscard]] size_t size() const { return m_size; }


    
    void push_back(const T &value) { // добавление элемента в конец вектора

        // если память под массив ещё не выделена, то выделяем и инициализируем соответствующие переменные
        if (m_begin == nullptr) {
            m_capacity = m_min_elements;
            m_begin = new T[m_capacity];
            m_end = m_begin + m_size;
            m_end_capacity = m_begin + m_capacity;
        }
        // если есть ещё место в векторе
        if (m_end != m_end_capacity) {
            *m_end = value;
            ++m_end;
            ++m_size;
        } else {
            // если места для добавления нового элемента уже нет, то
            m_capacity *= m_multiplicator;  // увеличиваем размер capacity
            T *temp = new T[m_capacity];    // выделяем новый увеличенный массив
            copy_elements_to(temp);         
            *(temp + m_size) = value;
            ++m_size;
            delete_elements();  
            // присваиваем новые указатели           
            this->m_begin = temp;           
            this->m_end = temp + m_size;
            this->m_end_capacity = temp + m_capacity;
        }
    }


    iterator insert(iterator pos, const T &value) {
        // проверка на корректность переданного итератора
        if (pos.get() >= m_begin && pos.get() <= m_end) {
            // вставка почти всегда вызывает реаллокацию всего массива
            // только если это не вставка в конец и есть свободное место
            if (pos.get() == m_end && m_end != m_end_capacity) {
                *m_end = value;
                T *new_pos = m_end;
                ++m_end;
                ++m_size;
                return iterator(new_pos);
            } else {
                // проверяем необходимость увеличить массив
                if (m_size == m_capacity) {
                    m_capacity *= m_multiplicator;
                }
                // выделяем новый массив
                T *temp = new T[m_capacity]; // 
                T *thisElem = this->m_begin;
                T *tempElem = temp;
                T *new_pos = nullptr;
                while (thisElem != m_end) {
                    if (pos.get() == thisElem) {
                        *tempElem = value;
                        new_pos = tempElem;
                        ++tempElem;
                    }
                    *tempElem = *thisElem;
                    ++tempElem;
                    ++thisElem;
                }
                delete_elements();
                ++m_size;
                // присваиваем новые указатели
                this->m_begin = temp;           
                this->m_end = temp + m_size;
                this->m_end_capacity = temp + m_capacity;
                return iterator(new_pos);
            }
        }
        return iterator(nullptr);
    }


    iterator erase(iterator pos) {
        // проверка на корректность переданного итератора
        if (pos.get() >= m_begin && pos.get() < m_end) {

            if (pos.get() == (m_end - 1)) {
                --m_end;
                --m_size;
                return iterator(m_end);
            } else {
                // выделяем новый массив
                T *temp = new T[m_capacity]; 
                T *thisElem = this->m_begin;
                T *tempElem = temp;
                T *new_pos = nullptr;
                while (thisElem != m_end) {
                    if (thisElem == pos.get()) {
                        ++thisElem;
                        *tempElem = *thisElem;
                        new_pos = tempElem;
                    } else {
                        *tempElem = *thisElem;
                    }
                    ++tempElem;
                    ++thisElem;
                }
                delete_elements();
                --m_size;
                // присваиваем новые указатели
                this->m_begin = temp;           
                this->m_end = temp + m_size;
                this->m_end_capacity = temp + m_capacity;
                return iterator(new_pos);
            }
        }
        return iterator(nullptr);
    }


    T &operator[](size_t index) { // проверяем на корректность переданного индекса элемента
        if (index < m_size) {
            return *(m_begin + index);
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта Vector"));
    }

    iterator begin() { return iterator(m_begin); }

    iterator end() { return iterator(m_end); }

    const_iterator begin() const { return const_iterator(m_begin); }

    const_iterator end() const { return const_iterator(m_end); }

private:
    const size_t m_min_elements = 4;
    const double m_multiplicator = 1.5;
    T *m_begin = nullptr;
    T *m_end = nullptr;
    T *m_end_capacity = nullptr;
    size_t m_size{};
    size_t m_capacity{};

private:
    void copy_elements_from(const Vector<T> &other) { // копируем элементы из другого объекта
        T *otherElem = other.m_begin;
        T *thisElem = this->m_begin;
        this->m_size = 0;
        while (otherElem != other.m_end) {
            *thisElem = *otherElem;
            ++otherElem;
            ++thisElem;
            ++(this->m_size);
        }
        this->m_end = thisElem;
        this->m_end_capacity = m_begin + m_capacity;
    }

    void copy_elements_to(T *temp) { // копируем элементы в temp
        T *thisElem = this->m_begin;
        while (thisElem != m_end) {
            *temp = *thisElem;
            ++temp;
            ++thisElem;
        }
    }

    void delete_elements() {        // удаляем элементы
        delete[] m_begin;
        m_begin = nullptr;
        m_end = nullptr;
        m_end_capacity = nullptr;
    }

};

template<typename T>
class VecIterator {

public:
    explicit VecIterator(T *p) : p(p) {}

    VecIterator(const VecIterator &it) : p(it.p) {}

    bool operator!=(VecIterator const &other) const {
        return p != other.p;
    }

    bool operator==(VecIterator const &other) const {
        return p == other.p;
    }

    T *get() { return p; }

    T &operator*() { return *p; }

    VecIterator<T> &operator++() {
        ++p;
        return *this;
    }

    VecIterator<T> &operator--() {
        --p;
        return *this;
    }

    VecIterator<T> &operator+(int i) {
        p = p + i;
        return *this;
    }

    VecIterator<T> &operator-(int i) {
        p = p - i;
        return *this;
    }

private:
    T *p;
};

template<typename T>
inline void print(const Vector<T> &vec) {
    for (auto elem: vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

#endif
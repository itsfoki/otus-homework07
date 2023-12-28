#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <iostream>

template<typename T>
class List {

    template<typename U>
    struct Node {

        U data{}; // хранимые данные
        Node<U> *prev = nullptr; // указатель на предыдущий элемент Node
        Node<U> *next = nullptr; // указатель на следующий элемент Node

        explicit Node(const U &value, Node<U> *prev = nullptr, Node<U> *next = nullptr) :
                data(value), prev(prev), next(next) {}
    };

public:
    template<typename V>
    class ListIterator {
        friend class List<V>;

    public:
        ListIterator() : pNode(new Node<T>{}) {}

        explicit ListIterator(Node<T> *p) : pNode(p) {}

        V &operator*() { return static_cast<Node<T> *>(pNode)->data; }

        V *operator->() { return &static_cast<Node<T> *>(pNode)->data; }

        V &get() { return *static_cast<Node<T> *>(pNode)->data; }

        bool operator!=(ListIterator const &other) const { return pNode != other.pNode; }

        bool operator!=(std::nullptr_t const &) const { return pNode != nullptr; }

        bool operator==(ListIterator const &other) const { return pNode == other.pNode; }

        ListIterator<V> &operator++() {
            pNode = pNode->next;
            return *this;
        }

        ListIterator<V> &operator--() {
            pNode = pNode->prev;
            return *this;
        }

        ListIterator<V> &operator+(int i) {
            while (i != 0) {
                if (pNode->next != nullptr) pNode = pNode->next;
                --i;
            }
            return *this;
        }

        ListIterator<V> &operator-(int i) {
            while (i != 0) {
                if (pNode->prev != nullptr) pNode = pNode->prev;
                --i;
            }
            return *this;
        }

    private:
        Node<T> *pNode;

        Node<T> *node() {
            return pNode;
        }
    };

    typedef ListIterator<T> iterator;
    typedef ListIterator<const T> const_iterator;

private:
    Node<T> *m_begin = nullptr;
    Node<T> *m_end = nullptr;
    size_t m_size{};

public:
    List() { // конструктор без параметров
        m_begin = nullptr;
        m_end = m_begin;
        m_size = 0;
    }

    List(const List<T> &other) { // конструктор копирования
        copy_elements_from(other);
    }

    List(const List<T> &&other)  noexcept { // конструктор перемещения
        delete_elements();
        // присваиваем указатели
        this->m_begin = other.m_begin;
        this->m_end = other.m_end;
        this->m_size = other.m_size;
    }

    ~List() { // деструктор
        delete_elements();
    }


    List<T> &operator=(const List<T> &other) { // оператор присваивания копированиемс
        // проверка и очистка
        if (other.m_begin == this->m_begin) { return *this; }
        delete_elements();
        copy_elements_from(other);
        return *this;
    }

    List<T> &operator=(List<T> &&other) noexcept { // оператор присваивания перемещением
        // проверка
        if (other.m_begin == this->m_begin) { return *this; }
        delete_elements();
        // присваиваем указатели и поля
        this->m_begin = other.m_begin;
        this->m_end = other.m_end;
        this->m_size = other.m_size;
        other.m_begin = nullptr;
        other.m_end = nullptr;
        other.m_size = 0;
        return *this;
    }

    [[nodiscard]] size_t size() const { return m_size; }

    void push_back(const T &value) {     // добавление элемента в конец листа
        auto *newNode = new Node<T>(value);    // создаём новую ноду
        if (m_size == 0) {
            this->m_begin = newNode;
            auto *endNode = new Node<T>({});    // создаём концевую ноду
            this->m_end = endNode;
            this->m_end->prev = m_begin;
            this->m_begin->next = m_end;
            this->m_size++;
        } else {
            (m_end->prev)->next = newNode; // в предпоследнюю ноду записываем новый конец
            newNode->prev = m_end->prev;    // в новую ноду записываем предпоследний элемент
            newNode->next = m_end;      // в последнюю ячейку присваиваем предыдущий конец
            m_end->prev = newNode;      // в конец записываем новую ноду
            this->m_size++;
        }
    }

    void push_front(const T &value) {           // добавление элемента в конец вектора
        auto *newNode = new Node<T>(value);    // создаём новую ноду
        if (m_size == 0) {
            this->m_begin = newNode;
            auto *endNode = new Node<T>({});    // создаём концевую ноду
            this->m_end = endNode;
            this->m_end->prev = m_begin;
            this->m_begin->next = m_end;
            this->m_size++;
        } else {
            newNode->next = m_begin;      // в первую ячейку присваиваем предыдущее начало
            m_begin->prev = newNode;      // в предыдущее начало присваиваем текущую ноду
            this->m_begin = newNode;      // в начало листа присваиваем адрес новой ноды
            this->m_size++;
        }
    }

    void pop_back() {       // удаление элемента из конца вектора
        if (m_end->prev != m_begin) {
            // находим предпоследний элемент
            Node<T> *pprev = (m_end->prev)->prev;
            // присваиваем новые указатели в обход удаляемого элемента
            pprev->next = m_end;
            // удаляем старый элемент
            delete m_end->prev;
            m_end->prev = pprev;
            --m_size;
        } else {
            delete m_end;
            delete m_begin;
            this->m_end = nullptr;    // указатели null
            this->m_begin = nullptr;
            --m_size;
        }
    }

    void pop_front() {     //  удаление элемента из начала списка
        if (m_begin->next != nullptr) { // если удаляем не последний элемент
            Node<T> *new_begin = m_begin->next;
            delete this->m_begin;
            this->m_begin = new_begin;    // присваиваем новые указатели
            m_begin->prev = nullptr;
            --m_size;
        } else {
            delete this->m_begin;
            this->m_end = nullptr;    // указатели null
            this->m_begin = nullptr;
            --m_size;
        }
    }

    T &operator[](size_t index) {
        // проверка
        if (index < m_size) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта List"));
    }

    T operator[](size_t index) const {
        // проверка
        if (index < m_size) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта List"));
    }

    iterator begin() { return iterator(m_begin); }

    iterator end() { return iterator(m_end); }

    const_iterator begin() const { return const_iterator(m_begin); }

    const_iterator end() const { return const_iterator(m_end); }

    iterator erase(iterator pos) {
        
        if (pos == begin()) { // удаляем из начала
            pop_front();
            return begin();
        } else if (pos == end()) { // удаляем из конца
            pop_back();
            return end();
        } else {
            // находим предыдущий и следующий элементы
            Node<T> *prev = pos.node()->prev;
            Node<T> *next = pos.node()->next;
            // присваиваем новые указатели
            prev->next = next;
            next->prev = prev;
            --m_size;
            delete pos.node();
            return iterator(next);
        }
    }

    iterator insert(iterator pos, const T &value) {
        
        if (pos == begin()) { // вставляем в начало
            push_front(value);
        } else if (pos == end()) { // вставляем в конец
            push_back(value);
        } else {
            // Находим предыдущий и следующий элементы
            Node<T> *prev = pos.node()->prev;
            Node<T> *next = pos.node();
            // Присваиваем новые указатели
            auto temp = new Node<T>(value);
            prev->next = temp;
            next->prev = temp;
            temp->prev = prev;
            temp->next = next;
            ++m_size;
        }
        return pos;
    }

private:
    void copy_elements_from(const List<T> &other) { // копируем элементы
        for(auto otherElem : other){
            push_back(otherElem);
        }
    }

    
    void delete_elements() { // удаляем элементы
        while (m_size > 0) {
            pop_back();
        }
    }

};

template<typename T>
inline void print(const List<T> &list) {
    for (auto elem: list) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

#endif
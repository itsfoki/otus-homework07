#ifndef REDIRECT_H
#define REDIRECT_H

#include <cstddef>
#include <iostream>


template<typename T>
class Redirect {

    template<typename U>
    struct Node {

        U data{}; // хранимые данные
        Node<U> *next = nullptr; // указатель на след. элемент Node

        explicit Node(const U &value, Node<U> *next = nullptr) :
                data(value), next(next) {}
    };

public:
    template<typename V>
    class RedirectIterator {
        friend class Redirect<V>;

    public:
        RedirectIterator() : pNode(new Node<T>{}) {}

        explicit RedirectIterator(Node<T> *p) : pNode(p) {}

        V &operator*() { return static_cast<Node<T> *>(pNode)->data; }

        V *operator->() { return static_cast<Node<T> *>(pNode); }

        V &get() { return *static_cast<Node<T> *>(pNode)->data; }

        bool operator!=(RedirectIterator const &other) const { return pNode != other.pNode; }

        bool operator!=(std::nullptr_t const &) const { return pNode != nullptr; }

        bool operator==(RedirectIterator const &other) const { return pNode == other.pNode; }

        RedirectIterator<V> &operator++() {
            pNode = pNode->next;
            return *this;
        }

        RedirectIterator<V> &operator+(int i) {
            while (i != 0) {
                if( pNode->next != nullptr ) pNode = pNode->next;
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

    typedef RedirectIterator<T> iterator;
    typedef RedirectIterator<const T> const_iterator;

private:
    Node<T> *m_begin = nullptr;
    size_t m_size{};

public:
    
    Redirect() { // конструктор без параметров
        m_begin = nullptr;
        m_size = 0;
    }

    Redirect(const Redirect<T> &&other) noexcept { // конструктор перемещения
        delete_elements();
        this->m_begin = other.m_begin;
        this->m_size = other.m_size;
    }

    Redirect(const Redirect<T> &other) { // конструктор копирования
        copy_elements_from(other);
    }


    ~Redirect() { // Деструктор
        delete_elements();
    }
    
    Redirect<T> &operator=(const Redirect<T> &other) { // оператор присваивания копированием
        // Проверка
        if( other.m_begin == this->m_begin ) { return *this; }
        delete_elements();
        copy_elements_from(other);
        return *this;
    }

    Redirect<T> &operator=(Redirect<T> &&other) noexcept { // оператор присваивания перемещением
        if( other.m_begin == this->m_begin ) { return *this; }
        delete_elements();
        this->m_begin = other.m_begin;
        this->m_size = other.m_size;
        other.m_begin = nullptr;
        other.m_size = 0;
        return *this;
    }

    [[nodiscard]] size_t size() const { return m_size; }
    
    void push_back(const T &value) { // добавление элемента в конец вектора
        auto *newNode = new Node<T>(value);    // создаём новую ноду
        if( m_size == 0 ) {
            this->m_begin = newNode;
            auto *endNode = new Node<T>({});    // создаём концевую ноду
            this->m_begin->next = endNode;
            this->m_size++;
        } else {
            Node<T> *current = this->m_begin;
            Node<T> *last = this->m_begin;
            while (current->next != nullptr) {
                last = current;
                current = current->next;
            }
            newNode->next = current;
            last->next = newNode;      // в предыдущий конец присваиваем текущую ноду
            this->m_size++;
        }
    }

    void push_front(const T &value) { // добавление элемента в конец вектора
        auto *newNode = new Node<T>(value);    // создаём новую ноду
        if( m_size == 0 ) {
            this->m_begin = newNode;
            auto *endNode = new Node<T>({});    // создаём концевую ноду
            this->m_begin->next = endNode;
            this->m_size++;
        } else {
            newNode->next = m_begin;      // в первую ячейку присваиваем предыдущее начало
            this->m_begin = newNode;      // в начало листа присваиваем адрес новой ноды
            this->m_size++;
        }
    }

    void pop_back() { // удаление элемента из конца списка
        Node<T> *current = this->m_begin;
        Node<T> *prev = this->m_begin;
        Node<T> *pprev = this->m_begin;
        while (current->next != nullptr) {
            pprev = prev;
            prev = current;
            current = current->next;
        }
        // если удаляем не последний элемент
        if( prev != m_begin ) { 
            delete prev;
            pprev->next = current;    // присваиваем новые указатели
            --m_size;
        } else {  
            // если удаляем последний элемент  
            delete prev;
            delete current;
            this->m_begin = nullptr;
            --m_size;
        }
    }
    
    void pop_front() {                // удаление элемента из начала списка
        if( m_begin != nullptr ) {    // если удаляем не последний элемент
            Node<T> *new_begin = m_begin->next;
            delete m_begin;
            m_begin = new_begin;      // присваиваем новые указатели
            --m_size;
        }
    }

    T &operator[](size_t index) {
        // проверяем на корректность передачи индекса
        if( index < m_size ) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта Redirect"));
    }

    T operator[](size_t index) const {
        // проверяем на корректность передачи индекса
        if( index < m_size ) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта Redirect"));
    }

    iterator begin() { return iterator(m_begin); }

    iterator end() {
        Node<T> *current = this->m_begin;
        if( current ) {
            while (current->next != nullptr) { current = current->next; }
            return iterator(current);
        }
        return iterator(nullptr);
    }

    const_iterator begin() const { return const_iterator(m_begin); }

    const_iterator end() const {
        Node<T> *current = this->m_begin;
        if( current ) {
            while (current->next != nullptr) { current = current->next; }
            return const_iterator(current);
        }
        return const_iterator(nullptr);
    }

    iterator erase(iterator pos) {
        if( pos == begin()) {  // удаляем из начала
            pop_front();
            return begin();
        } else if( pos == end()) { // удаляем из конца
            pop_back();
            return end();
        } else {
            // находим предыдущий и следующий элементы
            Node<T> *current = this->m_begin;
            Node<T> *prev = this->m_begin;
            while (current != pos.node()) {
                prev = current;
                current = current->next;
            }
            Node<T> *next = pos.node()->next; // присваиваем новые указатели
            prev->next = next;
            --m_size;
            delete pos.node(); // очищаем старые
            return iterator(next);
        }
    }

    iterator insert(iterator pos, const T &value) {
        
        if( pos == begin()) { // вставляем в начало
            push_front(value);
        } else if( pos == end()) { // вставляем  в конец
            push_back(value);
        } else { // находим предыдущий и следующий элементы
            Node<T> *current = this->m_begin;
            Node<T> *prev = this->m_begin;
            while (current != pos.node()) {
                prev = current;
                current = current->next;
            }
            Node<T> *next = pos.node();
            auto temp = new Node<T>(value); // присваиваем новые указатели
            prev->next = temp;
            temp->next = next;
            ++m_size;
        }
        return pos;
    }

private:
    void copy_elements_from(const Redirect<T> &other) { // копируем элементы
        for (auto otherElem: other) {
            push_back(otherElem);
        }
    }

    void delete_elements() { // удаляем элементы
        while (m_size > 0) {
            pop_front();
        }
    }

};

template<typename T>
inline void print(const Redirect<T> &list) {
    for (auto elem: list) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

#endif
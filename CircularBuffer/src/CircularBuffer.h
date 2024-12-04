#pragma once

typedef char valueType;

class CircularBuffer {
private:
    valueType *buffer;        // array of elements in the buffer
    int head;                // index of the first element
    int tail;                // index of the last element
    int bufferSize;        // number of elements
    int bufferCapacity;    // max number of elements

public:
    CircularBuffer();

    CircularBuffer(const CircularBuffer &cb);

    //Конструирует буфер заданной ёмкости.
    explicit CircularBuffer(int capacity);

    CircularBuffer(CircularBuffer &&cb) noexcept;

    //Конструирует буфер заданной ёмкости, целиком заполняет его элементом elem.
    CircularBuffer(int capacity, const valueType &elem);

    ~CircularBuffer();


    //Доступ по индексу. Не проверяют правильность индекса.
    valueType &operator[](int i);

    const valueType &operator[](int i) const;

    //Доступ по индексу. Методы бросают исключение в случае неверного индекса.
    valueType &at(int i);

    const valueType &at(int i) const;

    valueType &front(); //Ссылка на первый элемент.
    valueType &back();  //Ссылка на последний элемент.
    const valueType &front() const;

    const valueType &back() const;

    //Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
    //переместится в начало аллоцированной памяти. Возвращает указатель
    //на первый элемент.
    valueType *linearize();

    //Проверяет, является ли буфер линеаризованным.
    bool isLinearized() const;

    //Сдвигает буфер так, что по нулевому индексу окажется элемент
    //с индексом newBegin.
    void rotate(int newBegin);

    //Количество элементов, хранящихся в буфере.
    int size() const;

    bool empty() const;

    //true, если size() == capacity().
    bool full() const;

    //Количество свободных ячеек в буфере.
    int reserve() const;

    //ёмкость буфера
    int capacity() const;

    void setCapacity(int newCapacity);

    //Изменяет размер буфера.
    //В случае расширения, новые элементы заполняются элементом item.
    void resize(int newSize, const valueType &item);

    //Оператор присваивания.
    CircularBuffer &operator=(const CircularBuffer &cb);

    //Обменивает содержимое буфера с буфером cb.
    void swap(CircularBuffer &cb);

    //Добавляет элемент в конец буфера.
    //Если текущий размер буфера равен его ёмкости, то переписывается
    //первый элемент буфера (т.е., буфер закольцован).
    void pushBack(const valueType &item);

    //Добавляет новый элемент перед первым элементом буфера.
    //Аналогично pushBack, может переписать последний элемент буфера.
    void pushFront(const valueType &item);

    //удаляет последний элемент буфера.
    void popBack();

    //удаляет первый элемент буфера.
    void popFront();

    //Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной.
    void insert(int pos, const valueType &item);

    //Удаляет элементы из буфера в интервале [first, last).
    void erase(int first, int last);

    //Очищает буфер.
    void clear();

    friend bool operator==(const CircularBuffer &a, const CircularBuffer &b);

    friend bool operator!=(const CircularBuffer &a, const CircularBuffer &b);
};

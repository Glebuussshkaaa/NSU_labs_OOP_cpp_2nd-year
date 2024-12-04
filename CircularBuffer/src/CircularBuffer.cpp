#include "CircularBuffer.h"
#include <iostream>
#include <algorithm>

// default constructor
CircularBuffer::CircularBuffer() :
        buffer(nullptr),
        head(0),
        tail(0),
        bufferSize(0),
        bufferCapacity(0) {}

// default destructor
CircularBuffer::~CircularBuffer() {
    delete[] buffer;
}

// copy constructor
CircularBuffer::CircularBuffer(const CircularBuffer &cb) :
        buffer(new valueType[cb.bufferCapacity]),
        head(cb.head),
        tail(cb.tail),
        bufferSize(cb.bufferSize),
        bufferCapacity(cb.bufferCapacity) {
    std::copy(cb.buffer, cb.buffer + cb.bufferSize, buffer);
}

// Constructor with specified capacity
CircularBuffer::CircularBuffer(int capacity) :
        buffer(new valueType[capacity]),
        head(0),
        tail(0),
        bufferSize(0),
        bufferCapacity(capacity) {}

// Move-constructor
CircularBuffer::CircularBuffer(CircularBuffer &&cb) noexcept:
        buffer(std::exchange(cb.buffer, nullptr)),
        head(std::exchange(cb.head, 0)),
        tail(std::exchange(cb.tail, 0)),
        bufferSize(std::exchange(cb.bufferSize, 0)),
        bufferCapacity(std::exchange(cb.bufferCapacity, 0)) {}

// Constructor with specified capacity and element to fill the buffer
CircularBuffer::CircularBuffer(int capacity, const valueType &elem) :
        buffer(new valueType[capacity]),
        head(0),
        tail(0),
        bufferSize(capacity),
        bufferCapacity(capacity) {
    std::fill(buffer, buffer + capacity, elem);
}

//Доступ по индексу. Не проверяют правильность индекса.
valueType &CircularBuffer::operator[](int i) {
    return buffer[(head + i) % bufferCapacity];
}

const valueType &CircularBuffer::operator[](int i) const {
    return buffer[(head + i) % bufferCapacity];
}


//Доступ по индексу. Методы бросают исключение в случае неверного индекса.
valueType &CircularBuffer::at(int i) {
    if (i < 0 || i >= bufferSize) {
        throw std::out_of_range("Index out of range: " + std::to_string(i));
    }

    return buffer[(head + i) % bufferCapacity];
}

const valueType &CircularBuffer::at(int i) const {
    if (i < 0 || i >= bufferSize) {
        throw std::out_of_range("Index out of range: " + std::to_string(i));
    }

    return buffer[(head + i) % bufferCapacity];
}

//Ссылка на первый элемент.
valueType &CircularBuffer::front() {
    if (empty()) {
        throw std::logic_error("Buffer is empty");
    }

    return buffer[head];
}

//Ссылка на последний элемент.
valueType &CircularBuffer::back() {
    if (empty()) {
        throw std::logic_error("Buffer is empty");
    }

    return buffer[(tail - 1 + bufferCapacity) % bufferCapacity];
}

const valueType &CircularBuffer::front() const {
    if (empty()) {
        throw std::logic_error("Buffer is empty");
    }
    return buffer[head];
}

const valueType &CircularBuffer::back() const {
    if (empty()) {
        throw std::logic_error("Buffer is empty");
    }

    return buffer[(tail - 1 + bufferCapacity) % bufferCapacity];
}

//Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
//переместится в начало аллоцированной памяти. Возвращает указатель
//на первый элемент.
valueType *CircularBuffer::linearize() {
    if (!isLinearized()) {
        auto *linearBuffer = new valueType[bufferCapacity];
        for (int i = 0; i < bufferSize; ++i) {
            linearBuffer[i] = buffer[(head + i) % bufferCapacity];
        }

        std::swap(buffer, linearBuffer);
        delete[] linearBuffer;

        head = 0;
        tail = bufferSize;
    }

    return buffer;
}

//Проверяет, является ли буфер линеаризованным.
bool CircularBuffer::isLinearized() const {
    return head == 0;
}

//Сдвигает буфер так, что по нулевому индексу окажется элемент с индексом newBegin.
void CircularBuffer::rotate(int newBegin) {
    if (bufferCapacity == 0) {
        throw std::logic_error("Buffer capacity is zero");
    }

    if (newBegin < 0 || newBegin >= bufferSize) {
        throw std::out_of_range("NewBegin index out of range");
    }

    auto *newBuffer = new valueType[bufferCapacity];
    for (int i = 0; i < bufferSize; ++i) {
        int oldIndex = (newBegin + i) % bufferSize;
        newBuffer[i] = buffer[oldIndex];
    }

    std::swap(buffer, newBuffer);
    delete[] newBuffer;

    head = 0;
    tail = bufferSize;
}

//Количество элементов, хранящихся в буфере.
int CircularBuffer::size() const {
    return bufferSize;
}

bool CircularBuffer::empty() const {
    return bufferSize == 0;
}

//true, если size() == capacity().
bool CircularBuffer::full() const {
    return bufferSize == bufferCapacity;
}

//Количество свободных ячеек в буфере.
int CircularBuffer::reserve() const {
    return bufferCapacity - bufferSize;
}

//ёмкость буфера
int CircularBuffer::capacity() const {
    return bufferCapacity;
}

void CircularBuffer::setCapacity(int newCapacity) {
    if (newCapacity < bufferSize) {
        throw std::invalid_argument("New capacity is less than current size");
    }

    auto newBuffer = std::make_unique<char[]>(newCapacity);
    for (int i = 0; i < bufferSize; ++i) {
        newBuffer[i] = buffer[(head + i) % bufferCapacity];
    }

    auto tmp = buffer;
    buffer = newBuffer.release();
    delete[] tmp;

    head = 0;
    tail = bufferSize;
    bufferCapacity = newCapacity;
}

//Изменяет размер буфера.
//В случае расширения, новые элементы заполняются элементом item.
void CircularBuffer::resize(int newSize, const valueType &item) { // проверить можно ли вызывать функцию без valueType
    if (newSize > bufferCapacity) {
        throw std::invalid_argument("New size is greater than capacity");
    }

    if (newSize < bufferSize) {
        throw std::invalid_argument("New size is less than current size");
    }

    int deltaSize = newSize - bufferSize;
    if (newSize > bufferSize) {
        for (int i = 0; i < deltaSize; ++i) {
            buffer[(tail + i) % bufferCapacity] = item;
        }
    }

    bufferSize = newSize;
    tail = (tail + deltaSize) % bufferSize;
}

//Оператор присваивания.
CircularBuffer &CircularBuffer::operator=(const CircularBuffer &cb) {
    if (this != &cb) {
        CircularBuffer temp(cb);
        swap(temp);
    }
    return *this;
}

//Обменивает содержимое буфера с буфером cb.
void CircularBuffer::swap(CircularBuffer &cb) {
    std::swap(buffer, cb.buffer);
    std::swap(head, cb.head);
    std::swap(tail, cb.tail);
    std::swap(bufferSize, cb.bufferSize);
    std::swap(bufferCapacity, cb.bufferCapacity);
}

//Добавляет элемент в конец буфера.
//Если текущий размер буфера равен его ёмкости, то вызывает ошибку чтобы не терять первый элемент
void CircularBuffer::pushBack(const valueType &item) {
    if (full()) {
        throw std::overflow_error("Buffer is full. Cannot add new element at the back.");
    }

    buffer[tail] = item;
    tail = (tail + 1) % bufferCapacity;
    bufferSize++;
}

//Добавляет новый элемент перед первым элементом буфера.
//Аналогично pushBack, вызывает ошибку, чтобы не стирать первый элемент
void CircularBuffer::pushFront(const valueType &item) {
    if (full()) {
        throw std::overflow_error("Buffer is full. Cannot add new element at the front.");
    }

    head = (head + bufferCapacity - 1) % bufferCapacity;
    buffer[head] = item;
    bufferSize++;
}

//удаляет последний элемент буфера.
void CircularBuffer::popBack() {
    if (empty()) {
        throw std::underflow_error("Buffer is empty. Cannot remove element from the back.");
    }
    // добавить
    tail = (tail + bufferCapacity - 1) % bufferCapacity;
    bufferSize--;
}

//удаляет первый элемент буфера.
void CircularBuffer::popFront() {
    if (empty()) {
        throw std::underflow_error("Buffer is empty. Cannot remove element from the front.");
    }

    head = (head + 1) % bufferCapacity;
    bufferSize--;
}

//Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной.
void CircularBuffer::insert(int pos, const valueType &item) {
    if (pos < 0 || pos > bufferSize) {
        throw std::out_of_range("Insert position is out of range");
    }

    if (full()) {
        throw std::overflow_error("Buffer is full. Cannot insert new element.");
    }

    if (pos < bufferSize / 2) {
        head = (head - 1 + bufferCapacity) % bufferCapacity;
        for (int i = 0; i < pos; ++i) {
            int from = (head + i + 1) % bufferCapacity;
            int to = (head + i) % bufferCapacity;
            buffer[to] = buffer[from];
        }
    } else {
        tail = (tail + 1) % bufferCapacity;
        for (int i = bufferSize - 1; i >= pos; --i) {
            int from = (head + i) % bufferCapacity;
            int to = (head + i + 1) % bufferCapacity;
            buffer[to] = buffer[from];
        }
    }

    int insertIndex = (head + pos) % bufferCapacity;
    buffer[insertIndex] = item;

    bufferSize++;
}


//Удаляет элементы из буфера в интервале [first, last).
void CircularBuffer::erase(int first, int last) {
    if (first < 0 || last > bufferSize) {
        throw std::out_of_range("Erase range is invalid");
    }

    if (first >= last) {
        throw std::invalid_argument("first erase index >= last erase index");
    }

    int eraseCount = last - first;

    for (int i = last; i < bufferSize; ++i) {
        buffer[(head + i - eraseCount) % bufferCapacity] = buffer[(head + i) % bufferCapacity];
    }

    bufferSize -= eraseCount;
    tail = (head + bufferSize) % bufferCapacity;
}

//Очищает буфер.
void CircularBuffer::clear() {
    head = 0;
    tail = 0;
    bufferSize = 0;
}

bool operator==(const CircularBuffer &a, const CircularBuffer &b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const CircularBuffer &a, const CircularBuffer &b) {
    return !(a == b);
}



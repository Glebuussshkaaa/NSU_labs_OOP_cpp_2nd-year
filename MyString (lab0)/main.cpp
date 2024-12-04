#include <iostream>

class MyString {
private:
    char *data;
    std::size_t length;
public:
    explicit MyString(const char *str = "") {
        if (str) {
            length = strlen(str);
            data = new char[length + 1];
            for (int i = 0; i < length; i++) {
                data[i] = str[i];
            }
            data[length] = '\0';
        } else {
            length = 0;
            data = new char[length + 1];
            data[length] = '\0';
        }
    }

    MyString(const MyString &other) {
        length = other.getSize();
        data = new char[other.getSize() + 1];
        std::memcpy(data, other.data, length);
    }

    std::size_t getSize() const {
        return length;
    }

    const MyString &operator+=(const MyString &newStr) {
        char *newData = new char[length + newStr.getSize() + 1];
        for (std::size_t i = 0; i < length; i++) {
            newData[i] = data[i];
        }

        for (std::size_t i = 0; i < newStr.getSize(); i++) {
            newData[length + i] = newStr.data[i];
        }

        newData[length + newStr.getSize()] = '\0';

        delete[] data;
        data = newData;
        length += newStr.getSize();

        return *this;

    }

    MyString &operator=(const MyString &other) {
        if (this != &other) {
            delete[] data;
            length = other.getSize();
            data = new char[length + 1];
            memcpy(data, other.data, length);

        }
        return *this;
    }

    const char *getData() const {
        return data;
    }

    ~MyString() {
        delete[] data;
    }
};


std::ostream &operator<<(std::ostream &os, const MyString &str) {
    return os << str.getData();
}

int main() {
    MyString str1("Hello ");
    MyString str2("World!");

    MyString str3 = str2;

    str1 += str2;
    str3 = str1;

    std::cout << str3 << std::endl;
    std::cout << str1 << std::endl;

    return 0;
}

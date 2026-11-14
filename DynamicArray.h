#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>

// ---------------------------------------------------------------------
// 1. RECORD TYPE
// ---------------------------------------------------------------------
struct StudentRecord {
    int    id;
    std::string name;
    std::string course;
    double score;
};

// ---------------------------------------------------------------------
// 2. PRIMARY DATA STRUCTURE (A1)
// ---------------------------------------------------------------------
template <typename T>
class DynamicArray {
private:
    T*      data;
    int     count;
    int     capacity;

    void grow() {
        int newCap = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[newCap];
        for (int i = 0; i < count; ++i)
            newData[i] = data[i];
        delete[] data;
        data     = newData;
        capacity = newCap;
    }

public:
    DynamicArray() : data(nullptr), count(0), capacity(0) {}

    DynamicArray(const DynamicArray& other)
        : data(nullptr), count(other.count), capacity(other.capacity) {
        if (capacity > 0) {
            data = new T[capacity];
            for (int i = 0; i < count; ++i)
                data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        T* newData = nullptr;
        if (other.capacity > 0) {
            newData = new T[other.capacity];
            for (int i = 0; i < other.count; ++i)
                newData[i] = other.data[i];
        }
        delete[] data;
        data     = newData;
        count    = other.count;
        capacity = other.capacity;
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
        data = nullptr;
    }

    void push_back(const T& value) {
        if (count == capacity) grow();
        data[count++] = value;
    }

    void removeAt(int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("removeAt: index out of range");
        for (int i = index; i < count - 1; ++i)
            data[i] = data[i + 1];
        --count;
        if (capacity > 4 && count < capacity / 4) {
            int newCap = capacity / 2;
            T* newData = new T[newCap];
            for (int i = 0; i < count; ++i) newData[i] = data[i];
            delete[] data;
            data = newData;
            capacity = newCap;
        }
    }

    T& at(int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("at: index out of range");
        return data[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("at: index out of range");
        return data[index];
    }

    int size() const { return count; }
    int getCapacity() const { return capacity; }
    bool empty() const { return count == 0; }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("cannot open input file: " + filename);
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::istringstream ss(line);
            std::string idStr, name, course, scoreStr;
            if (!std::getline(ss, idStr,   ',')) continue;
            if (!std::getline(ss, name,    ',')) continue;
            if (!std::getline(ss, course,  ',')) continue;
            if (!std::getline(ss, scoreStr,',')) continue;
            StudentRecord r;
            r.id     = std::stoi(idStr);
            r.name   = name;
            r.course = course;
            r.score  = std::stod(scoreStr);
            push_back(r);
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("cannot open output file: " + filename);
        for (int i = 0; i < count; ++i) {
            out << data[i].id << ',' << data[i].name << ','
                << data[i].course << ',' << data[i].score << '\n';
        }
    }

    T*       raw()       { return data; }
    const T* raw() const { return data; }
};

#endif // DYNAMIC_ARRAY_H
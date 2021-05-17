// running the program via: g++ -std=gnu++17 vector.cpp -o run
#include <errno.h>
#include <initializer_list>
#include <iostream>

class Vector {
    int size_;      // current size
    int capacity_;  // total memory is available
    int *body_;

    void Realloc() {
        if (capacity_ == 0) {
            body_ = new int[1];
            capacity_ = 1;
        }

        capacity_ *= 2;
        int *tmp = new int[capacity_];
        for (int i = 0; i < size_; ++i) {
            tmp[i] = body_[i];
        }

        delete[] body_;
        body_ = tmp;
    }

public:
    Vector() : size_(0), capacity_(0), body_(nullptr) { std::cout << "default" << std::endl; }

    Vector(int sz) {
        if (sz < 0) {
            std::cout << "Error: vector size < 0" << std::endl;
        } else {
            body_ = new int[sz];
            size_ = sz;
            capacity_ = size_;
        }
        std::cout << "def_size = " << size_ << " def_capacity = " << capacity_ << std::endl;
    }

    Vector(int sz, int val) {
        if (sz < 0) {
            std::cout << "Error: vector size < 0" << std::endl;
        } else {
            body_ = new int[sz];
            size_ = sz;
            capacity_ = size_;

            for (int i = 0; i < size_; ++i) {
                body_[i] = val;
            }
        }
        std::cout << "def_size = " << size_ << " def_capacity = " << capacity_
                  << " def_val = " << val << std::endl;
    }

    Vector(const Vector &copy_vector) {
        size_ = copy_vector.size_;
        capacity_ = copy_vector.capacity_;
        body_ = new int[capacity_];

        for (int i = 0; i < size_; ++i) {
            body_[i] = copy_vector.body_[i];
        }
    }

    // initializer_list - template
    Vector(const std::initializer_list<int> &lst) : Vector(lst.size()) {
        auto it = lst.begin();

        for (int i = 0; i < lst.size(); ++i, ++it) {
            body_[i] = *it;
        }
    }

    ~Vector() {
        delete[] body_;
        std::cout << "Destructor" << std::endl;
    }

    int &operator[](int index) {
        if (index >= size_ || index < 0) {
            std::cout << "Error: out of range" << std::endl;
        }
        return body_[index];
    }

    const int &operator[](int index) const {
        if (index >= size_ || index < 0) {
            std::cout << "Error: out of range" << std::endl;
        }
        return body_[index];
    }

    Vector &operator=(const Vector &rhs) {  // rhs = right hand side
        if (this == &rhs) {
            std::cout << "Same vector\n";
            return *this;
        }

        delete[] body_;  // pre-delete the allocated memory

        body_ = new int[rhs.size_];
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;

        for (int i = 0; i < size_; ++i) {
            body_[i] = rhs.body_[i];
        }

        return *this;
    }

    int get_size() const { return size_; }  // return this -> size_;

    void push_back(int new_elem) {
        if (size_ == capacity_) {
            Realloc();
        }
        body_[size_] = new_elem;
        size_++;
    }

    int pop_back() {
        if (size_ > 0) {
            size_--;
            return body_[size_];
        } else {
            std::cout << "Error: empty vector returned 0" << std::endl;
            return 0;
        }
    }
};

void print_vector(const Vector &v) {
    for (int i = 0; i < v.get_size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    Vector v4 = {1, 4, 2, 2};
    Vector v1;

    v1 = v4;
    v1.push_back(5);
    print_vector(v1);
    std::cout << v1.pop_back() << std::endl;
    return 0;
}

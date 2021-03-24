#include <iostream>

class BadRange {
private:
    int index;
public:
    BadRange(int i) : index(i) {}
    const int get() {
        return index;
    }
    ~BadRange() {}
};

class SpecialInts {
    int size_;
    int capacity_;
    int *body_;
public:
    SpecialInts() : size_(0), capacity_(0), body_(nullptr) {}
    
    SpecialInts(int sz) {
        if (sz < 0) {
            std::cerr << "Error: vector size < 0" << std::endl;;
        } else {
            body_ = new int[sz];
            size_ = sz;
            capacity_ = size_;
        }
    }
    
    SpecialInts(int sz, int val) {
        if (sz < 0) {
            std::cerr << "Error: vector size < 0" << std::endl;;
        } else {
            body_ = new int[sz];
            size_ = sz;
            capacity_ = size_;
            
            for (int i = 0; i < size_; ++i) {
                body_[i] = val;
            }
        }
    }
    
    SpecialInts(const SpecialInts &copy_mass) {
        size_ = copy_mass.size_;
        capacity_ = copy_mass.capacity_;
        body_ = new int[capacity_];
        
        for (int i = 0; i < size_; ++i) {
            body_[i] = copy_mass.body_[i];
        }
    }
    
    ~SpecialInts() {
        delete [] body_;
    }
    
    SpecialInts &operator , (const int &new_value) {
        (*this).push_back(new_value);
        return *this;
    }
    
    int &operator [] (const int index) {
        if (index >= size_ || index < 0 ) {
            throw BadRange(index);
        }
        return body_[index];
    }
    
    SpecialInts &operator = (const SpecialInts &rhs) { // rhs = right hand side
        if (this == &rhs) {
            std::cout << "Same vector" << std::endl;
            return *this;
        }
        
        delete [] body_; // pre-delete the allocated memory
        
        body_ = new int[rhs.size_];
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;

        for (int i = 0; i < size_; ++i) {
            body_[i] = rhs.body_[i];
        }

        return *this;
    }
    
    void realloc_mas() {
        if (capacity_ == 0) {
            body_ = new int[1];
            capacity_ = 1;
        }
        
        capacity_ *= 2;
        int *tmp_mas = new int[capacity_];
        
        for (int i = 0; i < size_; ++i) {
            tmp_mas[i] = body_[i];
        }
        
        delete [] body_;
        body_ = tmp_mas;
    }
    
    void push_back(int new_elem) {
        if (size_ == capacity_) {
            realloc_mas();
        }
        body_[size_] = new_elem;
        size_++;
    }
    
    int pop_back() {
        if (size_ > 0) {
            size_--;
            return body_[size_];
        } else {
            std::cerr << "Error: empty vector returned 0" << std::endl;;
            return 0;
        }
    }
    int len() {
        return size_;
    }
};

int main() {
    SpecialInts m;
    m,5,2,6;
    std::cout << "Elem: " << m[0] << ", " << m[1] << ", "<< m[2];
    std::cout << ", length: " << m.len() << std::endl;
    
    // Example of error handling:
    try {
        m[10];
    } catch (BadRange &ex){
        std::cout << "error at index num " << ex.get() << std::endl;
    }
    return 0;
}

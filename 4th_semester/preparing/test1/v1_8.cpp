#include <iostream>

class QueueEmpty {};

class IntQueue {
    int numb_elem_;
    class Elem {
    public:
        int val;
        Elem *next;
    };
    Elem *head_;

public:
    IntQueue() {
        head_ = nullptr;
    }

    ~IntQueue() {
        while (head_ != nullptr) {
            Elem *tmp = head_->next;
            delete head_;
            head_ = tmp;
        }
    }

    void operator<<=(int new_numb) {
        add_elem(new_numb);
    }

    void operator>>=(int &delete_numb) {
        delete_elem(delete_numb);
    }

    void add_elem(int new_val) {
        if (Check(new_val)) {
            Elem *new_elem = new Elem;
            new_elem->val = new_val;
            new_elem->next = head_;
            head_ = new_elem;
            numb_elem_++;
        }
    }

    void delete_elem(int &delete_numb) {
        if (head_ == nullptr) {
            throw QueueEmpty();
        }

        Elem *tmp = head_;
        for (int i = 0; (i < numb_elem_ - 1) && (tmp->next != nullptr) && (tmp->next->next != nullptr); ++i) {
            tmp = tmp->next;
        }

        if (tmp->next != nullptr) {
            delete_numb = tmp->next->val;
            delete tmp->next;
            tmp->next = nullptr;
        } else {
            delete_numb = tmp->val;
            delete tmp;
            tmp = nullptr;
            head_ = nullptr;
        }
        numb_elem_--;
    }

    virtual bool Check(const int n) { return true; }

    void print_queue() {
        if (head_ != nullptr) {
            Elem *tmp = head_;
            std::cout << "queue: ";
            while (tmp != nullptr) {
                std::cout << tmp->val << ", ";
                tmp = tmp->next;
            }
            std::cout << std::endl;
        } else {
            std::cout << "Queue is empty" << std::endl;
        }
    }

    int size() {
        return numb_elem_;
    }
};

class SpecialInts : public IntQueue {
public:
    virtual bool Check(const int n) {
        if (n % 2 != 0) {
            return false;
        }
        return true;
    }
};

int main() {
    IntQueue q;
    q.add_elem(1);
    q.add_elem(3);
    q.add_elem(5);
    std::cout << "Size: " << q.size() << std::endl;
    q.print_queue();
    q <<= 8;
    q.print_queue();
    int tmp;
    q >>= tmp;
    std::cout << "Tmp from >>= : " << tmp << " " << std::endl;
    q >>= tmp;
    std::cout << "Tmp from >>= : " << tmp << " " << std::endl;
    q.print_queue();

    std::cout << "\nSpecialInts:" << std::endl;
    SpecialInts qm;
    qm.add_elem(1);
    qm.add_elem(2);
    qm.print_queue();

    try {
        qm >>= tmp;
        std::cout << "Tmp from SpecialInts >>= : " << tmp << " " << std::endl;
        qm >>= tmp;
        std::cout << "Tmp from SpecialInts >>= : " << tmp << " " << std::endl;
    } catch (QueueEmpty) {
        std::cout << "QueueEmpty() exception catched" << std::endl;
    }

    return 0;
}
#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H

#include "list/DLinkedList.h"
#include "sorting/ISort.h"

// DLinkedListSE kế thừa từ DLinkedList
template<class T>
class DLinkedListSE : public DLinkedList<T> {
public:
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*) = 0,
            bool (*itemEQ)(T&, T&) = 0) :
            DLinkedList<T>(removeData, itemEQ) {
    }

    DLinkedListSE(const DLinkedList<T>& list) {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T&, T&) = 0) {
        this->head = mergeSort(this->head, comparator);

        // Tìm lại tail
        Node* temp = this->head;
        while (temp && temp->next) {
            temp = temp->next;
        }
        this->tail = temp;
    }

protected:
    using typename DLinkedList<T>::Node;

    Node* mergeSort(Node* head, int (*comparator)(T&, T&)) {
        if (!head || !head->next) return head; // Nếu danh sách rỗng hoặc có một phần tử

        // Chia đôi danh sách
        Node* mid = getMiddle(head);
        Node* left = head;
        Node* right = mid->next;
        mid->next = nullptr;

        // Sắp xếp đệ quy từng nửa
        left = mergeSort(left, comparator);
        right = mergeSort(right, comparator);

        // Hợp nhất hai danh sách con
        return merge(left, right, comparator);
    }

    Node* merge(Node* left, Node* right, int (*comparator)(T&, T&)) {
        if (!left) return right;
        if (!right) return left;

        if (compare(left->data, right->data, comparator) <= 0) {
            left->next = merge(left->next, right, comparator);
            left->next->prev = left;
            left->prev = nullptr;
            return left;
        } else {
            right->next = merge(left, right->next, comparator);
            right->next->prev = right;
            right->prev = nullptr;
            return right;
        }
    }

    Node* getMiddle(Node* head) {
        if (!head) return head;

        Node* slow = head;
        Node* fast = head;

        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }

    static int compare(T& lhs, T& rhs, int (*comparator)(T&, T&) = 0) {
        if (comparator != nullptr) return comparator(lhs, rhs);
        else {
            if (lhs < rhs) return -1;
            else if (lhs > rhs) return 1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */

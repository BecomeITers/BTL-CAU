/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "hash/xMap.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
template <class T, class V>
using XHashMap = xMap<T, V>;

template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0){
        //TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }   
    DLinkedList<T> sort(int mode=0, bool sorted=true){
        //TODO
        if (mode == DFS){
            return dfsSort(sorted);
        }
        else{
            return bfsSort(sorted);
        }
    }
    DLinkedList<T> bfsSort(bool sorted=true){ 
        //TODO
        // Khởi tạo bảng in-degree và danh sách kết quả
        xMap<T, int> inDegreeMap = vertex2inDegree(hash_code);
        DLinkedListSE<T> result;
        Queue<T> queue;
        // Thêm tất cả các đỉnh có in-degree = 0 vào hàng đợi
        typename DLinkedList<T>::Iterator it = this->graph->vertices().begin();
        for (; it != this->graph->vertices().end(); it++){
            if (inDegreeMap.get(*it) == 0) {
                queue.push(*it);
            }
        }
        // Duyệt đồ thị theo BFS và cập nhật kết quả
        while (!queue.empty()) {
            T vertex = queue.pop();
            result.add(vertex);
            // Giảm bậc của các đỉnh kề và đưa vào queue nếu in-degree = 0
            DLinkedList<T> outwardEdges = this->graph->getOutwardEdges(vertex);
            typename DLinkedList<T>::Iterator edgeIt = outwardEdges.begin();
            for (edgeIt; edgeIt != outwardEdges.end(); edgeIt++){
                inDegreeMap.put(*edgeIt, inDegreeMap.get(*edgeIt) - 1);
                if (inDegreeMap.get(*edgeIt) == 0){
                    queue.push(*edgeIt);
                }
            }
        }
        if (sorted) result.sort(); // Sắp xếp kết quả nếu yêu cầu
        return result;
    }

    DLinkedList<T> dfsSort(bool sorted=true){
        //TODO
        xMap<T, bool> visited(this->hash_code);
        Stack<T> dfsStack;
        DLinkedListSE<T> result;
        // Đánh dấu tất cả các đỉnh là chưa thăm
        typename DLinkedList<T>::Iterator it = this->graph->vertices().begin();
        for (; it != this->graph->vertices().end(); it++){
            visited.put(*it, false);
        }
        // Thực hiện DFS từ các đỉnh chưa được thăm
        for (it = this->graph->vertices().begin(); it != this->graph->vertices().end(); it++){
            if (!visited.get(*it)){
                dfsHelper(*it, visited, dfsStack);
            }
        }
        // Đưa các đỉnh từ stack vào danh sách kết quả
        while (!dfsStack.empty()) {
            result.add(dfsStack.pop());
        }
        if (sorted) result.sort(); // Sắp xếp kết quả nếu yêu cầu
        return result;
    }

void dfsHelper(T vertex, xMap<T, bool>& visited, Stack<T>& dfsStack) {
    Stack<T> tempStack; // Stack tạm cho DFS
    tempStack.push(vertex);
    while (!tempStack.empty()){
        T current = tempStack.peek();
        if (!visited.get(current)){
            visited.put(current, true);
            // Đẩy các đỉnh kề chưa thăm vào stack
            DLinkedList<T> edges = this->graph->getOutwardEdges(current);
            typename DLinkedList<T>::Iterator it = edges.begin();
            for (; it != edges.end(); it++){
                if (!visited.get(*it)) {
                    tempStack.push(*it);
                }
            }
        }
        else{
            tempStack.pop();
            dfsStack.push(current);
        }
    }
}

protected:
    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int));
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int));
    DLinkedList<T> listOfZeroInDegrees();

}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */


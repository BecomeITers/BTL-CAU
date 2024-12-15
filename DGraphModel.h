/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "sorting/DLinkedListSE.h"


//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    void connect(T from, T to, float weight=0){
        //TODO
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
    if (!fromNode || !toNode) {
        throw VertexNotFoundException();
    }
        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to){
        //TODO
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);
        if (!fromNode || !toNode){
            throw VertexNotFoundException();
        }
        typename AbstractGraph<T>::Edge* edge = fromNode->getEdge(toNode);
        if (!edge){
            throw EdgeNotFoundException();
        }
        fromNode->removeTo(toNode);
    }
    void remove(T vertex){
        //TODO
        typename AbstractGraph<T>::VertexNode* targetNode = this->getVertexNode(vertex);
        if (!targetNode) {
            throw VertexNotFoundException();
        }
        DLinkedList<T> inwardEdges = this->getInwardEdges(vertex);
        typename DLinkedList<T>::Iterator itInward = inwardEdges.begin();
        while (itInward != inwardEdges.end()){
            typename AbstractGraph<T>::VertexNode* sourceNode = this->getVertexNode(*itInward);
            if (sourceNode){
                sourceNode->removeTo(targetNode); // Xóa cạnh từ sourceNode đến targetNode
            }
            ++itInward;
        }
        DLinkedList<T> outwardEdges = this->getOutwardEdges(vertex);
        typename DLinkedList<T>::Iterator itOutward = outwardEdges.begin();
        while (itOutward != outwardEdges.end()){
            typename AbstractGraph<T>::VertexNode* destNode = this->getVertexNode(*itOutward);
            if (destNode){
                targetNode->removeTo(destNode); // Xóa cạnh từ targetNode đến destNode
            }
            ++itOutward;
        }
        this->nodeList.removeAt(this->nodeList.indexOf(targetNode));
        delete targetNode;
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);
        // Thêm các đỉnh
        for (int i = 0; i < nvertices; i++){
            graph->add(vertices[i]);
        }
        // Thêm các cạnh
        for (int i = 0; i < nedges; i++){
            graph->connect(edges[i].from->vertex, edges[i].to->vertex, edges[i].weight);
        }
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */


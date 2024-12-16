/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);
        if (!fromNode){
            this->add(from);
            fromNode = this->getVertexNode(from);
        }
        if (!toNode){
            this->add(to);
            toNode = this->getVertexNode(to);
        }
        // Thêm cạnh vô hướng từ from -> to và ngược lại
        if (!fromNode->getEdge(toNode)){
            fromNode->connect(toNode, weight);
            toNode->connect(fromNode, weight);
        }
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);
        if (fromNode && toNode){
            // Xóa cạnh từ from -> to và từ to -> from
            fromNode->removeTo(toNode);
            toNode->removeTo(fromNode);
        }
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *targetNode = this->getVertexNode(vertex);
        if (targetNode){
            // Xóa tất cả các cạnh liên kết với đỉnh này
            typename DLinkedList<typename AbstractGraph<T>::VertexNode *>::Iterator it = this->nodeList.begin();
            while (it != this->nodeList.end()){
                (*it)->removeTo(targetNode);
                it++;
            }
            // Xóa đỉnh khỏi danh sách
            this->nodeList.removeAt(this->nodeList.indexOf(targetNode));
            delete targetNode;
        }
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);
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

#endif /* UGRAPHMODEL_H */

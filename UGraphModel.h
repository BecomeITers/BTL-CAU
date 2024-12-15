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
//#include "stacknqueue/PriorityQueue.h"

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
        VertexNode* fromNode = getVertexNode(from);
        VertexNode* toNode = getVertexNode(to);
        if(fromNode == nullptr) throw VertexNotFoundException(vertex2Str(from));
        if(toNode == nullptr) throw VertexNotFoundException(vertex2Str(to));
        if(from == to){
            Edge* selfLoopEdge = new Edge(fromNode, fromNode, weight);
            fromNode->adList.add(selfLoopEdge);
            fromNode->outDegree_++;
            fromNode->inDegree_++;
        }
        else{
            Edge* edge1 = new Edge(fromNode, toNode, weight);
            Edge* edge2 = new Edge(toNode, fromNode, weight);
            fromNode->adList.add(edge1);
            toNode->adList.add(edge2);
            fromNode->outDegree_++;
            toNode->inDegree_++;
            fromNode->inDegree_++;
            toNode->outDegree_++;
        }
    }
    void disconnect(T from, T to)
    {
        VertexNode* fromNode = getVertexNode(from);
        VertexNode* toNode = getVertexNode(to);
        if (fromNode == nullptr) throw VertexNotFoundException(vertex2Str(from));
        if (toNode == nullptr) throw VertexNotFoundException(vertex2Str(to));
        Edge* edge1 = fromNode->getEdge(toNode);
        if (edge1 != nullptr) {
            fromNode->adList.remove(edge1);  
            fromNode->outDegree_--;           
            toNode->inDegree_--;              
        }
        Edge* edge2 = toNode->getEdge(fromNode);
        if (edge2 != nullptr) {
            toNode->adList.remove(edge2);   
            toNode->outDegree_--;             
            fromNode->inDegree_--;          
        }
    }
    void remove(T vertex)
    {
        VertexNode* nodeToRemove = getVertexNode(vertex);
        if (nodeToRemove == nullptr) {
            throw VertexNotFoundException(vertex2Str(vertex));
        }
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        while (it != nodeList.end()) {
            VertexNode* currentNode = *it;
            Edge* edgeToRemove = currentNode->getEdge(nodeToRemove);
            if (edgeToRemove != nullptr) {
                currentNode->adList.remove(edgeToRemove);  
                currentNode->outDegree_--;                  
                nodeToRemove->inDegree_--;                 
            }
            Edge* edgeFromRemove = nodeToRemove->getEdge(currentNode);
            if (edgeFromRemove != nullptr) {
                nodeToRemove->adList.remove(edgeFromRemove);  
                nodeToRemove->outDegree_--;                   
                currentNode->inDegree_--;                     
            }

            it++; 
        }
        nodeList.remove(nodeToRemove); 
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        UGraphModel<T>* graph = new UGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; i++) {
            graph->addVertex(vertices[i]);
        }
        for (int i = 0; i < nedges; i++) {
            T from = edges[i].from;
            T to = edges[i].to;
            float weight = edges[i].weight;
            graph->connect(from, to, weight);
        }
        return graph; 
    }
};

#endif /* UGRAPHMODEL_H */

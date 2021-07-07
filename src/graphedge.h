#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

#include <vector>
#include <string>

class GraphNode; // forward declaration

class GraphEdge
{
/*
 * No assignment tasks in the GraphEdge class
 */

private:
    // data handles (not owned) --> Edges do not own anything - just the connection element with non owning rights of the functional components
    GraphNode *_childNode;
    GraphNode *_parentNode;

    // proprietary members
    int _id;
    std::vector<std::string> _keywords; // list of topics associated with this edge
    

public:
    // constructor / desctructor
    GraphEdge(int id);

    // getter / setter
    int GetID() { return _id; }
    void SetChildNode(GraphNode *childNode);
    void SetParentNode(GraphNode *parentNode);
    GraphNode *GetChildNode() { return _childNode; }
    std::vector<std::string> GetKeywords() { return _keywords; }

    // proprietary functions
    void AddToken(std::string token);
};

#endif /* GRAPHEDGE_H_ */
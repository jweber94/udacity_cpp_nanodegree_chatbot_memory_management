#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include "chatbot.h"
#include <string>
#include <vector>

// forward declarations
class GraphEdge;

class GraphNode {
private:
  //// STUDENT CODE
  ////

  // data handles (owned) --> The class IS responsible for the child edges
  std::vector<GraphEdge *> _childEdges; // edges to subsequent nodes --> owned
                                        // by graph node in the finished version

  // data handles (not owned) --> The class is NOT responsible for the non-owned
  // parent edges
  std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes
  ChatBot *_chatBot; // handle to the chatbot --> The chatbot is associated with
                     // a graph node, such that the chatbot object can move
                     // (!!!) throw the graph

  ////
  //// EOF STUDENT CODE

  // proprietary members
  int _id;
  std::vector<std::string>
      _answers; // Multiple answers possible, even if in the project is just one
                // answer per node existend

public:
  // constructor / destructor
  GraphNode(int id);
  ~GraphNode();

  // getter / setter
  int GetID() { return _id; }
  int GetNumberOfChildEdges() { return _childEdges.size(); }
  GraphEdge *GetChildEdgeAtIndex(int index);
  std::vector<std::string> GetAnswers() { return _answers; }
  int GetNumberOfParents() { return _parentEdges.size(); }

  // proprietary functions
  void AddToken(std::string token); // add answers to list --> A tokes is an
                                    // answer for the individual graph node
  void AddEdgeToParentNode(GraphEdge *edge);
  void AddEdgeToChildNode(GraphEdge *edge);

  //// STUDENT CODE
  ////

  void MoveChatbotHere(
      ChatBot
          *chatbot); // Move the chatbot throu the graph --> MUST be an
                     // unique_ptr, since the ownership to the chatbot is unique

  ////
  //// EOF STUDENT CODE

  void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */
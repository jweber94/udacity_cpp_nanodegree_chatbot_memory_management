#include "graphnode.h"
#include "graphedge.h"
#include <iostream> // debug

GraphNode::GraphNode(int id) { _id = id; }

GraphNode::~GraphNode() {
  //// STUDENT CODE
  ////

  // delete _chatBot; // CAUTION: This was the bug for task 0
  ////
  //// EOF STUDENT CODE
}

// The following methods are called just a single time, when the graph is loaded
// from the answergraph.txt file
void GraphNode::AddToken(std::string token) { _answers.push_back(token); }

void GraphNode::AddEdgeToParentNode(GraphEdge *edge) {
  _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge) {
  _childEdges.emplace_back(std::move(edge));
}

//// STUDENT CODE
//// -->
void GraphNode::MoveChatbotHere(ChatBot *chatbot) {
  // This method is called from another position
  _chatBot = chatbot; // pass the chatbot handle from the caller-node to the
                      // current node
  _chatBot->SetCurrentNode(
      this); // set the current node to the current node for the chatbot
             // The current node as well as the chatbot itself has a notion
             // about where the chatbot is currently at within the graph
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode) {
  // This is the function that is called by another node to move the chatbot to
  // the "newNode", which is the current node in MoveChatbotHere(ChatBot) --> By
  // calling this function, the new aim node for the chatbot is handed to the
  // function
  newNode->MoveChatbotHere(
      _chatBot); // CAUTION: We have two pointers to the chatbot for a short
                 // period of time, until the chatbot-pointer from the old node
                 // is invalidated --> Solve this with a unique pointer and move
                 // semantics
  _chatBot = nullptr; // invalidate pointer at source
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index) {
  // Helper function
  //// STUDENT CODE
  ////

  return _childEdges[index]
      .get(); // return data without handing over the ownership

  ////
  //// EOF STUDENT CODE
}
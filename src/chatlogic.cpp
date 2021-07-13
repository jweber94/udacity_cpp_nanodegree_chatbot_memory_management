#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <tuple>
#include <vector>

#include "chatbot.h"
#include "chatlogic.h"
#include "graphedge.h"
#include "graphnode.h"

ChatLogic::ChatLogic() {
  //// STUDENT CODE
  ////

  // create instance of chatbot
  _chatBot = new ChatBot("../images/chatbot.png");

  // add pointer to chatlogic so that chatbot answers can be passed on to the
  // GUI
  _chatBot->SetChatLogicHandle(this);

  ////
  //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic() {
  //// STUDENT CODE
  ////

  // delete chatbot instance
  std::cout << "calling delete _chatBot"
            << std::endl; // DEBUG --> If you call delete, the memory of
                          // _chatBot is deallocated and the destructor of the
                          // ChatBot class is called
  delete _chatBot;

  // delete all nodes --> We do not need the deletion, since the new
  // implementation does use smartpointer that delete the elements automatically
  // when the destructor is called (if the Class ChatLogic goes out of scope)
  /*
  for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it) {
    delete *it;
  }
  */

  /*
  // delete all edges
  for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
  {
    delete *it;
  }
  */

  ////
  //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens,
                                      T &element) {
  // find all occurences for current node
  auto token = tokens.begin();
  while (true) {
    token = std::find_if(
        token, tokens.end(),
        [&tokenID](const std::pair<std::string, std::string> &pair) {
          return pair.first == tokenID;
          ;
        });
    if (token != tokens.end()) {
      element.AddToken(token->second); // add new keyword to edge
      token++;                         // increment iterator to next element
    } else {
      break; // quit infinite while-loop
    }
  }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename) {
  // load file with answer graph elements
  std::ifstream file(filename);

  // check for file availability and process it line by line
  if (file) {
    // loop over all lines in the file
    std::string lineStr;
    while (getline(file, lineStr)) {
      // extract all tokens from current line
      tokenlist tokens;
      while (lineStr.size() > 0) {
        // extract next token
        int posTokenFront = lineStr.find("<");
        int posTokenBack = lineStr.find(">");
        if (posTokenFront < 0 || posTokenBack < 0)
          break; // quit loop if no complete token has been found
        std::string tokenStr =
            lineStr.substr(posTokenFront + 1, posTokenBack - 1);

        // extract token type and info
        int posTokenInfo = tokenStr.find(":");
        if (posTokenInfo != std::string::npos) {
          std::string tokenType = tokenStr.substr(0, posTokenInfo);
          std::string tokenInfo =
              tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

          // add token to vector
          tokens.push_back(std::make_pair(tokenType, tokenInfo));
        }

        // remove token from current line
        lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
      }

      // process tokens for current line
      auto type =
          std::find_if(tokens.begin(), tokens.end(),
                       [](const std::pair<std::string, std::string> &pair) {
                         return pair.first == "TYPE";
                       });
      if (type != tokens.end()) {
        // check for id
        auto idToken =
            std::find_if(tokens.begin(), tokens.end(),
                         [](const std::pair<std::string, std::string> &pair) {
                           return pair.first == "ID";
                         });
        if (idToken != tokens.end()) {
          // extract id from token
          int id = std::stoi(idToken->second);

          // node-based processing
          if (type->second == "NODE") {
            //// STUDENT CODE
            ////

            // check if node with this ID exists already
            auto newNode = std::find_if(
                _nodes.begin(), _nodes.end(),
                [&id](std::unique_ptr<GraphNode> &node) {
                  // We need to pass an unique_ptr reference since we do not
                  // want to hand over ownership to the lambda function but
                  // there is no possability to hand over the raw pointer since
                  // we use a STL function. To hand over a raw pointer we would
                  // need to edit the STL implementation
                  return node->GetID() == id;
                }); // this is a lambda function, [&id] makes the id avalable to
                    // the labmda function by reference
                    // std::find_if returns the first element that satisfies the
            // condition function (here this is the lambda function) No change
            // of the (lambda-)function parameter, since the lambda just needs
            // the value and not owner rights

            // create new element if ID does not yet exist
            if (newNode == _nodes.end()) {
              _nodes.emplace_back(std::make_unique<GraphNode>(id));
              //_nodes.emplace_back(std::make_unique<GraphNode>(GraphNode(id)));
              //// DO NOT USE THIS since it is syntacically wrong
              newNode = _nodes.end() - 1; // get iterator to last element

              // add all answers to current node
              AddAllTokensToElement("ANSWER", tokens, **newNode);
            }

            ////
            //// EOF STUDENT CODE
          }

          // edge-based processing
          if (type->second == "EDGE") {
            //// STUDENT CODE
            ////

            // find tokens for incoming (parent) and outgoing (child) node
            auto parentToken = std::find_if(
                tokens.begin(), tokens.end(),
                [](const std::pair<std::string, std::string> &pair) {
                  return pair.first == "PARENT";
                });
            auto childToken = std::find_if(
                tokens.begin(), tokens.end(),
                [](const std::pair<std::string, std::string> &pair) {
                  return pair.first == "CHILD";
                });

            if (parentToken != tokens.end() && childToken != tokens.end()) {
              // get iterator on incoming and outgoing node via ID search
              auto parentNode = std::find_if(
                  _nodes.begin(), _nodes.end(),
                  [&parentToken](std::unique_ptr<GraphNode> &node) {
                    return node->GetID() == std::stoi(parentToken->second);
                  });
              auto childNode = std::find_if(
                  _nodes.begin(), _nodes.end(),
                  [&childToken](std::unique_ptr<GraphNode> &node) {
                    return node->GetID() == std::stoi(childToken->second);
                  });

              // create new edge
              // std::unique_ptr<GraphEdge> edge =
              // std::make_unique<GraphEdge>(GraphEdge(id));
              std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
              edge->SetChildNode((*childNode).get());
              edge->SetParentNode((*parentNode).get());
              //_edges.push_back(std::move(edge)); // since we have exclusivly
              //owned edge elements, we need to move them instead of append them
              //to a vector of pointers that point to the heap
              // After task 4 we have to remove the _edges vector, since it does
              // not manage the edges anymore. Now the GraphNode elements are
              // exclusivly responsible for the GraphEdge elements. If we move
              // them to the _edges vector, a non valid unique_ptr will be
              // "moved" to the GraphNode elements in
              // (*parentNode)->AddEdgeToChildNode(std::move(edge)) and a non
              // valid reference is passed to
              // (*childNode)->AddEdgeToParentNode(edge.get())

              // find all keywords for current node
              AddAllTokensToElement("KEYWORD", tokens, *edge);

              // store reference in child node and parent node
              (*childNode)
                  ->AddEdgeToParentNode(
                      edge.get()); // The followning functions demands raw
                                   // pointers as function parameters, so we
                                   // have to hand the in the smartpointer
                                   // encapsulated data to the function instead
                                   // of the wrapping smartpointer
              (*parentNode)
                  ->AddEdgeToChildNode(
                      std::move(edge)); // move the ownership from the
                                        // initialized edge to the graph node
            }

            ////
            //// EOF STUDENT CODE
          }
        } else {
          std::cout << "Error: ID missing. Line is ignored!" << std::endl;
        }
      }
    } // eof loop over all lines in the file

    file.close();

  } // eof check for file availability
  else {
    std::cout << "File could not be opened!" << std::endl;
    return;
  }

  //// STUDENT CODE
  ////

  // identify root node
  GraphNode *rootNode = nullptr;
  for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it) {
    // going throu all the nodes and move and link the edges corresponding to
    // the ownership specifications by the task 4 of the assignment search for
    // nodes which have no incoming edges
    if ((*it)->GetNumberOfParents() == 0) {

      if (rootNode == nullptr) {
        rootNode = (*it).get(); // assign current node to root
      } else {
        std::cout << "ERROR : Multiple root nodes detected" << std::endl;
      }
    }
  }

  // add chatbot to graph root node
  _chatBot->SetRootNode(rootNode);
  rootNode->MoveChatbotHere(_chatBot);

  ////
  //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog) {
  _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot) { _chatBot = chatbot; }

void ChatLogic::SendMessageToChatbot(std::string message) {
  _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message) {
  _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot() {
  return _chatBot->GetImageHandle();
}

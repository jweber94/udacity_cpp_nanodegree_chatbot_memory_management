#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include "chatgui.h"
#include <string>
#include <vector>

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic {
  /*
   * Creates the chatbot answer graph and make it usable by the chatgui
   */

private:
  //// STUDENT CODE
  ////

  // data handles (owned) 
  std::vector<std::unique_ptr<GraphNode> > _nodes;
  //std::vector<std::unique_ptr<GraphEdge> > _edges; // after task 4 we do not need this vector anymore 

  ////
  //// EOF STUDENT CODE

  // data handles (not owned) --> Currently owned
  GraphNode *_currentNode;
  ChatBot *_chatBot; // just the pointer to the raw data without the intention to manage the underlying memory
  ChatBotPanelDialog *_panelDialog;

  // proprietary type definitions
  typedef std::vector<std::pair<std::string, std::string>> tokenlist;

  // proprietary functions
  template <typename T>
  void AddAllTokensToElement(std::string tokenID, tokenlist &tokens,
                             T &element);

public:
  // constructor / destructor
  ChatLogic();
  ~ChatLogic();

  // getter / setter
  void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
  void SetChatbotHandle(ChatBot *chatbot);

  // proprietary functions
  void LoadAnswerGraphFromFile(std::string filename);
  void SendMessageToChatbot(std::string message);
  void SendMessageToUser(std::string message);
  wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */
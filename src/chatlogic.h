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

  // data handles (owned) --> owned = unique_ptr (more restrictive and needs to
  // use move semantics) or shared_ptr
  std::vector<GraphNode *> _nodes;
  std::vector<GraphEdge *>
      _edges; // In one of the tasks, the ownership of the edges is moved into
              // the nodes - currently the ChatLogic class owns them both

  ////
  //// EOF STUDENT CODE

  // data handles (not owned) --> Currently owned
  GraphNode *_currentNode;
  ChatBot *_chatBot;
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
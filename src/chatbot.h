#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <string>
#include <wx/bitmap.h>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot {
private:
  // data handles (owned)
  wxBitmap *_image; // avatar image of the chatbot that is displayed on the
                    // chatbots output

  // data handles (not owned)
  GraphNode *_currentNode;
  GraphNode *_rootNode;
  ChatLogic *_chatLogic;

  // proprietary functions
  int ComputeLevenshteinDistance(
      std::string s1,
      std::string s2); // similarity measure between strings --> small
                       // levenshtein distances are very simular words. The int
                       // is the number of charaters that are different

public:
  // constructors / destructors
  ChatBot();                     // constructor WITHOUT memory allocation
  ChatBot(std::string filename); // constructor WITH memory allocation
  ~ChatBot();                    // (ROF) 1.: Destructor

  //// STUDENT CODE
  ////
  /*
   * Concept:
   *  Create the class with propoer memory management but without smartpointers,
   *  since the std::xxx_ptr<ChatBot> cb_ptr; is the RAII wrapper for the
   *  ownership and management of the class.
   *
   *  In order to create just one ChatBot instance, the memory management
   * concept is based on exclusive ownership, so there is a exclusive ownership
   * copy policy implemented in the copy constructor and copy assignment
   * operator
   *
   *  ROF = rule of five
   */
  // REMARK: Do NOT make the reference instance of the copy member methods
  // const, due to the need of invalidating the pointer membervariables
  ChatBot(ChatBot &refChatBot);            // (ROF) 2.: Copy constructor
  ChatBot &operator=(ChatBot &refChatBot); // (ROF) 3.: Copy assignment operator

  ChatBot(ChatBot &&refChatBot); // (ROF) 4.: Move constructor

  ChatBot &
  operator=(ChatBot &&refChatBot); // (ROF) 5.: Move assignment operator

  ////
  //// EOF STUDENT CODE

  // getters / setters
  void SetCurrentNode(GraphNode *node);
  void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
  void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
  ChatLogic *GetChatLogicHandle() { return _chatLogic; }
  wxBitmap *GetImageHandle() { return _image; }

  // communication
  void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

#include "chatbot.h"
#include "chatlogic.h"
#include "graphedge.h"
#include "graphnode.h"

// constructor WITHOUT memory allocation
ChatBot::ChatBot() {
  std::cout << "Called the ChatBot constructor without an image\n";
  // invalidate data handles
  _image = nullptr;
  _chatLogic = nullptr;
  _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename) {
  std::cout << "ChatBot Constructor with an image\n";

  // invalidate data handles
  _chatLogic = nullptr;
  _rootNode = nullptr;

  // load image into heap memory
  _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

ChatBot::~ChatBot() {
  // (ROF) 1.: Destructor
  std::cout << "ChatBot Destructor was called" << std::endl;

  // deallocate heap memory
  if (_image != NULL) // Attention: wxWidgets used NULL and not nullptr
  {
    delete _image;
    _image = NULL;
  }
}

//// STUDENT CODE
////
ChatBot::ChatBot(ChatBot &refChatBot) {
  // (ROF) 2.: Copy constructor
  std::cout << "ChatBot copy constructor was called\n";
  // exclusive ownership copy policy --> exclusive deep copy for the owned
  // member variables rewrite the content of the member
  if (refChatBot._image != nullptr || refChatBot._image != NULL) {
    // check if the old ChatBot instance was initialized with an image and copy
    // the image if so copy
    _image = new wxBitmap();
    *_image = *refChatBot._image;
    // delete the old one
    delete refChatBot._image;
    refChatBot._image = nullptr; // invalidate the old reference
  } else {
    // no image in the reference instance of ChatBot
    this->_image = nullptr;
  }

  // just remap the not owned memebers, since deleting them makes them unsable
  // for all other references to them
  _currentNode = refChatBot._currentNode;
  refChatBot._currentNode = nullptr;
  _rootNode = refChatBot._rootNode;
  refChatBot._rootNode = nullptr;
  _chatLogic = refChatBot._chatLogic;
  refChatBot._chatLogic = nullptr;
}

ChatBot &ChatBot::operator=(ChatBot &refChatBot) {
  // (ROF) 3.: Copy assignment operator
  std::cout << "ChatBot copy assignment operator was called\n";
  // no operation needed if you want to copy to the already existing object
  if (this == &refChatBot) { // compare the addresses of the class instances
    return *this;
  }

  // deep copy the owned members
  if (_image != nullptr && _image != NULL) {
    // check if there is an image behind the aimed ChatBot instance and if so,
    // delete it in order to make it available for the member of the reference
    // element
    delete _image;
  }
  *_image = *refChatBot._image;
  refChatBot._image =
      NULL; // invalidating due to exclusive ownership copy policy FIXME: Check
            // if this must be nullptr or NULL (and whats the difference?
            // https://stackoverflow.com/questions/20509734/null-vs-nullptr-why-was-it-replaced)

  // remap and invalidate the non owned members
  _currentNode = refChatBot._currentNode;
  refChatBot._currentNode = nullptr;
  _rootNode = refChatBot._rootNode;
  refChatBot._rootNode = nullptr;
  _chatLogic = refChatBot._chatLogic;
  refChatBot._chatLogic = nullptr;

  return *this;
}

ChatBot::ChatBot(ChatBot &&refChatBot) {
  // (ROF) 4.: Move constructor
  std::cout << "ChatBot move constructor was called\n";
  _image = refChatBot._image;
  _currentNode = refChatBot._currentNode;
  _rootNode = refChatBot._rootNode;
  _chatLogic = refChatBot._chatLogic;

  refChatBot._image = nullptr; // FIXME: Check if this must be NULL or nullptr
  refChatBot._currentNode = nullptr;
  refChatBot._rootNode = nullptr;
  refChatBot._chatLogic = nullptr;
}

ChatBot &ChatBot::operator=(ChatBot &&refChatBot) {
  // (ROF) 5.: Move assignment operator
  std::cout << "ChatBot move assignment operator was called\n";

  if (_image != nullptr && _image != NULL) {
    delete _image; // free the memory if there were an image on the heap is
                   // associated with the aim ChatBot instance
  }

  _image = refChatBot._image;
  _currentNode = refChatBot._currentNode;
  _rootNode = refChatBot._rootNode;
  _chatLogic = refChatBot._chatLogic;

  refChatBot._image = nullptr; // FIXME: Check if this must be NULL or nullptr
  refChatBot._currentNode = nullptr;
  refChatBot._rootNode = nullptr;
  refChatBot._chatLogic = nullptr;

  return *this;
}

////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message) {
  // loop over all edges and keywords and compute Levenshtein distance to query
  typedef std::pair<GraphEdge *, int> EdgeDist;
  std::vector<EdgeDist> levDists; // format is <ptr,levDist>

  for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i) {
    GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
    for (auto keyword : edge->GetKeywords()) {
      EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
      levDists.push_back(ed);
    }
  }

  // select best fitting edge to proceed along
  GraphNode *newNode;
  if (levDists.size() > 0) {
    // sort in ascending order of Levenshtein distance (best fit is at the top)
    std::sort(levDists.begin(), levDists.end(),
              [](const EdgeDist &a, const EdgeDist &b) {
                return a.second < b.second;
              });
    newNode = levDists.at(0).first->GetChildNode(); // after sorting the best
                                                    // edge is at first position
  } else {
    // go back to root node
    newNode = _rootNode;
  }

  // tell current node to move chatbot to new node
  _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node) {
  // update pointer to current node
  _currentNode = node;

  // select a random node answer (if several answers should exist)
  std::vector<std::string> answers = _currentNode->GetAnswers();
  std::mt19937 generator(int(std::time(0)));
  std::uniform_int_distribution<int> dis(0, answers.size() - 1);
  std::string answer = answers.at(dis(generator));

  // send selected node answer to user
  _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2) {
  // convert both strings to upper-case before comparing
  std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
  std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

  // compute Levenshtein distance measure between both strings
  const size_t m(s1.size());
  const size_t n(s2.size());

  if (m == 0)
    return n;
  if (n == 0)
    return m;

  size_t *costs = new size_t[n + 1];

  for (size_t k = 0; k <= n; k++)
    costs[k] = k;

  size_t i = 0;
  for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end();
       ++it1, ++i) {
    costs[0] = i + 1;
    size_t corner = i;

    size_t j = 0;
    for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end();
         ++it2, ++j) {
      size_t upper = costs[j + 1];
      if (*it1 == *it2) {
        costs[j + 1] = corner;
      } else {
        size_t t(upper < corner ? upper : corner);
        costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
      }

      corner = upper;
    }
  }

  int result = costs[n];
  delete[] costs;

  return result;
}
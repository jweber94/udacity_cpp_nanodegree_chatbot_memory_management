#ifndef CHATGUI_H_
#define CHATGUI_H_

#include <wx/wx.h>

class ChatLogic; // forward declaration

// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
/*
* Responsible for the Interface User <--> Chatbot
*/

private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

    //// STUDENT CODE
    ////

    ChatLogic *_chatLogic;

    ////
    //// EOF STUDENT CODE

public:
    // constructor / destructor
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);
    ~ChatBotPanelDialog();

    // getter / setter
    ChatLogic *GetChatLogicHandle() { return _chatLogic; }

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // proprietary functions
    void AddDialogItem(wxString text, bool isFromUser = true);
    void PrintChatbotResponse(std::string response);

    DECLARE_EVENT_TABLE()
};

// dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel
{
/*
* One Item that inicates, if it is the user input or the chatbot output
*/

private:
    // control elements
    wxStaticBitmap *_chatBotImg;
    wxStaticText *_chatBotTxt;

public:
    // constructor / destructor
    ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
    // TODO: Since _chatBotImg and _chatBotTxt are allocated on the heap, there will be a potential memory leak, because the class has no destructor, where the memory is deallocated
};

// frame containing all control elements
class ChatBotFrame : public wxFrame
{
/*
* Frame of the Window that the chatbot program is acting
*/

private:
    // control elements
    ChatBotPanelDialog *_panelDialog;
    wxTextCtrl *_userTextCtrl;

    // events
    void OnEnter(wxCommandEvent &WXUNUSED(event));

public:
    // constructor / desctructor
    ChatBotFrame(const wxString &title);
    // TODO: Since _panelDialog and _userTextCtrl are allocated on the heap, there will be a potential memory leak, because the class has no destructor, where the memory is deallocated
};

// control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
/*
* Display background image
*/

    // control elements
    wxBitmap _image;

public:
    // constructor / desctructor
    ChatBotFrameImagePanel(wxFrame *parent); 

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

// wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
/*
* Encapsulates the main function
*/

public:
    // events
    virtual bool OnInit();
};

#endif /* CHATGUI_H_ */

#include "ui/loading_screen.hpp"
#include "directory.hpp"

using namespace std;
using namespace Rocket;

extern PandaFramework* framework;

LoadingScreen* LoadingScreen::Current = 0;

LoadingScreen::LoadingScreen(WindowFramework* window, Core::Context* context) : UiBase(window, context)
{
  Core::ElementDocument* doc = context->LoadDocument("data/loading.rml");

  root = doc;
  if (root)
    Translate();
  Show();
  done    = false;
  Current = this;
}

LoadingScreen::~LoadingScreen()
{
  Current = 0;
}

void LoadingScreen::SetBackground(const string& str)
{
  if (Current)
  {
    Core::Element* element            = Current->root->GetElementById("loading_screen");
    Core::Element* background_defined = Current->root->GetElementById(Core::String("defined-") + str.c_str());

    if (element)
    {
      if (background_defined)
      {
        element->SetClass("default-loading-screen", false);
        element->SetClass(str.c_str(), true);
      }
      else
        element->SetClass("default-loading-screen", true);
    }
  }
}

void LoadingScreen::AppendText(const string& str)
{
  if (Current)
    Current->DoAppendText(str);
}

void LoadingScreen::DoAppendText(const std::string& str)
{
  Wait();
  {
    Core::Element* input = root->GetElementById("content");
    Core::String   content;

    input->GetInnerRML(content);
    content = Core::String(content + "<br />" + str.c_str());
    input->SetInnerRML(content);
  }
  Post();
  Refresh();
}

void LoadingScreen::Refresh(void)
{
  Wait();
  if (root)
  {
    root->PullToFront();
    framework->get_graphics_engine()->render_frame();
  }
  Post();
}

#include "gameui.hpp"

using namespace std;
using namespace Rocket;

extern PandaFramework* framework;

LoadingScreen::LoadingScreen(WindowFramework* window, Core::Context* context) : UiBase(window, context)
{
  Core::ElementDocument* doc = context->LoadDocument("data/loading.rml");

  _root = doc;
  if (_root)
    Translate();
  Show();
  done = false;
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::AppendText(const std::string& str)
{
  Core::Element* input = _root->GetElementById("content");
  Core::String   content;

  Wait();
  input->GetInnerRML(content);
  content = Core::String(str.c_str()) + "<br />" + content;
  input->SetInnerRML(content);
  Post();
  Refresh();
}

void LoadingScreen::Refresh(void)
{
  Wait();
  framework->get_graphics_engine()->render_frame();
  Post();
}


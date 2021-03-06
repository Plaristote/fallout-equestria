#include "ui/ui_equip_mode.hpp"
#include <level/inventory.hpp>

using namespace std;

UiEquipMode::UiEquipMode(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  root        = context->LoadDocument("data/dialog_equiped_mode.rml");
  root_choices = 0;
  if (root)
  {
    root_choices = root->GetElementById("dialog-actions");
    ToggleEventListener(true, "cancel", "click", CancelClicked);
    CancelClicked.EventReceived.Connect(*this, &UiEquipMode::CallbackCancel);
    root->Show();
  }
}

UiEquipMode::~UiEquipMode()
{
  ToggleEventListener(false, "cancel", "click", CancelClicked);
  ListenButtons(false);
}

void UiEquipMode::Destroy(void)
{
  if (root)
    root->Hide();
}

void UiEquipMode::ListenButtons(bool activate)
{
  Rocket::ForeachElement(root_choices, "button", [this, activate](Rocket::Core::Element* button)
  {
    if (activate)
      button->AddEventListener   ("click", &ModeClicked);
    else
      button->RemoveEventListener("click", &ModeClicked);
  });
}

void UiEquipMode::Initialize(void)
{
  ListenButtons(true);
  ModeClicked.EventReceived.Connect([this](Rocket::Core::Event& event)
  {
    Rocket::Core::Element* element  = event.GetCurrentElement();
    Rocket::Core::Variant* variable = element->GetAttribute("data-mode");

    if (variable)
    {
      int       mode;

      if (variable->GetInto<int>(mode))
      {
        EquipModeSelected.Emit(mode);
        Closed.Emit();
      }
    }
  });
}

void UiEquipMode::AddOption(unsigned char mode, const std::string& name)
{
  if (root_choices)
  {
    std::stringstream    rml;
    Rocket::Core::String rml_;

    rml << "<button data-mode='" << (int)mode << "' i18n=\"" << name << "\" class='universal_button'>";
    rml << i18n::T(name);
    rml << "</button><br />";
    root_choices->GetInnerRML(rml_);
    root_choices->SetInnerRML(rml_ + rml.str().c_str());
  }
}

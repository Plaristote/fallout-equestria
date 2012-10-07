#ifndef  SAVEUIS_HPP
# define SAVEUIS_HPP

# include "rocket_extension.hpp"
# include "directory.hpp"

class UiLoad : public UiBase
{
public:
  UiLoad(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath);
  virtual ~UiLoad();
  Observatory::Signal<void (unsigned char)> LoadSlot;
  
private:
  void LoadGame(Rocket::Core::Event&);
  void ClickSlot(Rocket::Core::Event& event);
  void Cancel(Rocket::Core::Event&);
  
  RocketListener EventLoadGame, EventClickSlot, EventCancel;

  Rocket::Core::Element* _selectedSlot;  
};

class UiSave : public UiBase
{
public:
  UiSave(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath);
  virtual ~UiSave();
  Observatory::Signal<bool (unsigned char)> SaveToSlot;

private:
  void SaveGame(Rocket::Core::Event&);
  void ClickSlot(Rocket::Core::Event& event);
  void Cancel(Rocket::Core::Event&);
  
  RocketListener EventSaveGame, EventClickSlot, EventCancel;

  Rocket::Core::Element* _selectedSlot;
};

#endif
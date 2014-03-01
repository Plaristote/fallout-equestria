#ifndef  SAVEUIS_HPP
# define SAVEUIS_HPP

# include "rocket_extension.hpp"
# include "directory.hpp"

class UiLoad : public UiBase
{
public:
  UiLoad(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath);
  virtual ~UiLoad();
  Sync::Signal<void (unsigned char)> LoadSlot, EraseSlot;
  Sync::Signal<void>                 Done;
  
  static std::string LoadSlotRml(const std::string& prefix, unsigned short slot);
  
private:
  void LoadSlots(const std::string& savepath);
  void LoadGame(Rocket::Core::Event&);
  void ClickSlot(Rocket::Core::Event& event);
  void Cancel(Rocket::Core::Event&);
  void Erase(Rocket::Core::Event&);
  
  RocketListener EventLoadGame, EventClickSlot, EventCancel, EventErase;

  Rocket::Core::Element* _selectedSlot;
  std::string            _savePath;
};

class UiSave : public UiBase
{
public:
  UiSave(WindowFramework* window, Rocket::Core::Context* context, const std::string& savePath);
  virtual ~UiSave();
  Sync::Signal<bool (unsigned char)> SaveToSlot, EraseSlot;
  Sync::Signal<void>                 Done;

private:
  void LoadSlots(const std::string& savepath);
  void SaveGame(Rocket::Core::Event&);
  void ClickSlot(Rocket::Core::Event& event);
  void Cancel(Rocket::Core::Event&);
  void Erase(Rocket::Core::Event&);
  
  RocketListener EventSaveGame, EventClickSlot, EventCancel, EventErase;

  Rocket::Core::Element* _selectedSlot;
  std::string            _savePath;
};

#endif
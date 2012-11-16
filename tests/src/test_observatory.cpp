#include "test.hpp"
#include "observatory.hpp"

void TestsObservatory(UnitTest& tester)
{
  tester.AddTest("Observatory", "Add Callback", []() -> string
  {
    Observatory::Signal<void> mysignal;
    bool                      called = false;

    mysignal.Connect([&called]() { called = true; });
    mysignal.Emit();
    if (!called)
      return ("The callback wasn't called");
    return ("");
  });

  tester.AddTest("Observatory", "Remove callback", []() -> string
  {
    Observatory::Signal<void> mysignal;
    bool                      called = false;
    Observatory::ObserverId   id;

    id = mysignal.Connect([&called]() { called = true; });
    mysignal.Disconnect(id);
    mysignal.Emit();
    if (called)
      return ("The callback wasn't removed from observer list");
    return ("");
  });

  tester.AddTest("Observatory", "Self-removing callback", []() -> string
  {
    Observatory::Signal<void> mysignal;
    Observatory::ObserverId   id;
    unsigned char             called = 0;

    id = mysignal.Connect([&id, &mysignal, &called]() { called++; mysignal.Disconnect(id); });
    mysignal.Connect([&called]() { called++; });
    mysignal.Emit();
    if (called != 2)
      return ("Something somewhere went wrong");
    return ("");
  });

  tester.AddTest("Observatory", "Observer Handler", []() -> string
  {
    Observatory::Signal<void>    mysignal;
    Observatory::ObserverHandler handler;
    bool                         called = false;

    handler.Connect(mysignal, [&called]() { called = true; });
    mysignal.Emit();
    if (!called)
      return ("The callback wasn't properly connected");
    handler.DisconnectAll();
    called = false;
    mysignal.Emit();
    if (called)
      return ("The callback wasn't removed properly");
    return ("");
  });

  tester.AddTest("Observatory", "Synchronous callback", []() -> string
  {
    Observatory::Signal<void> mysignal;
    bool                      called = false;

    mysignal.SetAsync(false);
    mysignal.Connect([&called]() { called = true; });
    mysignal.Emit();
    if (called)
      return ("The callback was directly called instead of being put on queue");
    mysignal.ExecuteRecordedCalls();
    if (!called)
      return ("The queued calls didn't get called by ExecuteRecordedCalls");
    return ("");
  });
}



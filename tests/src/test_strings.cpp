#include "test.hpp"
#include "globals.hpp"

using namespace std;

void TestsString(UnitTest& tester)
{
  //
  // Testing Humanize
  //
  tester.AddTest("humanize", "Test #1", []() -> string
  {
    string ret = humanize("coucou_monsieur");

    if (ret != "Coucou Monsieur")
      return ("Param: 'coucou_monsieur', Ret: '" + ret + "', Expected: 'Coucou Monsieur'");
    return ("");
  });

  tester.AddTest("humanize", "Test #2", []() -> string
  {
    string ret = humanize("Coucou Monsieur");

    if (ret != "Coucou Monsieur")
      return ("Param: 'Coucou Monsieur', Ret: '" + ret + "', Expected: 'Coucou Monsieur'");
    return ("");
  });

  tester.AddTest("humanize", "Test #3", []() -> string
  {
    string ret = humanize("  coucou monsieur");

    if (ret != "Coucou Monsieur")
      return ("Param: '  coucou monsieur', Ret: '" + ret + "', Expected: 'Coucou Monsieur'");
    return ("");
  });

  //
  // Testing Underscore
  //
  tester.AddTest("underscore", "Test #1", []() -> string
  {
    string ret = underscore("Coucou Monsieur");

    if (ret != "coucou_monsieur")
      return ("Param: 'Coucou Monsieur', Ret: '" + ret + "', Expected: 'coucou_monsieur'");
    return ("");
  });

  tester.AddTest("underscore", "Test #2", []() -> string
  {
    string ret = underscore("coucou_monsieur");

    if (ret != "coucou_monsieur")
      return ("Param: 'coucou_monsieur', Ret: '" + ret + "', Expected: 'coucou_monsieur'");
    return ("");
  });

  tester.AddTest("underscore", "Test #3", []() -> string
  {
    string ret = underscore("  coucou monsieur  ");

    if (ret != "coucou_monsieur")
      return ("Param: '  coucou monsieur  ', Ret: '" + ret + "', Expected: 'coucou_monsieur'");
    return ("");
  });
}



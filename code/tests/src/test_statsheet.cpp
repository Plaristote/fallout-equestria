#include "test.hpp"
#include "statsheet.hpp"

using namespace std;

void TestsStatistics(UnitTest& tester)
{
  tester.AddTest("Statistics", "Perks >> Parse available perks", []() -> string
  {
    DataTree*      tree = DataTree::Factory::JSON("data/charsheet.json");

    if (tree)
    {
      Data           data(tree);
      StatController controller(data);

      list<string> perks = controller.Model().GetAvailablePerks();
      if ((*perks.begin() == "TestSuccessPerk") && (*perks.rbegin() == "OtherPerkForFun") && perks.size() == 2)
        return ("");
      string err;
      for_each(perks.begin(), perks.end(), [&err](string str) { err += " " + str; });
      return ("Was supposed to be [ TestSuccessPerk OtherPerkForFun ], actually was [" + err + " ]");
    }
    return ("JSON failure, could not perform the test");
  });

  tester.AddTest("Statistics", "Perks >> Adding perks", []() -> string
  {
    DataTree* tree = DataTree::Factory::JSON("data/charsheet.json");

    if (tree)
    {
      Data           data(tree);
      StatController controller(data);

      data["Variables"]["Perks"] = 1;
      if (!(controller.Model().AddPerk("TestSuccessPerk")))
        return ("Failed to add 'TestSuccessPerk'");
      if ((int)(data["Variables"]["Perks"]) != 0)
        return ("Didn't decrement the amount of perk points");
      if (controller.Model().AddPerk("OtherPerkForFun"))
        return ("Added a perk even without perk points");
      return ("");
    }
    return ("JSON failure, could not perform the test");
  });

  tester.AddTest("Statistics", "Kills >> Adding kills", []() -> string
  {
    DataTree* tree = DataTree::Factory::JSON("data/charsheet.json");

   if (tree)
   {
     Data           data(tree);
     StatController controller(data);

     controller.Model().AddKill("testrace");
     if (controller.Model().GetKills("testrace") == 0)
       return ("Failed to create a race category");
     controller.Model().AddKill("testrace");
     if (controller.Model().GetKills("testrace") == 2)
       return ("");
     return ("Failed to add the kills properly");
   }
  });
}


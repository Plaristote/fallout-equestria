#include "test.hpp"
#include "directory.hpp"
#include "datatree.hpp"

using namespace std;

void TestsDirectory(UnitTest& tester)
{
  tester.AddTest("Filesystem", "Creating a directory", []() -> string
  {
    if (Directory::MakeDir("testdir"))
      return ("");
    return ("Directory::MakeDir returned false");
  });
  tester.AddTest("Filesystem", "Removing a directory", []() -> string
  {
    if (Directory::RemoveDir("testdir"))
      return ("");
    return ("Directory::RemoveDir returned false");
  });
  tester.AddTest("Filesystem", "Copying a file", []() -> string
  {
    if (!(Directory::MakeDir("testdir")))
      return ("Can't perform test without a working MakeDir");
    if (Filesystem::FileCopy("data/perks.json", "testdir/perks.json"))
    {
      DataTree* tree = DataTree::Factory::JSON("testdir/perks.json");

      if (tree)
        return ("");
      return ("Failed to properly copy file");
    }
    return ("Filesystem::FileCopy returned false");
  });
  tester.AddTest("Filesystem", "Failing at removing a directory containing files", []() -> string
  {
    string to_ret;

    if (Directory::RemoveDir("testdir"))
      to_ret = "Directory::RemoveDir returned true";
    remove("testdir/perks.json");
    Directory::RemoveDir("testdir");
    return (to_ret);
  });
}


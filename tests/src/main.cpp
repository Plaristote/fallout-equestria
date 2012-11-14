#include "globals.hpp"
#include "test.hpp"

#include "directory.hpp"

#include "datatree.hpp"

using namespace std;

void TestsString(UnitTest&);
void TestsDirectory(UnitTest&);
void TestsSerializer(UnitTest&);
void TestsJSON(UnitTest&);
void TestsStatistics(UnitTest&);

list<function<void (UnitTest&)> > TestInitializers;

int main(void)
{
  UnitTest tester;

  TestInitializers.push_back(&TestsString);
  TestInitializers.push_back(&TestsDirectory);
  TestInitializers.push_back(&TestsSerializer);
  TestInitializers.push_back(&TestsJSON);
  TestInitializers.push_back(&TestsStatistics);

  for_each(TestInitializers.begin(), TestInitializers.end(), [&tester](function<void (UnitTest& tester)> callback) { callback(tester); });

  tester.Execute();
}

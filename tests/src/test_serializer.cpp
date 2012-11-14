#include "test.hpp"
#include "serializer.hpp"

using namespace std;

void TestsSerializer(UnitTest& tester)
{
  tester.AddTest("Serializer", "Values", []() -> string
  {
    Utils::Packet in;
    unsigned int   a = 42;
    unsigned short b = 21;
    int            c = 24;
    short          d = 12;
    float          e = 4.2f;

    in << a << b << c << d << e;
    
    Utils::Packet out(in.raw(), in.size());

    out >> a >> b >> c >> d >> e;

    if (a == 42 && b == 21 && c == 24 && d == 12 && e == 4.2f)
      return ("");
    return ("Serializer miserably failed the test");
  });

  tester.AddTest("Serializer", "Strings", []() -> string
  {
    Utils::Packet in;
    string  totest("Trololo");

    in << totest;

    Utils::Packet out(in.raw(), in.size());

    out >> totest;

    if (totest == "Trololo")
      return ("");
    return ("Serializer didn't pass the test");
  });

  tester.AddTest("Serializer", "Arrays", []() -> string
  {
    Utils::Packet    in;
    std::list<float> floats;

    floats.push_back(12);
    floats.push_back(15);
    floats.push_back(18);
    in << floats;

    Utils::Packet      out(in.raw(), in.size());
    std::vector<float> floats2;

    out >> floats2;
    if (floats2[0] == 12 && floats2[1] == 15 && floats2[2] == 18 && floats2.size() == 3)
      return ("");
    return ("Something was wrong with the array");
  });

}


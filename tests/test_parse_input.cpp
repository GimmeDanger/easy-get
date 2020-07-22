#include "parse_input.h"
#include "test_runner.h"

#include <string>
#include <sstream>

void parse_input_options_test() {
  {
    // Test 1: empty [OPTIONS]
    int i = 1;
    int argc = 3;
    std::string arg0 = "name";
    std::string arg1 = "url1";
    std::string arg2 = "url2";
    char *argv[] = {arg0.data(), arg1.data(), arg2.data()};
    auto ret = parse_and_validate_options(i, argc, argv);
    ASSERT(ret.has_value());
    ASSERT(i == 1);
  }
  {
    // Test 2: unknown option
    int i = 1;
    int argc = 2;
    std::string arg0 = "name";
    std::string arg1 = "-unknown";
    char *argv[] = {arg0.data(), arg1.data()};
    auto ret = parse_and_validate_options(i, argc, argv);
    ASSERT(!ret.has_value());
    ASSERT(i == 1);
  }
  {
    // Test 2: -h option
    int i = 1;
    int argc = 3;
    std::string arg0 = "name";
    std::string arg1 = "-h";
    char *argv[] = {arg0.data(), arg1.data()};
    auto ret = parse_and_validate_options(i, argc, argv);
    ASSERT(!ret.has_value());
    ASSERT(i == 1);
  }
  {
    // Test 3: -n NUM option without NUM
    int i = 1;
    int argc = 3;
    std::string arg0 = "name";
    std::string arg1 = "-n";
    char *argv[] = {arg0.data(), arg1.data()};
    auto ret = parse_and_validate_options(i, argc, argv);
    ASSERT(!ret.has_value());
    ASSERT(i == 2);
  }
  {
    // Test 4: -n NUM option with invalid NUM
    int i = 1;
    int argc = 3;
    std::string arg0 = "name";
    std::string arg1 = "-n";
    std::string arg2 = "XXXX";
    char *argv[] = {arg0.data(), arg1.data(), arg2.data()};
    auto ret = parse_and_validate_options(i, argc, argv);
    ASSERT(!ret.has_value());
    ASSERT(i == 2);
  }
  {
    // Test 5: -n NUM option
    int i = 1;
    int argc = 3;
    std::string arg0 = "name";
    std::string arg1 = "-n";
    std::string arg2 = "212431";
    char *argv[] = {arg0.data(), arg1.data(), arg2.data()};
    auto ret = parse_and_validate_options(i, argc, argv);
    ASSERT(ret.has_value());
    ASSERT(i == 3);
  }
}

// Note: url construction is tested in test_url.cpp
void parse_input_urls_test() {
  {
    // Test 1: empty input
    int i = 1;
    int argc = 1;
    std::string arg0 = "name";
    char *argv[] = {arg0.data()};
    std::stringstream ss;
    auto ret = parse_and_validate_urls(i, argc, argv, ss);
    ASSERT (ss.str () == "No valid url was passed!\n");
    ASSERT(!ret.has_value());
    ASSERT(i == 1);
  }
  {
    // Test 1: valid url
    int i = 1;
    int argc = 2;
    std::string arg0 = "name";
    std::string arg1 = "http://google.com/hehe.jpeg";
    char *argv[] = {arg0.data(), arg1.data()};
    std::stringstream ss;
    auto ret = parse_and_validate_urls(i, argc, argv, ss);
    ASSERT(ss.str ().empty());
    ASSERT(ret.has_value());
    ASSERT(i == 2);
  }
  {
    // Test 2: valid url, invalid url
    int i = 1;
    int argc = 3;
    std::string arg0 = "name";
    std::string arg1 = "http://google.com/hehe.jpeg";
    std::string arg2 = "http://google";
    char *argv[] = {arg0.data(), arg1.data(), arg2.data()};
    std::stringstream ss;
    auto ret = parse_and_validate_urls(i, argc, argv, ss);
    ASSERT(ss.str () == "Error: invalid format: url \"http://google\" will be skipped\n");
    ASSERT(ret.has_value());
    ASSERT(i == 3);
  }
  {
    // Test 3: several valid urls
    int i = 1;
    int argc = 4;
    std::string arg0 = "name";
    std::string arg1 = "http://google.com";
    std::string arg2 = "http://mail.ru";
    std::string arg3 = "http://yandex.ru";
    char *argv[] = {arg0.data(), arg1.data(), arg2.data(), arg3.data()};
    std::stringstream ss;
    auto ret = parse_and_validate_urls(i, argc, argv, ss);
    ASSERT(ss.str ().empty());
    ASSERT(ret.has_value());
    ASSERT(i == 4);
  }
}

int main()
{
  TestRunner tr;
  RUN_TEST (tr, parse_input_options_test);
  RUN_TEST (tr, parse_input_urls_test);
  return 0;
}
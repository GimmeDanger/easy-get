#include "network/url.h"
#include "test_runner.h"

#include <vector>
#include <string>

void url_construction_test() {
  auto check_invalid = [](
      const std::string &invalid_url,
      const std::string &to_check = "invalid format") {
    try {
      url u(invalid_url);
      ASSERT(false); //< must throw
    } catch (std::exception &ex) {
      ASSERT_EQUAL(std::string(ex.what()), to_check);
    }
  };
  auto check_valid = [](
      const std::string &valid_url) {
    try {
      url u(valid_url);
    } catch (std::exception &ex) {
      ASSERT(false); //< must not catch
    }
  };
  // Test 1: invalid url: empty
  check_invalid ("");
  // Test 2: invalid url: random symbols
  check_invalid ("xxxfad@@$*!$@)($_!fdgr!$(*@$!yhfgbvcf54g89d0vdsac:xxx");
  // Test 3: invalid url: single host name
  check_invalid ("google");
  check_invalid ("google.com");
  // Test 4: invalid url: protocol must be specified
  check_invalid ("www.google.com");
  // Test 5: invalid url: https is not supported
  check_invalid ("https://google.com", "https protocol is not supported");
  // Test 6: invalid url: incorrect hostname
  check_invalid ("http://google/com");
  // Test 7: invalid url: incorrect uri
  check_invalid ("http://yandex.ru/i$-thi$-uri-valid-?????????");
  // Test 8-10: test valid
  check_valid ("http://google.com");
  check_valid ("http://go.mail.ru/search_social?fr=main&frm=main");
  check_valid ("http://ichef.bbci.co.uk/live/images/2020/7/19/037d31214d.jpg");
}

int main()
{
  TestRunner tr;
  RUN_TEST (tr, url_construction_test);
  return 0;
}
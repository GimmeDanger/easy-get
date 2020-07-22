#include "url.h"

#include <regex>
#include <stdexcept>

url::url(std::string _s) : s(std::move(_s)) {

  if (!is_valid(s)) {
    throw std::invalid_argument("invalid format");
  }

  auto protocol_end_pos = s.find("://");
  std::string protocol = s.substr(0, protocol_end_pos);
  if (protocol == to_string(protocol_t::https)) {
    throw std::invalid_argument("https protocol is not supported");
  }

  auto host_begin_pos = protocol_end_pos + 3;
  auto uri_begin_pos = s.find('/', host_begin_pos);
  host = s.substr(host_begin_pos, uri_begin_pos - host_begin_pos);
  path = (uri_begin_pos != std::string::npos ? s.substr(uri_begin_pos) : "/");
}

std::string url::to_string(url::protocol_t protocol) {
  switch (protocol) {
  case protocol_t ::http:
    return "http";
  case protocol_t ::https:
    return "https";
  default:
    throw std::invalid_argument("unknown protocol");
  }
}

bool url::operator<(const url &that) {
  return s < that.s;
}

bool url::operator==(const url &that) {
  return s == that.s;
}

[[nodiscard]] bool url::is_valid (const std::string &_s) {
  std::cmatch m;
  // details: https://stackoverflow.com/questions/3809401/what-is-a-good-regular-expression-to-match-a-url
  std::regex re(R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,4}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*))");
  return std::regex_match(_s.c_str(), m, re);
}
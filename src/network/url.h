/*
  Class represents url for http requests,
  it validates url format in constructor
 */

#pragma once

#include <string>
#include <iostream>

class url {
public:
  enum class protocol_t {
    http,
    https
  };
  static std::string to_string(protocol_t protocol);

private:
  std::string s;    //< input url
  std::string host; //< host part
  std::string path;  //< path part

public:
  explicit url(std::string _s);

  // Operator overloads
  bool operator<(const url &that);
  bool operator==(const url &that);

  // Check if url has valid format with regexp
  [[nodiscard]] static bool is_valid(const std::string &_s);

  // Getters
  [[nodiscard]] const std::string &to_string() const { return s; }
  [[nodiscard]] const std::string &get_host() const { return host; }
  [[nodiscard]] const std::string &get_path() const { return path; }

  static void help() {
    std::cout << "Expected URL format: http://HOSTNAME/[PATH]" << std::endl;
  }
};
#pragma once

#include <iostream>
#include <thread>
#include <array>

// Stores information about specified options
class options_info {
public:
  enum class yes_or_no : int {
    yes,
    no
  };
  enum class option_t : int {
    n, //< number of processes
    v, //< verbose mode
    h, //< help
    COUNT
  };

private:
  std::array<unsigned, static_cast<int> (option_t::COUNT)> values = {
      std::max(std::thread::hardware_concurrency(), 1u), 0, 0
  };
  std::array<bool, static_cast<int> (option_t::COUNT)> specified = {
      false, false, false
  };

public:
  void update(option_t opt, int new_value) {
    values[static_cast<int>(opt)] = new_value;
  }

  void update(option_t opt, yes_or_no new_value) {
    update (opt, (new_value == yes_or_no::yes ? 1 : 0));
  }

  [[nodiscard]] int get_value(option_t opt) const {
    return values[static_cast<int>(opt)];
  }

  [[nodiscard]] bool is_specified(option_t opt) const {
    return specified[static_cast<int>(opt)];
  }

  static option_t to_enum(char opt) {
    switch (opt) {
    case 'v':
      return option_t::v;
    case 'n':
      return option_t::n;
    case 'h':
      return option_t::h;
    default:
      return option_t::COUNT;
    }
  }

  static bool has_argument(option_t opt) {
    switch (opt) {
    case option_t::n:
      return true;
    case option_t::v:
    case option_t::h:
    case option_t::COUNT:
      return false;
    default:
      throw std::invalid_argument("unknown option");
    }
  }

  static bool is_valid(option_t opt) {
    return opt != option_t::COUNT;
  }

  static void help() {
    using namespace std;
    cout << "usage: easy-get [OPTIONS] URL ...";
    cout << "\n-n NUM: available number of thread";
    cout << "\n-h    : print help message and exit";
    cout << endl;
  }
};

#include "parse_input.h"

#include <algorithm>
#include <iostream>
#include <cassert>

static inline void usage()
{
  options_info::help();
  url::help();
  exit(0);
}

// Stage 1: Parse [OPTIONS] section: must go first or not set
std::optional<options_info> parse_and_validate_options(int &i, int argc, char *argv[])
{
  assert (i == 1);
  options_info opts_info;
  for (; i < argc; i++) {
    if (argv[i][0] == '-') {
      auto opt = options_info::to_enum(argv[i][1]);
      bool successfully_parsed = false;
      try {
        if (options_info::is_valid(opt) && opt != options_info::option_t::h) {
          if (!options_info::has_argument(opt)) {
            opts_info.update(opt, options_info::yes_or_no::yes);
          }
          else if (i < argc - 1) {
            opts_info.update(opt, std::stoi(std::string(argv[++i])));
          }
          successfully_parsed = true;
        }
      } catch (...) {
        // swallow possible std::stoi exception
      }
      if (!successfully_parsed)
        return {};
    } else {
      // [OPTIONS] section has ended
      break;
    }
  }
  return {opts_info};
}

// Stage 2: Parse urls section: must be at least one
std::optional<std::vector<url>> parse_and_validate_urls(int &i, int argc, char *argv[], std::ostream &os)
{
  assert(i >= 1);
  std::vector<url> urls;
  for (; i < argc; i++) {
    try {
      urls.emplace_back(url(std::string(argv[i])));
    } catch (std::exception &ex) {
      os  << "Error: " << ex.what()
          << ": url \"" << std::string(argv[i])
          << "\" will be skipped" << std::endl;
    }
  }

  if (urls.empty()) {
    os << "No valid url was passed!" << std::endl;
    return {};
  }

  // erase duplicates and return
  std::sort (urls.begin (), urls.end ());
  urls.erase(std::unique(urls.begin (), urls.end ()), urls.end ());
  return {urls};
}

std::pair<std::vector<url>, options_info>
parse_and_validate_input(int argc, char *argv[]) {
  int i = 1;
  // Stage 1: Parse [OPTIONS] section: must go first or not set
  auto opts_info = parse_and_validate_options(i, argc, argv);
  if (!opts_info.has_value ())
    usage ();
  // Stage 2: Parse urls section: must be at least one
  auto urls = parse_and_validate_urls(i, argc, argv, std::cout);
  if (!urls.has_value ())
    usage ();
  return {urls.value(), opts_info.value()};
}
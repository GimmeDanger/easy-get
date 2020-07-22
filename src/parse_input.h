#pragma once

#include "network/url.h"
#include "options_info.h"

#include <vector>
#include <utility>
#include <optional>

std::pair<std::vector<url>, options_info> parse_and_validate_input(int argc, char *argv[]);

// helpers
std::optional<options_info> parse_and_validate_options(int &i, int argc, char *argv[]);
std::optional<std::vector<url>> parse_and_validate_urls(int &i, int argc, char *argv[], std::ostream &os);
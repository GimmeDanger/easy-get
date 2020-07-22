#include "network/http_client.h"
#include "utils/paginator.h"
#include "utils/profile.h"
#include "parse_input.h"

#include <future>
#include <vector>
#include <sstream>

#define DBG_PROFILING 0

int main(int argc, char *argv[]) {

#if DBG_PROFILING
  LOG_DURATION("main");
#endif

  // Process input arguments, exit(0) if invalid
  auto [urls_to_get, opts_info] = parse_and_validate_input(argc, argv);

  // Find maximal size of batch for each thread
  const size_t max_page_size = std::max<size_t>(
      1, urls_to_get.size() / opts_info.get_value(options_info::option_t::n));

  // Send get requests in parallel (split urls into batches uniformly)
  std::vector<std::future<std::vector<std::string>>> futures;
  for (auto page : Paginate(urls_to_get, max_page_size)) {
    futures.push_back(std::async([page]{
      std::vector<std::string> ret;
      for (auto &url : page) {
        try {
          http_client client(url.get_host());
          auto verdict = client.get(url.get_path());
          std::stringstream message;
          message << "GET request for url \"" << url.to_string () << "\" ";
          message << "finished with status: " << http_client::to_string(verdict) << std::endl;
          ret.push_back (message.str());
        } catch (std::exception &ex) {
          std::stringstream message;
          message << "Error: " << ex.what () << std::endl;
          ret.push_back (message.str());
        }
      }
      return ret;
    }));
  }
  for (auto &f : futures) {
    for (auto &result : f.get())
      std::cout << result;
  }
  return 0;
}


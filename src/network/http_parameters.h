#pragma once

#include <string>
#include <stdexcept>

enum class http_method_t {
  GET,
  HEAD,
  POST,
  PUT,
  PATCH,
  DELETE,
  TRACE,
  CONNECT,
  COUNT
};

static inline std::string to_string(http_method_t method) {
  switch (method) {
  case http_method_t::GET:
    return "GET";
  default:
    throw std::runtime_error("Not supported yet");
  }
}

enum class http_version_t {
  http_0_9,
  http_1_0,
  http_1_1,
  http_2,
  COUNT
};

static inline std::string to_string(http_version_t version) {
  switch (version) {
  case http_version_t::http_1_1:
    return "HTTP/1.1";
  default:
    throw std::runtime_error("Not supported yet");
  }
}
#pragma once

#include "http_parameters.h"
#include "tcp_connection.h"
#include "utils/common.h"

#include <memory>
#include <vector>

class tcp_connection;

struct request_header {
  std::string header;
  explicit request_header(
      http_method_t method, http_version_t version,
      const std::string &host, const std::string &path);
};

class http_client
{
public:
  enum class get_code_t {
    OK,                     //< success
    EMPTY_CONN,             //< connection must be opened first
    SEND_FAILED,            //< send request returned error
    RECV_HEADER_FAILED,     //< recv header returned error
    RECV_HEADER_FORMAT_ERR, //< recv header has incorrect format
    RECV_HEADER_EMPTY_BODY, //< recv header has empty body
    RECV_BODY_ERR,          //< recv header body returned error
    BAD_STATUS_CODE,        //< recv status code != 200
    INVALID
  };

  static std::string to_string(get_code_t code);

private:
  static constexpr unsigned short default_port = 80;
  static constexpr unsigned short recv_buffer_size = 4096;
  std::vector<char> recv_buffer = std::vector<char>(recv_buffer_size);
  std::unique_ptr<tcp_connection> conn;

public:
  explicit http_client() = default;
  explicit http_client(
      const std::string &_host_name,
      unsigned short _host_port = default_port);
  DISALLOW_COPY_AND_MOVE(http_client);

public:
  get_code_t get (const std::string &path);
  tcp_connection::reset_code_t reset(
      const std::string &_host_name,
      unsigned short _host_port);

private:
  // Returns header if success, {} otherwise
  std::optional<std::string> recv_header_only ();

  // Extracts Content-Length numeric value from header
  static unsigned long long get_content_length_field (const std::string &header);

  // Extracts http status numeric value from header
  static unsigned long long get_status_code (http_version_t version, const std::string &header);

  // Generates filename to save the received data
  static std::string get_filename (const std::string &host_name, const std::string &path);
};



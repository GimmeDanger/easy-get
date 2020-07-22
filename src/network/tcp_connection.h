#pragma once

#include "utils/common.h"
#include "socket.h"

#include <memory>
#include <optional>
#include <string>

class tcp_connection
{
public:
  static constexpr unsigned short default_port = 80;

  enum class reset_code_t {
    OK,                 //< success
    SOCK_CONSTR_ERR,    //< couldn't open new socket
    SOCKET_CONN_ERR,    //< couldn't start the connection
    HOSTNAME_TO_IP_ERR, //< couldn't convert host name to ip
    IP_TO_BIN_ERR       //< couldn't convert ip to binary
  };
  static std::string to_string(reset_code_t code);

private:
  unsigned short host_port = default_port;
  std::unique_ptr<socket_t> socket;
  std::string host_name;
  std::string host_ip;

  // Set all data members to default state
  void clear ();

  // Wrapper for glibc methods, which get an information about address of a service provider
  static std::optional<std::string> get_ip_by_hostname(const std::string &_host_name);

public:
  explicit tcp_connection() = default;

  explicit tcp_connection(
      const std::string &_host_name,
      unsigned short _host_port = default_port);

  ~tcp_connection();

  DISALLOW_COPY_AND_MOVE (tcp_connection);

  // Close connection, if opened
  void close() noexcept;

  // Open new connection, close previous if needed
  [[nodiscard]] reset_code_t reset(
      const std::string &_host_name,
      unsigned short _host_port = default_port);

  // receive http response via socket
  template<socket_t::recv_mode_t mode = socket_t::recv_mode_t::not_set>
  [[nodiscard]] socket_t::recv_ret recv(char *buf, size_t buf_len);

  // send http request via socket
  [[nodiscard]] ret_code_t send(const std::string &request);

  // Getters
  [[nodiscard]] const std::string &get_host_name() const { return host_name; }
  [[nodiscard]] const std::string &get_host_ip() const { return host_ip; }
  [[nodiscard]] unsigned short get_host_port() const { return host_port; }
};
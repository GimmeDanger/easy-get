#include "http_client.h"
#include "tcp_connection.h"
#include "socket.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

request_header::request_header(
    http_method_t method, http_version_t version,
    const std::string &host, const std::string &path)
{
  std::string line_end = "\r\n";
  std::stringstream ss;
  ss << to_string(method) << " " << path << " "
     << to_string(version) << line_end;
  ss << "Host: " << host << line_end;
  ss << "Accept: */*" << line_end;
  ss << "Connection: close" << line_end;
  ss << line_end;
  header = ss.str ();
}

http_client::http_client (
    const std::string &_host_name,
    unsigned short _host_port)
{
  conn = std::make_unique<tcp_connection>(_host_name, _host_port);
}

tcp_connection::reset_code_t http_client::reset(
    const std::string &_host_name,
    unsigned short _host_port)
{
  conn = std::make_unique<tcp_connection> ();
  return conn->reset (_host_name, _host_port);
}

http_client::get_code_t http_client::get (const std::string &path)
{
  if (!conn)
    return get_code_t::EMPTY_CONN;

  request_header request (http_method_t::GET, http_version_t::http_1_1, conn->get_host_name(), path);
  if (conn->send (request.header) != ret_code_t::OK)
    return get_code_t::SEND_FAILED;

  auto opt_headers = recv_header_only ();
  if (!opt_headers.has_value ())
    return get_code_t::RECV_HEADER_FAILED;
  std::string http_headers = std::move (opt_headers.value());

  size_t status_code = 0;
  TRY_INIT_WITH_EXPR(status_code, [&http_headers]() {
    return static_cast<size_t> (get_status_code (http_version_t::http_1_1, http_headers));
  }, get_code_t::RECV_HEADER_FORMAT_ERR);
  if (status_code != 200)
    return get_code_t::BAD_STATUS_CODE;

  // Receive request body and save to disk
  std::string filename = get_filename (conn->get_host_name (), path);
  std::ofstream fout(filename, std::ios_base::out | std::ios_base::binary);
  if (fout) {
    recv_buffer.resize(recv_buffer_size);
    std::fill(recv_buffer.begin(), recv_buffer.end(), 0);
    for(;;) {
      auto ret = conn->recv (recv_buffer.data(), recv_buffer.size());
      if (ret.code == socket_t::recv_code_t::ERROR)
        return get_code_t::RECV_BODY_ERR;
      fout.write(recv_buffer.data(), ret.total_recv_size);
      if (ret.code == socket_t::recv_code_t::OK)
        break;
    }
    fout.close ();
  }
  return get_code_t::OK;
}

std::optional<std::string> http_client::recv_header_only ()
{
  if (!conn)
    return {};

  /* MSG_PEEK
    This flag causes the receive operation to return data from the
    beginning of the receive queue without removing that data from
    the queue.  Thus, a subsequent receive call will return the
    same data.*/
  recv_buffer.resize(recv_buffer_size);
  std::fill(recv_buffer.begin(), recv_buffer.end(), 0);
  auto recv_ret = conn->recv<socket_t::recv_mode_t::msg_peek>(recv_buffer.data(), recv_buffer.size());
  if (recv_ret.code == socket_t::recv_code_t::ERROR || recv_ret.total_recv_size == 0)
    return {};

  // Check format
  std::string header (recv_buffer.data(), recv_ret.total_recv_size);
  size_t pos = header.find ("\r\n\r\n");
  if (pos == std::string::npos)
    return {};
  header = header.substr(0, pos + 4);

  // now receive header again, to put the "carriage" just at the beginning of body
  recv_ret = conn->recv(recv_buffer.data(), header.size());
  if (recv_ret.code == socket_t::recv_code_t::ERROR || recv_ret.total_recv_size == 0)
    return {};

  return {header};
}

unsigned long long http_client::get_content_length_field (const std::string &header) {
  std::string needle = "Content-Length: ";
  size_t pos = header.find(needle);
  if (pos == std::string::npos)
    return 0; //< not found
  size_t number_start = pos + needle.size();
  size_t number_end = header.find ('\r', number_start);
  return std::stoll (header.substr (number_start, number_end));
}

unsigned long long http_client::get_status_code (http_version_t version, const std::string &header) {
  size_t number_start = ::to_string(version).size() + 1/*sep*/;
  size_t number_end = header.find (' ', number_start);
  return std::stoll (header.substr (number_start, number_end));
}

std::string http_client::to_string(http_client::get_code_t code) {
  switch (code) {
  case get_code_t::OK:
    return "OK, file was saved";
  case get_code_t::EMPTY_CONN:
    return "error, connection must be opened first";
  case get_code_t::SEND_FAILED:
    return "error: send request failed";
  case get_code_t::RECV_HEADER_FAILED:
    return "error: receiving of header failed";
  case get_code_t::RECV_HEADER_FORMAT_ERR:
    return "error: received message has incorrect header";
  case get_code_t::RECV_HEADER_EMPTY_BODY:
    return "error: received message has empty body";
  case get_code_t::RECV_BODY_ERR:
    return "error: receiving of body failed";
  case get_code_t::BAD_STATUS_CODE:
    return "error: received status code != 200";
  default:
    throw std::invalid_argument("unsupported get code");
  }
}

std::string http_client::get_filename (const std::string &host_name, const std::string &path) {
  // stick together again
  std::string filename = "easy_get_filename_";
  filename += host_name + path;
  // replace slashes for valid filename
  std::transform (filename.begin(), filename.end(), filename.begin(), [](char ch) {
    if (ch == '/')
      return '_';
    else
      return ch;
  });
  // save as .html, if file format is not contained in url
  auto format_pos = path.find('.');
  if (format_pos == std::string::npos)
    filename += ".html";
  return filename;
}
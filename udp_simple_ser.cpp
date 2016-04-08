/* Copyright 2016 - Alessandro Fabbri */

/***************************************************************************
This file is part of boost_testing.

boost_testing is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

boost_testing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with boost_testing. If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <boost/algorithm/string.hpp>

#include "udp_lib.hpp"
#include "md5.h"

int main(){
  boost::asio::io_service ioService;
  UDPConnection udp_con(ioService, "localhost", "12345", "localhost", "54321");
  std::cout << "Awaiting connections..." << std::endl << std::endl;

  std::array<char, 128> buffer;
  boost::system::error_code err;
  int msg_counter = 0;
  std::string message, client_id, text, hash;

  for (;;) {
    // receive message from client
    udp_con.recv(boost::asio::buffer(&buffer[0], buffer.size()), err);
    message = std::string(buffer.data(), udp_con.len_recv);
    std::cout << "Message #" << ++msg_counter << " received (" << udp_con.len_recv << " bytes) : " << message << std::endl;

    // extract data from message
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, message, boost::algorithm::is_any_of(":"), boost::algorithm::token_compress_on);
    client_id = tokens[0];
    text = tokens[1];
    hash = tokens[2];

    // check MD5hash and send ack to client
    std::string md5string = make_md5hash(text);
    if (hash == std::string(md5string)) {
      std::cout << "Hash match! Sending ACK to client" << std::endl;
      udp_con.send(std::to_string(udp_con.len_recv));
    }
  }

  return 0;
}
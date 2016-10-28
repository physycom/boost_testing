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

int main(int argc, char** argv){
  std::string loc_addr, loc_port, rem_addr, rem_port;
  if (argc == 5) {
    loc_addr = argv[1];
    loc_port = argv[2];
    rem_addr = argv[3];
    rem_port = argv[4];
  }
  else if (argc == 2 && std::string(argv[1]) == "def") {
    loc_addr = "127.0.0.1";
    loc_port = "11111";
    rem_addr = "127.0.0.1";
    rem_port = "22222";
  }
  else {
    std::cout << "Usage: " << argv[0] << " local_addr local_port remote_addr remote_port" << std::endl;
    std::cout << "       " << " Set up UDP server listening on local_addr:local_port and sending ACK message to remote_addr:remote_port" << std::endl;
    std::cout << "Usage: " << " def" << std::endl;
    std::cout << "       " << " Set up UDP server listening on 127.0.0.1:11111 and sending ACK message to 127.0.0.1:22222" << std::endl;
    exit(1);
  }

  std::cout << "**************************" << std::endl;
  std::cout << "**** BOOST UDP SERVER ****" << std::endl;
  std::cout << "**************************" << std::endl;

  boost::asio::io_service ioService;
  UDPConnection udp_con(ioService, loc_addr, loc_port, rem_addr, rem_port );

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
    boost::algorithm::split(tokens, message, boost::algorithm::is_any_of(":"), boost::algorithm::token_compress_off);
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
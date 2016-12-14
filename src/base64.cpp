/* Copyright 2015 - Alessandro Fabbri */

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

#include <iostream>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

////////// BASE64 encode/decode
std::string decode64(const std::string &val) {
  using namespace boost::archive::iterators;
  using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
  return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
    return c == '\0';
  });
}

std::string encode64(const std::string &val) {
  using namespace boost::archive::iterators;
  using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
  auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
  return tmp.append((3 - val.size() % 3) % 3, '=');
}

////////// A sample struct
struct mystruct {
  int n;
  float f;
  double x;
};


int main() {
  //////// PLAIN TEXT CASE
  std::string plain_text = "a very long message to convert";
  std::cout << "Plain text      : " << plain_text << std::endl;
  std::cout << "Base64 encoding : " << encode64(plain_text) << std::endl;
  std::cout << "Base64 decoding : " << decode64(encode64(plain_text)) << std::endl << std::endl;

  //////// BINARY CASE
  struct mystruct s = {123, 4.5678f, 9.8765e-13};
  // Convert struct to byte buffer
  unsigned char * buf = new unsigned char[sizeof s];
  memcpy(buf, &s, sizeof s);
  // Store byte buffer to string
  std::string plain_binary;
  for (size_t i = 0; i < sizeof s; ++i) {
    plain_binary.push_back(buf[i]);
  }
  // Encode and decode
  std::string encoded_binary = encode64(plain_binary);
  std::string decoded_binary = decode64(encode64(plain_binary));
  // Reconstruct byte buffer
  for (size_t i = 0; i < decoded_binary.size(); ++i) {
    buf[i] = decoded_binary[i];
  }
  // Use a struct pointer to access data
  struct mystruct * ptr = (struct mystruct *) buf;
  // Display results
  std::cout << "Plain binary : " << plain_binary << std::endl;
  std::cout << "Base64 encoding : " << encoded_binary << std::endl;
  std::cout << "Base64 decoding : " << decoded_binary << std::endl;
  std::cout << "Check struct    : " << s.n << " = " << ptr->n << " - " << s.f << " = " << ptr->f << " - " << s.x << " = " << ptr->x << std::endl;
  
  return 0;
}

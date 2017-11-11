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
#include <sstream>

#include <boost/exception/all.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace boost::property_tree;

constexpr char good_xml[] = R"(
<xml>
  <id>a_meaningful_name</id>
  <float>1.733</float>
  <answer>
    <str>"42"</str>
    <integer>42</integer>
    <double>42.0</double>
  </answer>
</xml>
)";

constexpr char bad_xml[] = R"(
<blabla>
  not good
)";


int main() {

  ptree root;
  stringstream ss;

  // try bad json
  cout << "Testing bad formed XML" << endl;
  ss.str(bad_xml);
  try {
    read_xml(ss, root);
  }
  catch (const boost::property_tree::xml_parser::xml_parser_error& e) {
    cout << "Invalid XML : " << e.what() << endl;  // Never gets here
  }
  catch (std::exception & e) {
    cout << "Generic error : " << e.what() << endl;
    exit(1);
  }

  // try good json
  cout << "Testing good XML" << endl;
  ss.str(good_xml);
  try {
    read_xml(ss, root);
  }
  catch (const boost::property_tree::xml_parser::xml_parser_error& e) {
    cout << "Invalid XML : " << e.what() << endl;  // Never gets here
    exit(1);
  }
  catch (std::exception & e) {
    cout << "Generic error : " << e.what() << endl;
    exit(2);
  }

  cout << "Testing non existing key : ";
  try {
    auto def = root.get_child("nonono");
  }
  catch (std::exception& e) {
    cout << e.what() << endl;
  }

  cout << "Looping xml values" << endl;
  for (ptree::value_type &node : root) {
    std::string key = node.first;
    std::string value = (node.second.size() ? "child node(s) :" : node.second.data());
    cout << (key == "" ? "nokey" : key) << "(" << node.second.size() << ") : " << value << endl;

    if (node.second.size()) {
      for (ptree::value_type &child : node.second) {
        key = child.first;
        value = (child.second.size() ? "child node(s) :" : child.second.data());
        cout << "\t" << (key == "" ? "nokey" : key) << "(" << child.second.size() << ") : " << value << endl;

        if (child.second.size()) {
          for (ptree::value_type &child2 : child.second) {
            key = child2.first;
            value = (child2.second.size() ? "child node(s) :" : child2.second.data());
            cout << "\t\t" << (key == "" ? "nokey" : key) << "(" << child2.second.size() << ") : " << value << endl;
          }
        }
      }
    }
  }

  return 0;
}

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
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace boost::property_tree;

constexpr char good_json[] = R"(
{
  "values"  : 
  {
    "bool" : true,
    "null" : null
  },
  ""  :  3,
  "age"  :  31,
  "axis" : [ "x", "y", "z" ],
  "ax" : 1.733
}
)";

constexpr char bad_json[] = R"(
{
  "answer"   : "42",
]
}
)";


int main() {

  ptree root;
  stringstream ss;

  // try bad json
  cout << "Testing bad formed JSON" << endl;
  ss.str(bad_json);
  try {
    read_json(ss, root);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e) {
    cout << "Invalid JSON : " << e.what() << endl;  // Never gets here
  }
  catch (std::exception & e) {
    cout << "Generic error : " << e.what() << endl;
    exit(1);
  }

  // try good json
  cout << "Testing good JSON" << endl;
  ss.str(good_json);
  try {
    read_json(ss, root);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e) {
    cout << "Invalid JSON : " << e.what() << endl;  // Never gets here
  }
  catch (std::exception & e) {
    cout << "Generic error : " << e.what() << endl;
    exit(1);
  }
  auto bo = root.get<bool>("values.bool", false);
  if (bo == true) cout << "Testing bool : OK" << endl;

  auto three = root.get<int>("", 1);
  if (three == 3) cout << "Testing null key : OK" << endl;

  auto x = root.get<double>("", 4.44);
  if (x == 1.733) cout << "Testing double : OK" << endl;

  auto def = root.get<int>("non existing key", 12);
  if (def == 12) cout << "Testing default value : OK" << endl;

  cout << "Testing non existing key : ";
  try {
    auto def = root.get_child("nonono");
  }
  catch (std::exception& e) {
    cout << e.what() << endl;
  }

  cout << "Looping json values" << endl;
  for (ptree::value_type &node : root) {
    std::string key = node.first;
    std::string value = node.second.data();
    cout << (key == "" ? "nokey" : key) << "(" << node.second.size() << ") : " << value << endl;

    if (node.second.size()) {
      for (ptree::value_type &child : node.second) {
        key = child.first;
        value = child.second.data();
        cout << "\t" << (key == "" ? "nokey" : key) << "(" << child.second.size() << ") : " << value << endl;
      }
    }
  }

  return 0;
}

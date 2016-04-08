/* Copyright 2016 - Stefano Sinigardi, Alessandro Fabbri */

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

#pragma once

/*
How to use this MD5Library:

unsigned char digest[16];
const char* mystring = "Hello World";
MD5_CTX context;
MD5_Init(&context);
MD5_Update(&context, mystring, strlen(mystring));
MD5_Final(digest, &context);

This will give you back an integer representation of the hash. You can then turn this into a hex representation if you want to pass it around as a string
char md5string[33];
for(int i = 0; i < 16; ++i)
sprintf(&md5string[i*2], "%02x", (unsigned int)digest[i]);
md5string[32]='\0';

Otherwise you can use the wrapper
std::string text = "Hello World!"
std::string md5string = make_md5hash(text);
*/

#include <string>
#include <cstring>
#include <cstdint>

typedef struct {
  uint32_t lo, hi;
  uint32_t a, b, c, d;
  unsigned char buffer[64];
  uint32_t block[16];
} MD5_CTX;


static const void *body(MD5_CTX *, const void *, uint32_t );

void MD5_Init(MD5_CTX *);

void MD5_Update(MD5_CTX *, const void *, uint32_t );

void MD5_Final(unsigned char *, MD5_CTX *);

std::string make_md5hash(std::string);


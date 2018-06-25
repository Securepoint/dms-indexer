/*
 base32.cpp and base32.h

 Copyright (C) 2014 Securepoint GmbH

 James Fuge james.fuge@securepoint.de
 */

#include <string>

std::string base32_encode(unsigned char const *bytes_to_encode,
  unsigned int in_length);
std::string base32_decode(std::string const &s);

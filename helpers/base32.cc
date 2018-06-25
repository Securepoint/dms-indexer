/*
 base32.cpp and base32.h

 Copyright (C) 2014 Securepoint GmbH

 James Fuge james.fuge@securepoint.de
 */

#include "base32.h"
#include <string.h>
#include <stdexcept>
using namespace std;

static const
      std::string base32_chars = "abcdefghijklmnopqrstuvwxyz2345678";

/**
 * Encodes an unsigned bytes array to base32
 */
std::string base32_encode(unsigned char const *bytes_to_encode,
  unsigned int in_length)
{
  std::string ret_val = "";
  int num_in_chars = 5;         // Read 5 input chars
  int num_out_chars = 8;        // Output 8 chars
  int i = 0;
  int j = 0;
  unsigned char input_array[num_in_chars];
  unsigned char output_array[num_out_chars];

  // Calculate the number of characters we will need
  int num_5bits = 1 + (((in_length * 8) - 1) / 5);      // in_length fits into 5 bits x times
  int remainder = num_5bits % 8;

  if (remainder != 0) {
    num_5bits = num_5bits + 8 - remainder;
  }

  try {
    ret_val.reserve(num_5bits);
  }
  catch(const std::length_error & le) {
    in_length = 0;
  }
  catch(std::bad_alloc & ba) {
    in_length = 0;
  }

  while (in_length--) {

    input_array[i++] = *(bytes_to_encode++);

    if (i == num_in_chars || in_length == 0) {
      // If we ended with less than 5 characters,
      // fill the remaining array values with 0
      if (in_length == 0) {
        for (j = i; j < num_in_chars; j++) {
          input_array[j] = 0;
        }
      }

      output_array[0] = input_array[0] >> 3;
      output_array[1] =
        ((input_array[0] & 0x7) << 2) + ((input_array[1]) >> 6);
      output_array[2] = ((input_array[1] >> 1) & 0x1f);
      output_array[3] = ((input_array[1] & 0x1) << 4) + (input_array[2] >> 4);
      output_array[4] =
        ((input_array[2] & 0xf) << 1) + ((input_array[3] & 0x80) >> 7);
      output_array[5] = ((input_array[3] & 0x7c) >> 2);
      output_array[6] = ((input_array[3] & 0x3) << 3) + (input_array[4] >> 5);
      output_array[7] = ((input_array[4] & 0x1f));

      // If we ended with less than 5 characters,
      // set padding appropriately
      if (in_length == 0) {
        if (i == 1) {
          output_array[2] = output_array[3] = output_array[4] =
            output_array[5] = output_array[6] = output_array[7] = 32;
        } else if (i == 2) {
          output_array[4] = output_array[5] = output_array[6] =
            output_array[7] = 32;
        } else if (i == 3) {
          output_array[5] = output_array[6] = output_array[7] = 32;
        } else if (i == 4) {
          output_array[7] = 32;
        }
      }

      for (i = 0; i < num_out_chars; i++) {
        ret_val += base32_chars[output_array[i]];
      }

      i = 0;
    }
  }

  return ret_val;
}

/**
 * Decodes a base32 encoded string
 */
std::string base32_decode(std::string const &encoded_string)
{
  int in_len = encoded_string.size();
  int i = 0;
  int in_counter = 0;
  int num_in_chars = 8;         // Read 8 input chars
  int num_out_chars = 5;        // decoded as 5 output chars
  unsigned char input_array[num_in_chars];
  unsigned char output_array[num_out_chars];
  std::string ret_val = "";

  // Check the string length is a multiple of 8
  if (in_len % 8 > 0) {
    return "";
  }
  // Calculate the number of characters we will need
  int num_8bits = 1 + (((in_len * 5) - 1) / 8);

  try {
    ret_val.reserve(num_8bits);
  }
  catch(const std::length_error & le) {
    in_len = 0;
  }
  catch(std::bad_alloc & ba) {
    in_len = 0;
  }

  while (in_len--) {
    input_array[i++] = encoded_string[in_counter];
    in_counter++;

    if (i == num_in_chars) {

      for (i = 0; i < num_in_chars; i++) {
        input_array[i] = base32_chars.find(input_array[i]);

        // If the input character wasn't in our array of characters, return an empty string.
        if (input_array[i] > 32) {
          ret_val = "";
          goto escape;
        }
      }

      output_array[0] = (input_array[0] << 3) + (input_array[1] >> 2);
      output_array[1] =
        ((input_array[1] & 0x3) << 6) + (input_array[2] << 1) +
        (input_array[3] >> 4);
      output_array[2] = ((input_array[3] & 0xf) << 4) + (input_array[4] >> 1);
      output_array[3] =
        ((input_array[4] & 0x1) << 7) + (input_array[5] << 2) +
        (input_array[6] >> 3);
      output_array[4] = ((input_array[6] & 0x7) << 5) + input_array[7];

      ret_val += output_array[0];

      if (input_array[7] != 32) {
        ret_val += output_array[1];
        ret_val += output_array[2];
        ret_val += output_array[3];
        ret_val += output_array[4];
      } else if (input_array[6] != 32) {
        ret_val += output_array[1];
        ret_val += output_array[2];
        ret_val += output_array[3];
      } else if (input_array[4] != 32) {
        ret_val += output_array[1];
        ret_val += output_array[2];
      } else if (input_array[3] != 32) {
        ret_val += output_array[1];
      }

      i = 0;
    }
  }

escape:
  return ret_val;
}

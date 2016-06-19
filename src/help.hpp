#pragma once
#include <string>

std::string help =
  "\nUsage: crypto [OPTION]...\n"
  "     -a \"string\"      Automatically attempt to solve substitution ciphers.\n"
  "     -d \"string\"      Decrypts a substitution cipher. Requires -k.\n"
  "     -e \"string\"      Encrypts a message using the key given by the -k option.\n"
  "                      If no key is given, a key will be randomly generated.\n"
  "     -h               Display this message.\n";

#pragma once
#include <array>
#include <numeric>
#include <stdio.h>
#include <string>
#include "crypto_utils.h"

string char_pattern(string in);

//[in] must be UPPERCASE
array<int, 26> countchars(string in);
void print_histogram(string in);
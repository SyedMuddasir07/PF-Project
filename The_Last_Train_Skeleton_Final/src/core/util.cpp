#include "util.h"
#include <cctype>
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
// Primitive helper utilities, clamping(Go below or above range), and screen control()
int clampInt(int val, int minVal, int maxVal)
{
    if (val < minVal){
        return minVal;
    }
    if (val > maxVal){
        return maxVal;
    }
    return val;
}

void trimWhitespace(char* str)
{
    int len = strlen(str); // removing the spaces at the start and end of the string array
    // Trim leading whitespace
    int start = 0;  
    while (start < len && isspace(str[start])) { 
        start++; 
    }
    // Shift the string to remove leading whitespace
    for (int i = 0; i < len - start; i++) {
        str[i] = str[i + start];
    }
    str[len - start] = '\0';
    // Trim trailing whitespace
    int end = strlen(str) - 1;
    while (end >= 0 && isspace(str[end])) {
        str[end] = '\0';
        end--;
    }
}

bool stringEquals(const char* a, const char* b)
{
    return strcmp(a, b) == 0; // Compare two C-style strings for equality
}

void printHeader(const char* title)
{
    cout << "\n=== " << title << " ===\n"; // Print a formatted header with the given title
}
void clearScreen()
{
    // Clear the console screen using ANSI escape codes
    printf("\033[2J\033[H");
}


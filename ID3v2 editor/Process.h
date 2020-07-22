#pragma once

#include "MP3.h"

bool checkArgs(const char* argv[], size_t argc);
void show(const MP3& mp3);
void get(const MP3& mp3, const char* frameName);
void set(MP3& mp3, const char* filename, const char* frameName, const char* frameValue);
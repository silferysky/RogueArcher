#pragma once
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

bool WriteToFile(std::string FileName, const char* DataTitle, std::string DataToChange);
bool WriteToFilePretty(const char* FileName, const char* DataTitle, const char* DataToChange);
bool ReadFromFile(const char* FileName);
bool IfFileExists(const char* FileName);

#pragma once

#include<cassert>
#include<stdint.h>
#include<string>
#include<sstream>
#include<iostream>

#define assertm(msg,cond) assert(((msg),(cond)))



using id_t = uint64_t;
using entryName_t = std::string;
using shoolName_t = std::string;
using category_t = std::string;
using competitor_t = std::string;
using advisor_t = std::string;

struct BasicInformation
{
    entryName_t entryName;
    shoolName_t school;
    category_t category;
    competitor_t competitor;
    advisor_t advisor;
};
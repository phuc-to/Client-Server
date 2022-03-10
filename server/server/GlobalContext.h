/**
 @file auth.h
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 1.0
 */

#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class GlobalContext
{
public:
	GlobalContext() {
		totalRPC = 0;
		breakfast = 0;
		lunch = 0;
		dinner = 0;
	}

private:
	int totalRPC;
	int breakfast;
	int lunch;
	int dinner;
};


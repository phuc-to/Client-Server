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
		mealTime = new int[mealTimeNumber];
		for (int i = 0; i < mealTimeNumber; i++) {
			mealTime[i] = 0;
		}
		mealTimeName = new string[mealTimeNumber]{ "breakfast", "lunch", "server" };
	}

	void incRPC() {
		totalRPC++;
	}

	void incBreakfast() {
		mealTime[0]++;
	}

	void incLunch() {
		mealTime[1]++;
	}

	void incDinner() {
		mealTime[2]++;
	}

	string mostAsked() {
		int n = 0;
		for (int i = 1; i < mealTimeNumber; i++)
			if (mealTime[i] > mealTime[n])
				n = i;
		return mealTimeName[n];
	}

private:
	const int mealTimeNumber = 3;
	int totalRPC;
	int* mealTime;
	string* mealTimeName;
};


/**
 @file  Meal class implementation file
 @for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 2/12/22
 @version 1.0
 */

#include "Meal.h"

Meal::Meal() {};

Meal::~Meal() {};

Meal::Meal(string n, string t, string c) {
	name = n;
	time = t;
	cuisine = c;
}

string Meal::getName() {
	return name;
}

string Meal::getTime() {
	return time;
}

string Meal::getCuisine() {
	return cuisine;
}








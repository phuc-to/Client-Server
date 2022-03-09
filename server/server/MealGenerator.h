/**
 @file  MealGenerator class header file for socket programming project. Serves as Meal database.
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 2.0
 */
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Meal.h"

using namespace std;

class MealGenerator
{
public:

		/** Creates and instance of MealGenerator. Initializes the list of meals to the
		default options in the Meal Generator.*/
	    MealGenerator();

		/** Destructor for instance of MealGenerator.*/
		~MealGenerator();

		/**Adds a Meal to the list of meals in the Meal Generator.

		@param name, the name of the meal.
		@param time, the time when the meal is eaten (breakfast, lunch, or dinner).
		@param cuisine, the type of cuisine for the meal
		@return true, if meal did not already exist in Meal Generator. Else, false. 
		*/
		bool addMeal(string, string, string);

		/** Returns the name of random meal from the list of meals in Meal Generator.

		@returns name, the name of the random meal.
		*/
		string getRandomMeal();

		/** Returns the name of random meal from the list of meals in Meal Generator with
		the time from the input parameter, or returns a statement saying that there were
		no meals with the time from the input parameter.

		@param time, the time when the meal is eaten (breakfast, lunch, or dinner).
		@return name, name of the random meal.
		*/
		string getRandomMealByTime(string);

		/** Returns the name of random meal from the list of meals in Meal Generator with
		the cuisine type from the input parameter, or returns a statement saying that there
		were no meals with the cuisine type from the input parameter.

		@param cuisine, thecuisine type of the meal.
		@return name, name of the random meal.
		*/
		string getRandomMealByCuisine(string);

		/** Returns the current TIMES vector. 

		@return TIMES, vector holding time of day selections. 
		*/
		vector<string> const getTimesOfDay();

		/** Returns the current CUISINES vector.

		@return CUISINES, vecot holding list of cuisines.
		*/
		vector<string> const getCuisines();


private:
	int numMeals;      // Holds size of Meals.  

	vector<Meal> meals; // Holds all Meals stored in the Meal Generator object.

	const vector<string> TIMES = { "breakfast", "lunch", "dinner" }; // Holds Time of Day selections. 

	vector<string> CUISINES;  // Holds list of cuisines to choose from. 


	/** Private static variable used to ensure that  the seed value for the sRand
	object is only set once.
	*/
	static bool isSeeded;

	/** getRandomMeal helper function to generate random int and seed sRand object.
	@return int, random number between 1 and size of the Meal vector.
	*/
	int randomize(int);
};

/**
 @file  MealGenerator class header file for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 2/12/22
 @version 1.0
 */
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Meal.h"

using namespace std;

class MealGenerator
{

private:
	vector<Meal> meals;

	/*
    Function:   randomize
	Purpose:    Private function to help the getRandomMeal function
	            return the name of a random meal.
	Parameter:  the number of available meal options
	Returns:    a random number within the number of available 
	            meal options.
	*/
	int randomize(int);

	
	/*
	Private static variable used to ensure that we only set 
    the seed value for the rand() function once.
	*/
	static bool isSeeded;

public:
	 /*
     Default constructor.
     Initializes the list of meals to the default options in the
     Meal Generator.
     */
	    MealGenerator();

		~MealGenerator();

		/*
		Function:   addMeal
		Purpose:    adds a meal to the list of meals in the Meal Generator.
		Parameters: the name of the meal
				    the time when the meal is eaten (breakfast, lunch, or dinner)
				    the type of cuisine for the meal
	    */
		void addMeal(string, string, string);

		/*
		Function:   getRandomMeal
		Purpose:    returns the name of random meal from the list
		            of meals in Meal Generator.
		Returns:    the name of the random meal;
		*/
		string getRandomMeal();

		/*
		Function:   getRandomMealByTime
		Purpose:    returns the name of random meal from the list
		            of meals in Meal Generator with the time from the input parameter, 
					or returns a statement saying that there were no meals with the
	                time from the input parameter.
					// QUESTION FOR GROUP - is this how we want to handle there
					// being no meals matching the input cuisine/time?
		Parameter:  the time when the meal is eaten (breakfast, lunch, or dinner).
		Returns:    the name of the random meal.
		*/
		string getRandomMealByTime(string);

		/*
        Function:   getRandomMealByCuisine
		Purpose:    returns the name of random meal from the list
		            of meals in Meal Generator with the cuisine type
					from the input parameter, or returns a statement 
					saying that there were no meals with the cuisine 
					type from the input parameter.
		Parameters: the cuisine type of the meal.
		Returns:    the name of the random meal.
        */
		string getRandomMealByCuisine(string);

		//Print function for testing
		void print();
};

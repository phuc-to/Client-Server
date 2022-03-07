/**
 @file  MealGenerator class implementation file
 @for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 2/12/22
 @version 1.0
 */
#include <string>
#include <iostream>
#include "MealGenerator.h"

bool MealGenerator::isSeeded = false;

MealGenerator::MealGenerator() {

	// Add cuisines to CUISINE vector. 
	this->CUISINES.push_back("american");
	this->CUISINES.push_back("mexican");
	this->CUISINES.push_back("portuguese");
	this->CUISINES.push_back("indian");
	this->CUISINES.push_back("japanese");
	this->CUISINES.push_back("vietnamese");
	this->CUISINES.push_back("italian");
	this->CUISINES.push_back("thai");
	this->CUISINES.push_back("russian");
	this->CUISINES.push_back("russian");
	this->CUISINES.push_back("chinese");

	// Store Times of Days in const vars. 
	const string BREAKFAST = this->TIMES[0];
	const string LUNCH = this->TIMES[1];
	const string DINNER = this->TIMES[2];

	// Populate initial Meal list. 
	// Breakfast
	addMeal("Scrambled eggs", BREAKFAST, "american");
	addMeal("Avocado toast", BREAKFAST, "american");
	addMeal("Breakfast burritos", BREAKFAST, "american");
	addMeal("Sausage egg and cheese mcgriddle", BREAKFAST, "american");
	addMeal("Buttermilk pancakes", BREAKFAST, "american");
	addMeal("Chicken and waffles", BREAKFAST, "american");
	addMeal("Eggs benedict", BREAKFAST, "american");

	addMeal("Huevos Rancheros", BREAKFAST, "mexican");
	addMeal("Papas con Chorizo", BREAKFAST, "mexican");
	addMeal("Chilaquiles", BREAKFAST, "mexican");
	addMeal("Migas", BREAKFAST, "mexican");
	addMeal("Churros waffles", BREAKFAST, "mexican");

	addMeal("Pastel del nata", BREAKFAST, "portuguese");
	addMeal("portuguese baked eggs", BREAKFAST, "portuguese");
	addMeal("Broa de milho", BREAKFAST, "portuguese");
	addMeal("Bolo do Caco", BREAKFAST, "portuguese");

	addMeal("Persarattu Upma", BREAKFAST, "indian");
	addMeal("Khura", BREAKFAST, "indian");
	addMeal("Litti Chokha", BREAKFAST, "indian");
	addMeal("Muthiya", BREAKFAST, "indian");
	addMeal("Dhokla", BREAKFAST, "indian");
	addMeal("Chakuli Pitha", BREAKFAST, "indian");

	addMeal("Tamagoyaki (rolled omelette", BREAKFAST, "japanese");
	addMeal("Omurice (omelette rice)", BREAKFAST, "japanese");
	addMeal("Dashi eggs with rice", BREAKFAST, "japanese");

	addMeal("Xoi", BREAKFAST, "vietnamese");
	addMeal("Banh cuon", BREAKFAST, "vietnamese");
	addMeal("Chao Long (rice porrage)", BREAKFAST, "vietnamese");

	// Lunch
	addMeal("Onigiri", LUNCH, "japanese");
	addMeal("Okanomiyaki", LUNCH, "japanese");
	addMeal("Takoyaki", LUNCH, "japanese");

	addMeal("Soup, salad, and breadsticks", LUNCH, "italian");
	addMeal("Pizza", LUNCH, "italian");
	addMeal("Bruschetta", LUNCH, "italian");

	addMeal("Grilled cheese and tomato soup", LUNCH, "american");
	addMeal("Philly cheesesteak", LUNCH, "american");
	addMeal("BLT sandwich", LUNCH, "american");
	addMeal("Cobb salad", LUNCH, "american");
	addMeal("Tater tots", LUNCH, "american");

	addMeal("Banh mi", LUNCH, "vietnamese");
	addMeal("Banh xeo", LUNCH, "vietnamese");
	addMeal("Bot chien", LUNCH, "vietnamese");
	addMeal("Bun cha gio", LUNCH, "vietnamese");
	addMeal("Goi cuon", LUNCH, "vietnamese");

	addMeal("Butter chicken", LUNCH, "indian");
	addMeal("Dal makhani", LUNCH, "indian");
	addMeal("Saag paneer", LUNCH, "indian");
	addMeal("Chana masala", LUNCH, "indian");

	// Dinner
	addMeal("Pho", DINNER, "vietnamese");
	addMeal("Cha ca", DINNER, "vietnamese");
	addMeal("Cao lau", DINNER, "vietnamese");
	addMeal("com hen", DINNER, "vietnamese");
	addMeal("Bun bo hue", DINNER, "vietnamese");

	addMeal("Pad thai", DINNER, "thai");
	addMeal("Pad see ew", DINNER, "thai");
	addMeal("Tom yum", DINNER, "thai");
	addMeal("Thai panang curry", DINNER, "thai");

	addMeal("Chicken tinga", DINNER, "mexican");
	addMeal("Empanadas", DINNER, "mexican");
	addMeal("Chili con carne", DINNER, "mexican");
	addMeal("Tacos", DINNER, "mexican");
	addMeal("Enchiladas", DINNER, "mexican");

	addMeal("Pelmeni", DINNER, "russian");
	addMeal("Chicken kiev", DINNER, "russian");
	addMeal("Beef stroganoff", DINNER, "russian");
	addMeal("Shashlyk", DINNER, "russian");
	addMeal("Draniki", DINNER, "russian");

	addMeal("Blini", DINNER, "russian");
	addMeal("Kulich", DINNER, "russian");
	addMeal("Buckwheat porridge", DINNER, "russian");
	addMeal("Syrniki", DINNER, "russian");
	addMeal("Ponchiki", DINNER, "russian");

	addMeal("Peking duck", DINNER, "chinese");
	addMeal("Xiaolong Bao", DINNER, "chinese");
	addMeal("Mapo tofu", DINNER, "chinese");
	addMeal("Dim sum", DINNER, "chinese");
	addMeal("Biang Biang Noodles", DINNER, "chinese");
	addMeal("Hot and sour soup", DINNER, "chinese");

	addMeal("Cheeseburger and fries", DINNER, "american");
	addMeal("Mac and cheese", DINNER, "american");
	addMeal("Meatloaf", DINNER, "american");
	addMeal("New england clam chowder", DINNER, "american");
	addMeal("Manhattan clam chowder", DINNER, "american");
	addMeal("Chili and cornbread", DINNER, "american");
	addMeal("Sloppy joes", DINNER, "american");

	addMeal("Arroz de pato", DINNER, "portugese");
	addMeal("Cataplana", DINNER, "portugese");
	addMeal("Alcatra", DINNER, "portugese");
	addMeal("Caldo Verde", DINNER, "portugese");
	addMeal("Portuguese Tomato Rice", DINNER, "portugese");

	addMeal("Ramen", DINNER, "japanese");
	addMeal("Udon", DINNER, "japanese");
	addMeal("Yakisoba", DINNER, "japanese");
	addMeal("gyudon (beef bowl)", DINNER, "japanese");
	addMeal("Sushi", DINNER, "japanese");

	numMeals = meals.size(); // Init number of Meals. 
};

MealGenerator::~MealGenerator() {};

bool MealGenerator::addMeal(string name, string time, string cuisine) {
	// TODO Input validation.
	// Check if name of meal is not in meals already. 
	for (int i = 0; i < numMeals; i++) {
		if (name == meals.at(i).getName()) {
			return false;
		}
	}

	// Check if cuisine listed in CUISINES. If not, add to cuisines. 
	for (int i = 0; i < CUISINES.size(); i++) {
		if (cuisine == CUISINES.at(i)) {
			break;
		}
		else
			CUISINES.push_back(cuisine);
	}
	Meal dish(name, time, cuisine);
	meals.push_back(dish);

	return true;
}

string MealGenerator::getRandomMeal() {
	// Generate random Meal using current size of Meal vector. 
	int i = randomize(numMeals);
	return meals.at(i).getName();
}

string MealGenerator::getRandomMealByTime(string time) {
	// Holds current list of Meals matching input time field. 
	vector<Meal> mealsByTime;

	// Iterate over all Meals in collection, push matches to mealsByTime. 
	for (int i = 0; i < numMeals; i++) {
		if (time == meals.at(i).getTime()) {
			mealsByTime.push_back(meals.at(i));
		}
	}
	// No Meals of that time of day stored yet.
	if (numMeals == 0) {
		return "";
	}
	// 1 or more Meal of input time of day exists. 
	else {
		int i = randomize(numMeals); return mealsByTime.at(i).getName();
	}
}

string MealGenerator::getRandomMealByCuisine(string cuisine) {
	// Holds current list of Meals with matching input cuisine field.
	vector<Meal> mealsByCuisine;  

	// Iterate over all Meals in collection, push matches to mealsByCuisine. 
    for (int i = 0; i < numMeals; i++) {
        if (cuisine == meals.at(i).getCuisine()) {
            mealsByCuisine.push_back(meals.at(i));
        }
    }

	// No Meals of that cuisine type stored yet.
    if (mealsByCuisine.size() == 0) {
        return "";   
    }
	// 1 or more Meal of input cuisine exists. 
    else { 
        int i = randomize(mealsByCuisine.size()); 
		return mealsByCuisine.at(i).getName();
    }
}

vector<string> const MealGenerator::getTimesOfDay()
{ return this->TIMES;}

vector<string> const MealGenerator::getCuisines()
{ return this->CUISINES;}


int MealGenerator::randomize(int options) {
	if (!isSeeded) {
		srand(time(NULL));
		isSeeded = true;
	}
	return (rand() % options);
}







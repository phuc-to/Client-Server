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

int MealGenerator::randomize(int options) {
	if (!isSeeded) {
		srand(time(NULL));
		isSeeded = true;
	}
	return (rand() % options);
}

MealGenerator::MealGenerator() {
    // Consider how to handle meals that can classified as 2+ different times -
    // For example, can eat pizza for lunch or dinner.
    // Consider variants like different types of pho or tacos  (could handle as different meals)

     // breakfast
    addMeal("Scrambled eggs", "breakfast", "american");
    addMeal("Avocado toast", "breakfast", "american");
    addMeal("Breakfast burritos", "breakfast", "american");
    addMeal("Sausage egg and cheese mcgriddle", "breakfast", "american");
    addMeal("Buttermilk pancakes", "breakfast", "american");
    addMeal("Chicken and waffles", "breakfast", "american");
    addMeal("Eggs benedict", "breakfast", "american");

    addMeal("Huevos Rancheros", "breakfast", "mexican");
    addMeal("Papas con Chorizo", "breakfast", "mexican");
    addMeal("Chilaquiles", "breakfast", "mexican");
    addMeal("Migas", "breakfast", "mexican");
    addMeal("Churros waffles", "breakfast", "mexican");

    addMeal("Pastel del nata", "breakfast", "portuguese");
    addMeal("portuguese baked eggs", "breakfast", "portuguese");
    addMeal("Broa de milho", "breakfast", "portuguese");
    addMeal("Bolo do Caco", "breakfast", "portuguese");

    addMeal("Persarattu Upma", "breakfast", "indian");
    addMeal("Khura", "breakfast", "indian");
    addMeal("Litti Chokha", "breakfast", "indian");
    addMeal("Muthiya", "breakfast", "indian");
    addMeal("Dhokla", "breakfast", "indian");
    addMeal("Chakuli Pitha", "breakfast", "indian");

    addMeal("Tamagoyaki (rolled omelette", "breakfast", "japanese");
    addMeal("Omurice (omelette rice)", "breakfast", "japanese");
    addMeal("Dashi eggs with rice", "breakfast", "japanese");

    addMeal("Xoi", "breakfast", "vietnamese");
    addMeal("Banh cuon", "breakfast", "vietnamese");
    addMeal("Chao Long (rice porrage)", "breakfast", "vietnamese");

    addMeal("Blini", "dinner", "russian");
    addMeal("Kulich", "dinner", "russian");
    addMeal("Buckwheat porridge", "dinner", "russian");
    addMeal("Syrniki", "dinner", "russian");
    addMeal("Ponchiki", "dinner", "russian");

    // lunch
    addMeal("Onigiri", "lunch", "japanese");
    addMeal("Okanomiyaki", "lunch", "japanese");
    addMeal("Takoyaki", "lunch", "japanese");

    addMeal("Soup, salad, and breadsticks", "lunch", "italian");
    addMeal("Pizza", "lunch", "italian");
    addMeal("Bruschetta", "lunch", "italian");

    addMeal("Grilled cheese and tomato soup", "lunch", "american");
    addMeal("Philly cheesesteak", "lunch", "american");
    addMeal("BLT sandwich", "lunch", "american");
    addMeal("Cobb salad", "lunch", "american");
    addMeal("Tater tots", "lunch", "american");

    addMeal("Banh mi", "lunch", "vietnamese");
    addMeal("Banh xeo", "lunch", "vietnamese");
    addMeal("Bot chien", "lunch", "vietnamese");
    addMeal("Bun cha gio", "lunch", "vietnamese");
    addMeal("Goi cuon", "lunch", "vietnamese");

    // dinner
    addMeal("Pho", "dinner", "vietnamese");
    addMeal("Cha ca", "dinner", "vietnamese");
    addMeal("Cao lau", "dinner", "vietnamese");
    addMeal("com hen", "dinner", "vietnamese");
    addMeal("Bun bo hue", "dinner", "vietnamese");

    addMeal("Pad thai", "dinner", "thai");
    addMeal("Pad see ew", "dinner", "thai");
    addMeal("Tom yum", "dinner", "thai");
    addMeal("Thai panang curry", "dinner", "thai");

    addMeal("Butter chicken", "lunch", "indian");
    addMeal("Dal makhani", "lunch", "indian");
    addMeal("Saag paneer", "lunch", "indian");
    addMeal("Chana masala", "lunch", "indian");

    addMeal("Chicken tinga", "dinner", "mexican");
    addMeal("Empanadas", "dinner", "mexican");
    addMeal("Chili con carne", "dinner", "mexican");
    addMeal("Tacos", "dinner", "mexican");
    addMeal("Enchiladas", "dinner", "mexican");

    addMeal("Pelmeni", "dinner", "russian");
    addMeal("Chicken kiev", "dinner", "russian");
    addMeal("Beef stroganoff", "dinner", "russian");
    addMeal("Shashlyk", "dinner", "russian");
    addMeal("Draniki", "dinner", "russian");

    addMeal("Peking duck", "dinner", "chinese");
    addMeal("Xiaolong Bao", "dinner", "chinese");
    addMeal("Mapo tofu", "dinner", "chinese");
    addMeal("Dim sum", "dinner", "chinese");
    addMeal("Biang Biang Noodles", "dinner", "chinese");
    addMeal("Hot and sour soup", "dinner", "chinese");

    addMeal("Cheeseburger and fries", "dinner", "american");
    addMeal("Mac and cheese", "dinner", "american");
    addMeal("Meatloaf", "dinner", "american");
    addMeal("New england clam chowder", "dinner", "american");
    addMeal("Manhattan clam chowder", "dinner", "american");
    addMeal("Chili and cornbread", "dinner", "american");
    addMeal("Sloppy joes", "dinner", "american");

    addMeal("Arroz de pato", "dinner", "portugese");
    addMeal("Cataplana", "dinner", "portugese");
    addMeal("Alcatra", "dinner", "portugese");
    addMeal("Caldo Verde", "dinner", "portugese");
    addMeal("Portuguese Tomato Rice", "dinner", "portugese");

    addMeal("Ramen", "dinner", "japanese");
    addMeal("Udon", "dinner", "japanese");
    addMeal("Yakisoba", "dinner", "japanese");
    addMeal("gyudon (beef bowl)", "dinner", "japanese");
    addMeal("Sushi", "dinner", "japanese");
};

MealGenerator::~MealGenerator() {};

void MealGenerator::addMeal(string name, string time, string cuisine) {
	// Add code to check for a repeat meal
	
	// New keyword used - do we need to do anything to deallocate memory?
	// Is the new keyword actually necessary?
	Meal dish(name, time, cuisine);
	meals.push_back(dish);
}

string MealGenerator::getRandomMeal() {
	int i = randomize(meals.size());
	return meals.at(i).getName();
}

string MealGenerator::getRandomMealByCuisine(string cuisine) {
    vector<Meal> mealsByCuisine;
    for (int i = 0; i < meals.size(); i++) {
        if (cuisine == meals.at(i).getCuisine()) {
            mealsByCuisine.push_back(meals.at(i));
        }
    }
    int options = mealsByCuisine.size();
    if (options == 0) {
        return "";
    }
    else {
        int i = randomize(options); return mealsByCuisine.at(i).getName();
    }
}

string MealGenerator::getRandomMealByTime(string time) {
    vector<Meal> mealsByTime;
    for (int i = 0; i < meals.size(); i++) {
        if (time == meals.at(i).getTime()) {
            mealsByTime.push_back(meals.at(i));
        }
    }
    int options = mealsByTime.size();
    if (options == 0) {
        return "";
    }
    else {
        int i = randomize(options); return mealsByTime.at(i).getName();
    }
}



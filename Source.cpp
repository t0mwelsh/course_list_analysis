// PHYS 30762 Programming in C++
// Assignment 2

// Program to compute mean, standard deviation and standardberror of a set of courses. Data is read from a file.
// This program is set such that the file has rows of data entries of the form: grade course_code  course_name, 
// eg. 65.6 10071   Mathematics 1

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <functional>

// Functions to compute mean and standard deviation

double mean(std::vector<double> vector)
// returns mean of the values in a 1D vector
{
	if (vector.empty()) {
		return 0;
	}
	return std::accumulate(vector.begin(), vector.end(), 0.0) / vector.size();
}

double sd(std::vector<double> vector, double mean_value)
// returns standard deviation of the values in a 1D vector with a pre-calculated mean
{
	if (vector.empty()) {
		return 0;
	}
	double square_sum = std::inner_product(vector.begin(), vector.end(), vector.begin(), 0.0);
	return sqrt(square_sum / vector.size() - mean_value * mean_value);
}

std::vector<int> year_selector()
// returns a vector with elements corresponding to zero if that year is not selected and 
// the year if a year should be selected in ascending order, eg. [1, 0, 3, 0] corresponds to years 1 and 3 being selected
{
	bool select_years{ true };
	int first_year{ 0 };
	int second_year{ 0 };
	int third_year{ 0 };
	int fourth_year{ 0 };
	bool year_been_selected{ false };
	std::string input_string;

	std::cout << "Please select years to calculate with. Type 1, 2, 3 or 4 to select a respective year;"
		<< std::endl << "this will repeat until you type 'n'." << std::endl;

	while (select_years)
	{
		getline(std::cin, input_string);

		if (input_string == "n" && !year_been_selected) {
			std::cout << "Please choose a year first" << std::endl;
			continue;
		}

		else if (input_string == "1") { first_year = 1; year_been_selected = true; }
		else if (input_string == "2") { second_year = 2; year_been_selected = true; }
		else if (input_string == "3") { third_year = 3;; year_been_selected = true; }
		else if (input_string == "4") { fourth_year = 4;; year_been_selected = true; }

		else if (input_string == "n" && year_been_selected) {
			select_years = false;
		}
		else {
			std::cout << "Please enter 1, 2, 3, 4, or n" << std::endl;
		}
	}

	std::vector<int> year_vector(4);
	year_vector[0] = first_year; year_vector[1] = second_year;
	year_vector[2] = third_year; year_vector[3] = fourth_year;

	return year_vector;
}

bool first_digit_in(int a, std::vector<int> vector)
//returns a boolean to say if the first digit of an integer is in a vector of integers
{
	while (a >= 10) { a /= 10; } //gets first digit
	std::vector<int>::iterator iter;

	iter = std::find(vector.begin(), vector.end(), a);
	if (iter != vector.end())
		return true;
	else
		return false;
}

void specific_alphabetical_sort(std::vector<std::string>& s, int i)
// sorts a vector of strings alphabetically from a specific index of a string, eg. 3rd letter.
// the user is first asked if they want to sort
{
	bool incorrect_input{ true };
	std::string input_string;
	bool sort;

	std::cout << "Would you like to sort alphabetically? Type y for yes and n for no" << std::endl;

	while (incorrect_input)
	{
		getline(std::cin, input_string);
		if (input_string == "n") { sort = false; incorrect_input = false; }
		else if (input_string == "y") { sort = true; incorrect_input = false;}
		else { std::cout << "Please enter y or n" << std::endl; }
	}

	if (sort) {
		std::sort(s.begin(), s.end(), [i](std::string a, std::string b)
			{return a.substr(i, a.size() - 1) < b.substr(i, b.size() - 1);});
	}
}

// Main function

int main()
{
	// Ask user to enter filename
	std::string filename;
	std::ifstream input_file;

	std::cout << "Enter data filename: ";
	std::cin >> filename;

	// Open file and check if successful
	input_file.open(filename); //filename should be courselist.dat

	if (input_file.fail())
	{
		std::cout << "File failed to open." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "File opened successfully" << std::endl;
	}

	std::vector<std::string> course_names; std::string course_name;
	std::vector<double> grades; double grade;
	std::vector<int> course_codes; int course_code;
	int number_courses{ 0 };

	// Creates vectors of the 3 columns of data in the file with appropriate types and finds number of records in the file
	while (input_file.peek() != EOF)
	{
		number_courses++;
		input_file >> grade;
		grades.push_back(grade);
		input_file >> course_code;
		course_codes.push_back(course_code);
		getline(input_file, course_name); //whitespace in name means can't use previous tactic
		course_name = course_name.substr(2, course_name.size() - 1); // removes whitespace at start
		course_names.push_back(course_name);
	}

	// Close file
	input_file.close();

	// Select which years to calculate on
	std::vector<int> year_vector{ year_selector() };

	std::vector<int> chosen_indicies; // vector of all the indicies we want to work over due to selected years

	// use find_if function to return the indicies of the coursecodes whose first digit is in the year_vector. lambda function used as find_if
	// requires a unary function and the lambda allows us to set the second argument of first_digit_in function to year_vector but still iterate
	// over the course codes
	std::vector<int>::iterator iter = course_codes.begin();
	while ((iter = std::find_if(iter, course_codes.end(), [year_vector](int x) {return first_digit_in(x, year_vector); })) != course_codes.end())
	{
		chosen_indicies.push_back(std::distance(course_codes.begin(), iter));
		iter++;
	}

	// use string stream to combine the integer course code and title and select grades over chosen_indicies
	std::vector<std::string>string_stream_to_print;
	std::vector<double>selected_grades;
	for (int i : chosen_indicies)
	{
		std::ostringstream temp;
		temp << "PHYS " << course_codes[i] << course_names[i];
		string_stream_to_print.push_back(temp.str());
		selected_grades.push_back(grades[i]);
	}
	
	//asks user to sort the courses alphabetically or not, i.e coursecodes
	specific_alphabetical_sort(string_stream_to_print, 11);

	double mean_value{ mean(selected_grades) };
	double standard_deviation{ sd(selected_grades, mean_value) };
	double mean_error{ standard_deviation / sqrt(selected_grades.size()) };

	// use iterator to print out info of each course
	std::cout << "Courses selected:" << std::endl;
	for (std::vector<std::string>::const_iterator i = string_stream_to_print.begin(); i != string_stream_to_print.end(); ++i)
		std::cout << *i << std::endl;
	
	// print out mean, mean error and sd for selected courses with set precision
	std::cout << std::fixed << std::setprecision(1) << "The mean of the grades selected is " << mean_value << " with error " 
		<< mean_error << std::endl;
	std::cout << std::fixed << std::setprecision(2) << "The standard deviation is " << standard_deviation << std::endl;

	return 0;
}
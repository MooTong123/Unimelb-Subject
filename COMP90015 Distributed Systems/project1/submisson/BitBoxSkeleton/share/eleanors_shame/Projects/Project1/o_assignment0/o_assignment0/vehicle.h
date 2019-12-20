#pragma once
#include <string>

class vehicle {
	public:
		vehicle(void);
		vehicle(std::string _make, std::string _model, std::string _colour, int _year, int _mileage);

		std::string get_make(void);
		std::string get_model(void);
		std::string get_colour(void);
		int get_year(void);
		int get_mileage(void);

		void set_make(std::string _make);
		void set_model(std::string _model);
		void set_colour(std::string _colour);
		void set_year(int _year);
		void set_mileage(int _mileage);

		virtual void details(void) = 0;
	private:
		std::string make, model, colour;
		int year, mileage;
};

class car: public vehicle {
	public:
		car(void);
		car(std::string _make, std::string _model, std::string _colour, int _year, int _mileage);

		void details(void);
};

class truck: public vehicle {
	public:
		truck(void
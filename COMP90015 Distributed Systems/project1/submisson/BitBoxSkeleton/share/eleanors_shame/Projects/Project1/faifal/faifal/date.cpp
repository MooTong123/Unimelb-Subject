#include "date.h"
#include <string>
using namespace std;
void Date::setDate(string xmonth, int xday, int xyear)
{
 this->month = xmonth;
 this->day = xday;
 this->year = xyear;
}
Date::Date(string month, int day, int year)
{
  setDate(month, day, year);
}
Date::Date()
{
  setDate("January", 1, 1900);
}
string Date::getMonth() { return month; }
int Date::getDay() { return day; }
int Date::getYear() { return year; }
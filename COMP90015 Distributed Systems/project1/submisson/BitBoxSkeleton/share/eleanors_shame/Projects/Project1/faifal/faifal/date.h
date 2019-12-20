#ifndef DATE_H
#define DATE_H
#include <string>
using namespace std;

//Date class
class Date
{
private:
   string month;
   int day;
   int year;
public:
   void setDate(string xdate, int xday, int xyear);
   Date(string month, int day, int year);
   Date(void);
   string getMonth(void);
   int getDay(void);
   int getYear(void);

};


#endif

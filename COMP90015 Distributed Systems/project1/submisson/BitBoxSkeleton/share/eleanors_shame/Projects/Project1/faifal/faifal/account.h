#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Customer.h"

//Account Class
class Account {
private:
        string type;
        int number;
    double balance;
    double rate;
        double interest;
        double NewBalance;

public:
        Account(); // Constructor
        Account(string, int, double, double) ;
                void setType(string);
                void setNumber(int);
                void setBalance(double);
                void setRate(double);

                string getType();
                int getNumber();
                double getBalance();
                double getRate();
                void valid_rate(double);
                void ApplyInterest();
                                void operator += (Account& a);
                                void operator -= (Account& a);
                                void operator ++ (int n );

                };      
#endif
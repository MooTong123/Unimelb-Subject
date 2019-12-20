#include "account.h"
#include <string>
#include <iostream>
using namespace std;
Account::Account() {
                type = " ";
                number = 0;
                balance = 0;
                rate = 0;
        }

        Account::Account(string type, int number, double balance, double rate) {
                Account::type = type;
                Account::number = number;
                Account::balance = balance;
                valid_rate(rate);
                }
void Account::setType(string type) {
    Account::type = type;
}

void Account::setNumber(int number) {
    Account::number = number;
}

void Account::setBalance(double balance) {
    Account::balance = balance;
}

void Account::setRate(double rate) {
        valid_rate(rate);
}

string Account::getType() {
    return type;
}
int Account::getNumber() {
    return number;
}
double Account::getBalance() {
    return balance;
}
double Account::getRate() {
    return rate;
}

void Account::valid_rate(double rate) {
        if (rate >=.01 && rate <= .1)
                this->rate=rate;
        else {
                this->rate=0;
                cout << "WARNING! You have entered an invalid rate!\n";
        }
}

void Account::ApplyInterest() {
                interest = rate * balance;
                NewBalance = interest + balance;
                cout << "The amount of interest for the Customer is: $" << interest << " and the account balance is: $" << NewBalance << ".\n";
}      


   // void operator += (Account& a) {
    void Account::operator+= (Account& a) {
        setBalance(getBalance()+ a.getBalance());
    }

    //void operator -= (Account& a) {
        void Account::operator-= (Account& a) {
        setBalance(getBalance() - a.getBalance());
    }

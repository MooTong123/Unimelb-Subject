#include "customer.h"
#include <string>

Customer::Customer() {
    name = " ";
    ssn = " ";
    address = " ";
    birthDate = 0;
    savings = 0;
        checking = 0;
        balance = 0;
}

Customer::Customer(string name, string ssn, string address, int birthDate, int savings, int checking, int balance) {
    Customer::name =  name;
    Customer::ssn = ssn;
    Customer::address = address;
    Customer::birthDate = birthDate;
    Customer::savings = savings;
    Customer::checking = checking;
        Customer::balance = balance;
}

void Customer::setName(string name) {
    Customer::name = name;
}

void Customer::setSSN(string ssn) {
    Customer::ssn = ssn;
}

void Customer::setAddress(string address) {
    Customer::address = address;
}

void Customer::setBirthDate(int birthDate) {
    Customer::birthDate = birthDate;
}

void Customer::setSavings(double savings) {
    Customer::savings = savings;
}

void Customer::setChecking(double checking) {
    Customer::checking = checking;
}

void Customer::setBalance(double balance) {
        Customer::balance = balance;
}



string Customer::getName() {
    return name;
}

string Customer::getSSN() {
    return ssn;
}

string Customer::getAddress() {
    return address;
}

int Customer::getBirthDate() {
    return birthDate;
}

double Customer::getSavings() {
    return savings;
}

double Customer::getChecking() {
    return checking;
}

double Customer::getBalance() {
        return balance;

}

void Customer::displayCustomer() {
    cout << "The current customer is " << name << ", their address is " << address << ", and their Social Security Number is "
                << ssn << ".  "  << name << "'s Saving Account Ballance is:" << savings << ".  The Checking Account Balance is:" << checking << ".\n";
        cout << "The consolidated balance is: $" << balance << ".\n";
}

string Customer::string_displayCustomer() {
        stringstream buf;
        cout << "The current customer is " << name << ", their address is " << address << ", and their Social Security Number is "
        << ssn << ".  "  << name << "'s Saving Account Ballance is:" << savings << ".  The Checking Account Balance is:" << checking << ".\n";
        cout << "The consolidated balance is: $" << balance << ".\n";
    return buf.str();
}
void Customer::operator += ( double dValue ) {
        setBalance( getBalance()+ dValue );
                //return this;
    }
void Customer::operator -= ( double dValue ) {
        setBalance( getBalance() - dValue );
                //return this;
    }

void Customer::operator ++ () {
       
      setBalance(getSavings()  +  getChecking());
 }

#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

int rollDice();
// Rolls a d6 and outputs a random number that could be more random
// according to everything I read online but screw it I still got homework to do
void printCootie();
//this gives us all our cootie data printed out to the screen after rolls
int rollIO(int& rollResult, int& diceCount, char charname);
//asks for input, updates the result tally, and spits out the result of the function rollDice
bool applyRoll(int rollResult, bool& body, bool& head, bool& top, bool& eyes, bool& mouth, int& legs);
//take the results of the roll and see what cootie parts you get out of it, if any
void checkBody(int rollResult);
//Body good? Yes? addBody No? Try again.
void addBody(bool& body);
//body == true
void checkHead(int rollResult);
//second verse, same as the first (check.) Only it checks the head.
void addHead(bool& head);
//head == true
void checkRest(int rollResult, bool& top, bool& eyes, bool& mouth, int& legs);
// Like lever 2000; for all your other (cootie) parts.
void topCheck(bool& top);
// Want a hat? Get a hat. Got a hat? Need a hat. 
void eyeCheck(bool& eyes);
//  cootie eyes y/n
void mouthCheck(bool& mouth);
// mouth check
void legCheck(int& legs);
//Count to six!
bool cootieComplete(bool& top, bool& eyes, bool& mouth, int& legs, int diceCount, char& doagain);
// Please god let this be done already.



int main(int argc, char *argv[])
{
char doagain = 0; 
    do
    {

    srand(static_cast<unsigned int>(time(0)));

    bool head = false;
    bool body = false;
    bool top = false;
    bool eyes = false;
    bool mouth = false;
    int legs = 0;
    int rollResult;
    int diceCount=0;
    char charname;


       cout << "Let's play Cootie! Enter any character to roll the dice." << endl;
       rollIO(rollResult, diceCount, charname);
       applyRoll(rollResult, body, head, top, eyes, mouth, legs);
       printCootie();
       cootieComplete(top, eyes, mouth, legs, diceCount, doagain);

  }while (doagain == 'Y' || doagain == 'y');
    system ("pause");
    return 0;
}

int rollDice() {


   return (rand()% 6) + 1;

}

void printCootie(bool head, bool body, bool top, bool mouth, bool eyes, int legs) {
    cout << "Cootie so far " << body << " body, "  << head << " head, "
        << top << " antennae, " <<  mouth << " mouth, "
        << eyes << " eyes and " << legs << " legs." << endl;
}

int rollIO(int& rollResult, int& diceCount, char charname){

    cout << "Roll the dice: " << endl;
    cin >> charname;
    diceCount++;
    rollResult = rollDice();
    cout << "You rolled a " << rollResult << endl;
	return rollResult;
}


bool applyRoll(int rollResult, bool& body, bool& head, bool& top, bool& eyes, bool& mouth, int& legs){
       while(body == false){
        rollIO(rollResult);
        checkBody(rollResult);
       }
       while (head==false){
        rollIO(rollResult);
        checkHead(rollResult);
       }
       while(top == false || eyes == false || mouth == false || legs != 6){
        rollIO(rollResult);
        checkRest(rollResult, top, eyes, mouth, legs);
       }
printCootie();
return 0;
}

void checkBody(int rollResult, bool& body){
    switch(rollResult){
        case 1: addBody();
        break;
        default: cout << "You need the body first!";
        break;
    }
}

void addBody(bool& body) {
    cout << "You got the body!" << endl;
    body = true;
    }

void checkHead(int diceRoll, bool& head){
    switch(diceRoll){
        case 1: addHead();
        break;
        default: cout << "You need the head first!";
        break;
    }
}

void addHead(bool& head) {
    cout << "You got the head!" << endl;
    head = true;
}

void checkRest(int rollResult, bool& top, bool& eyes, bool& mouth, int& legs){
    while(top == false || eyes == false || mouth == false || legs != 6){
       switch(rollResult){
          case 3:
              topCheck(top);
              break;
          case 4:
              eyeCheck(eyes);
              break;
          case 5:
              mouthCheck(mouth);
              break;
          case 6:
              legCheck(legs);
              break;
          default:
              cout << "how the hell did you trigger this something is seriously wrong" << endl;
              break;
        }
    }
}

void topCheck(bool& top){
    if(top == true){
        cout << "You already got the antennae!" << endl;
        }
    else{
        cout << "You got the antennae!" << endl;
        top = true;
        }
}


void eyeCheck(bool& eyes){
    if(eyes == true){
        cout << "You already got the eyes!" << endl;
        }
    else{
        cout << "You got the eyes!" << endl;
        eyes = true;
        }
}

void mouthCheck(bool& mouth){
    if(mouth == true){
        cout << "You already got the mouth!" << endl;
    }
    else{
        cout << "You got the mouth!" << endl;
        mouth = true;
    }
}

void legCheck(int& legs){
    if(legs == 6){
        cout << "You already got all the legs!" << endl;
    }
    else{
        cout << "You got a leg!" << endl;
        legs++;
    }
}

bool cootieComplete(bool& top, bool& eyes, bool& mouth, int& legs, int diceCount, char& doagain){
       if(top == true && eyes == true && mouth == true && legs == 6){
           cout << "You win!!" << "It took you " << diceCount << " attempts to win." << endl;
           cout << "Try again? (Y/N)" << endl;
           cin >> doagain;
           return doagain;
        }
}


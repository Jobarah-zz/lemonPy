#include <string>

using namespace std;

class Token {
    public:
        Token(string *str, int num){
            this->name = str;
            this->value = num;
        }
        string *name;
        int value;
};
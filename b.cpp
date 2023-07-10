#include <iostream>
#include <cstring>

void change(int*& a){
    int b = 20;
    a = &b;
}

struct lap{
    char* lol;
    bool co;
};

int main(){
    lap lap;
    lap.lol = (char*)malloc(5);
    strcpy(lap.lol, "cain");

    free(lap.lol);

    std::cout << lap .co << '\n';
}
#include "./include/Application.hpp"

int main(){
    FontManager& fontManager = FontManager::getInstance();

    Application app(fontManager);
    app.run();

    return 0;
}
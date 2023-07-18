#include "./include/WorldCoords.hpp"
#include "./include/TextBox.hpp"
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <vector>
#include <memory>

void renderBuckets(std::unique_ptr<HashTable*>* ht){
    return;
}

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "HashTable Illustration");

    std::unique_ptr<HashTable*> hash_t = std::make_unique<HashTable*>(ht_create(13, fnv_hash_func, LINEAR_PROBING));
    std::vector<TextBox> boxes((*hash_t)->capacity , TextBox());
    renderBuckets(&hash_t);

    sf::Font mJosefinSans;
    if(!mJosefinSans.loadFromFile("./Fonts/JosefinSans-Bold.ttf")){
        std::cout << "Couldn't load font from disk\n";
        return -1;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        // 5 per column
        for(size_t i = 0; i < 5; ++i){
            boxes[i].generateTextBox({100, 80}, {50 + static_cast<float>(i) * 150.75f, 50}, color::Black, color::White, 2);
            boxes[i].writeText("Cali3ssn" + std::to_string(i), &mJosefinSans);    
        }

        for(auto& box: boxes)
            box.renderToScreen(window);

        window.display();
    }

    return 0;
}
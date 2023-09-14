#include "./include/WorldCoords.hpp"
#include "./include/TextBox.hpp"
#include "./include/Button.hpp"
#include "./include/CustomColors.hpp"
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <vector>
#include <memory>

void renderBuckets(std::vector<TextBox>& boxes, std::unique_ptr<HashTable*>& ht, const sf::Font& font){
    if(boxes.size() != (*ht)->capacity)
        boxes.resize((*ht)->capacity);

    int cols = 5;
    int rows = (boxes.size() + cols - 1) / cols;

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            size_t idx = j + i * cols;
            if(idx < boxes.size()){
                boxes[idx].generateTextBox(
                    {130, 95}, 
                    // Position Settings
                    {50 + static_cast<float>(j) * 150.75f, 120 + static_cast<float>(i) * 145.75f}, 
                    color::Black, color::White, 2
                    );
                
                if((*ht)->buckets[idx] != NULL){
                    boxes[idx].setOutlineColor(color::Green);
                    boxes[idx].writeHashTableText(
                        "K: " + std::string(static_cast<char*>((*ht)->buckets[idx]->key)), 
                        "V: " + std::string(static_cast<char*>((*ht)->buckets[idx]->val)), idx, 
                        &font, color::Yellow, CUSTOM_CYAN, CUSTOM_PURPLE   
                    );
                }else{
                    boxes[idx].setOutlineColor(color::Red);
                    boxes[idx].writeHashTableText("", "NULL", idx, &font);
                }
            }
        }
    }
}

void renderBucketsToScreen(std::vector<TextBox>& boxes, sf::RenderWindow& window){
    for(auto& box: boxes){
        box.renderToScreen(window);
    }
}

void insertQueries(std::unique_ptr<HashTable*>& hash_t){
    ht_insert(*hash_t, "salut:D", "Regele", 7);
    ht_insert(*hash_t, "CalinRege", "Apricot", 7);
    ht_insert(*hash_t, "Rares Boss", "rege", 5);
    ht_insert(*hash_t, "Dejan", "sall", 5);
    ht_insert(*hash_t, "Aries", "Glitter", 8);
    ht_insert(*hash_t, "AriesWow", "Glitter", 8);
    ht_insert(*hash_t, "Amy's Grave", "Glitter", 8);
    ht_insert(*hash_t, "90Days", "Glitter", 8);
}

int main(){
    sf::RenderWindow window(sf::VideoMode(820, 600), "HashTable Illustration");
    sf::View view(window.getDefaultView());

    std::unique_ptr<HashTable*> hash_t = std::make_unique<HashTable*>(ht_create(33, fnv_hash_func, LINEAR_PROBING));
    std::vector<TextBox> boxes((*hash_t)->capacity , TextBox());
    insertQueries(hash_t);

    sf::Font mJosefinSans;
    if(!mJosefinSans.loadFromFile("./Fonts/JosefinSans-Bold.ttf")){
        std::cout << "Couldn't load font from disk\n";
        return -1;
    }

    renderBuckets(boxes, hash_t, mJosefinSans);
    Button insertButton({100, 50}, {50, 30}, mJosefinSans);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseWheelScrolled){
                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel){
                    view.move(0, -event.mouseWheelScroll.delta * 60.0f);
                    window.setView(view);
                }
            }
        }

        window.clear();
        
        renderBucketsToScreen(boxes, window);
        insertButton.renderButtonToScreen(window);

        window.display();
    }

    return 0;
}
#include "./include/TextBox.hpp"
#include "./include/Button.hpp"
#include "./include/DynamicTextBox.hpp"
#include "./include/Dropdown.hpp"
#include "./include/CustomColors.hpp"
#include "./include/FontManager.hpp"
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <vector>
#include <memory>
#include <iostream>

#define INSERTION 0x01
#define DELETION 0x02

// TODO: If there is chaining, the bucket's value will be overwritten, Solution: Adauga un I(info) care cand hovered sa afiseze o lista cu nodurile chained

void createBuckets(std::vector<TextBox>& buckets, std::unique_ptr<HashTable*>& ht, const font& font){
    if(buckets.size() != (*ht)->capacity)
        buckets.resize((*ht)->capacity);

    int cols = 5;
    int rows = (buckets.size() + cols - 1) / cols;

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            size_t idx = j + i * cols;
            if(idx < buckets.size()){
                buckets[idx].generateTextBox(
                    {130, 95}, 
                    // Position Settings, 200 is the distance in pixels from the top of the screen until the first row
                    {50 + static_cast<float>(j) * 150.75f, 200 + static_cast<float>(i) * 145.75f}, 
                    NULL_BOX_FILL, NULL_BOX_TEXT_COLOR, 2 
                    );
                
                // *Tombstone check for OPEN-ADDRESSING Collision Resolutions
                if((*ht)->buckets[idx] != NULL && !(*ht)->buckets[idx]->is_tombstone){
                    buckets[idx].setOutlineColor(color::Green);
                    buckets[idx].writeHashTableText(
                        "K: " + std::string(static_cast<char*>((*ht)->buckets[idx]->key)), 
                        "V: " + std::string(static_cast<char*>((*ht)->buckets[idx]->val)), idx, 
                        font, color::Yellow, CUSTOM_CYAN, CUSTOM_PURPLE   
                    );
                }else{
                    buckets[idx].setOutlineColor(NULL_BOX_OUTLINE);
                    buckets[idx].writeHashTableText("", "NULL", idx, font);
                }
            }
        }
    }
}

// TODO: Implement, only update a bucket, do not recreate the whole vector of buckets
inline void updateBucket(std::vector<TextBox>& buckets, const size_t idx, const font& font, uint16_t flag){
    if(flag & INSERTION){
        // TODO
        return;
    }else if(flag & DELETION){
        buckets[idx].setOutlineColor(NULL_BOX_OUTLINE);
        buckets[idx].writeHashTableText("", "NULL", idx, font);
    }
}

inline void renderBucketsToScreen(std::vector<TextBox>& buckets, renderWin& window){
    for(auto& box: buckets){
        box.renderToScreen(window);
    }
}

template <typename... Buttons>
inline void renderActionButtonsToScreen(renderWin& window, const Buttons&... buttons){
    (buttons.renderToScreen(window), ...);
}

inline void insertQueries(HashTable* ht){
    ht_insert(ht, "Eu", "Aries", 6);
    ht_insert(ht, "Tu", "Taurus", 7);
    ht_insert(ht, "El", "Gemini", 8);
    ht_insert(ht, "Ea", "Cancer", 8);
    // ht_insert(ht, "Noi", "Leo", 4);
    // ht_insert(ht, "Voi", "Virgo", 6);
}

int main(){
    // Creating the Window
    renderWin window(sf::VideoMode(820, 900), "HashTable Illustration");
    sfview view(window.getDefaultView());

    // Loading the font from disk
    FontManager& fontManager = FontManager::getInstance();
    fontManager.loadFont("josefin-sans-bold", "./Fonts/JosefinSans-Bold.ttf");
    fontManager.loadFont("roman-bold", "./Fonts/times_new_roman_bold.ttf");
    font& appFont = fontManager.getFont("josefin-sans-bold");

    // Creating the buckets
    std::unique_ptr<HashTable*> hash_t = std::make_unique<HashTable*>(ht_create(10, fnv_hash_func, LINEAR_PROBING));
    std::vector<TextBox> buckets((*hash_t)->capacity , TextBox());
    
    // Define custom UIStyle elements
    UIStyle josefinSansFontStyle{appFont};
    UIStyle actionButtonsStyle{appFont, color::Black, color::White, CUSTOM_PURPLE};

    insertQueries(*hash_t);

    createBuckets(buckets, hash_t, appFont);
    Button insertButton({50, 30}, {80, 40}, actionButtonsStyle, "Insert", 16);
    Button deleteButton({150, 30}, {80, 40}, actionButtonsStyle, "Delete", 16);
    Button clearButton({250, 30}, {80, 40}, actionButtonsStyle, "Clear HT", 16);
    // Dropdown dropdown({350, 30}, {120, 40}, josefinSansFontStyle, {"Wow", "Lawl"});

    std::vector<DynamicTextBox> textBoxes{};
    textBoxes.emplace_back(DynamicTextBox{{50, 130}, {180, 50}, appFont, 28, "./sprites/clear-text.png"});
    textBoxes.emplace_back(DynamicTextBox{{350, 130}, {180, 50}, appFont, 28, "./sprites/clear-text.png"});
    textBoxes[0].setLabel("Key", josefinSansFontStyle, 20.f);
    textBoxes[1].setLabel("Value", josefinSansFontStyle, 20.f);

    while(window.isOpen()){
        event event;
        while (window.pollEvent(event)){
            switch(event.type){
                case event::Closed:
                    free_ht(&(*hash_t));
                    window.close();
                    break;
                case event::MouseWheelScrolled:
                    if(event.mouseWheelScroll.wheel == mouse::VerticalWheel){
                        view.move(0, -event.mouseWheelScroll.delta * 60.0f);
                        window.setView(view);
                    }
                    break;
                case event::TextEntered:
                    // Pass the text input event to the active text box (if any)
                    for(auto& textBox: textBoxes){
                        if(textBox.getIsActive()){
                            textBox.handleEvent(event, window);
                        }
                    }
                    break;
                case event::MouseButtonPressed:
                    // Check if any text box should be activated
                    for(auto& textBox: textBoxes){
                        if (textBox.containsPoint(mouse::getPosition(window).x, mouse::getPosition(window).y)) {
                            textBox.setActive(true);
                        } else {
                            textBox.setActive(false);
                        }
                    }
                    break;
                case event::KeyPressed:
                    for(auto& textBox: textBoxes){
                        if(textBox.getIsActive()){
                            if(event.key.code == sf::Keyboard::Delete){
                                textBox.clearTextBoxText();
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        window.clear();

        // Update and render the text buckets
        for(auto& textBox : textBoxes){
            textBox.update(window);
        }

        // Render HashTable's Buckets to screen
        renderBucketsToScreen(buckets, window);
        renderActionButtonsToScreen<Button>(window, insertButton, deleteButton, clearButton);
        // dropdown.renderToScreen(window);

        if(insertButton.is_clicked(window, CUSTOM_CYAN)){
            appFont.loadFromFile("./Fonts/JosefinSans-Bold.ttf");
            if(!textBoxes[0].isEmpty() && !textBoxes[1].isEmpty()){
                const std::string_view& key = textBoxes[0].getInputAsString();
                const std::string_view& value = textBoxes[1].getInputAsString();
                const size_t valueLen = textBoxes[1].getInputLength();
                ht_insert(*hash_t, key.data(), value.data(), valueLen + 1);
                createBuckets(buckets, hash_t, appFont);
                std::cout << (*hash_t)->hash_func(key.data(), (*hash_t)->capacity) << '\n';
            }
        }

        if(deleteButton.is_clicked(window, CUSTOM_CYAN)){
            appFont = fontManager.getFont("roman-bold"); 
            if(!textBoxes[0].isEmpty()){
                const size_t idx = ht_remove(*hash_t, textBoxes[0].getInputAsCharArr());
                if(idx != SIZE_MAX){
                    updateBucket(buckets, idx, appFont, DELETION);
                }
            }
        }

        if(clearButton.is_clicked(window, CUSTOM_CYAN)){
            ht_clear(*hash_t);
            createBuckets(buckets, hash_t, appFont);
        }

        window.display();
    }


    return 0;
}
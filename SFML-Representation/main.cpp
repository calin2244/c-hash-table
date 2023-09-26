#include "./include/WorldCoords.hpp"
#include "./include/TextBox.hpp"
#include "./include/Button.hpp"
#include "./include/DynamicTextBox.hpp"
#include "./include/Dropdown.hpp"
#include "./include/CustomColors.hpp"
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <vector>
#include <memory>

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
                        &font, color::Yellow, CUSTOM_CYAN, CUSTOM_PURPLE   
                    );
                }else{
                    buckets[idx].setOutlineColor(NULL_BOX_OUTLINE);
                    buckets[idx].writeHashTableText("", "NULL", idx, &font);
                }
            }
        }
    }
}

// TODO: Implement, only update a bucket, do not recreate the whole vector of buckets
void updateBucket(){

}

void renderBucketsToScreen(std::vector<TextBox>& buckets, renderWin& window){
    for(auto& box: buckets){
        box.renderToScreen(window);
    }
}

void insertQueries(HashTable* ht){
    ht_insert(ht, "Eu", "Aries", 5);
    ht_insert(ht, "Tu", "Taurus", 6);
    ht_insert(ht, "El", "Gemini", 7);
    ht_insert(ht, "Ea", "Cancer", 7);
    ht_insert(ht, "Noi", "Leo", 4);
    ht_insert(ht, "Voi", "Virgo", 6);
    ht_insert(ht, "Ei", "Libra", 6);
    ht_insert(ht, "Ele", "Scorpio", 8);
    ht_insert(ht, "Acela", "Capricorn", 10);
    ht_insert(ht, "Ceva", "Aquarius", 8);
    ht_insert(ht, "Altceva", "Pisces", 8);
    ht_insert(ht, "Lorem", "Lorem Ipsum", 11);
    ht_insert(ht, "RKey2", "Value2", 6); // Shorter key
    ht_insert(ht, "Key3", "Value3", 5); // Shorter key
    ht_insert(ht, "Key24", "Value3", 5); // Shorter key
    ht_insert(ht, "Key6", "Value3", 5); // Shorter key
}


int main(){
    renderWin window(sf::VideoMode(820, 900), "HashTable Illustration");
    sfview view(window.getDefaultView());

    std::unique_ptr<HashTable*> hash_t = std::make_unique<HashTable*>(ht_create(10, fnv_hash_func, LINEAR_PROBING));
    std::vector<TextBox> buckets((*hash_t)->capacity , TextBox());

    font fJosefinSans;
    if(!fJosefinSans.loadFromFile("./Fonts/JosefinSans-Bold.ttf")){
        std::cout << "Couldn't load font from disk\n";
        return -1;
    }
    UIStyle josefinSansFontStyle = {
        .font = fJosefinSans
    };

    insertQueries(*hash_t);

    createBuckets(buckets, hash_t, fJosefinSans);
    Button insertButton({50, 30}, {100, 50}, josefinSansFontStyle, "Insert");
    Button deleteButton({200, 30}, {100, 50}, josefinSansFontStyle, "Delete");
    Button clearButton({350, 30}, {100, 50}, josefinSansFontStyle, "Clear HT");
    Dropdown dropdown({50, 300}, {200, 100}, josefinSansFontStyle);

    DynamicTextBox keyTextBox({50, 100}, {180, 50}, fJosefinSans, 28);
    DynamicTextBox valueTextBox({350, 100}, {180, 50}, fJosefinSans, 28);
    std::vector<DynamicTextBox> textBoxes{ std::move(keyTextBox), std::move(valueTextBox) };

    while (window.isOpen()){
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
                    for(auto& textBox : textBoxes){
                        if(textBox.getIsActive()){
                            textBox.handleEvent(event, window);
                        }
                    }
                    break;
                case event::MouseButtonPressed:
                    // Check if any text box should be activated
                    for(auto& textBox : textBoxes){
                        if (textBox.containsPoint(mouse::getPosition(window).x, mouse::getPosition(window).y)) {
                            textBox.setActive(true);
                        } else {
                            textBox.setActive(false);
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

        insertButton.renderToScreen(window);
        deleteButton.renderToScreen(window);
        clearButton.renderToScreen(window);

        if(insertButton.is_clicked(window, CUSTOM_CYAN)){
            const std::string_view& key = textBoxes[0].getInputAsString();
            const std::string_view& value = textBoxes[1].getInputAsString();
            const size_t valueLen = textBoxes[1].getInputLength();
            ht_insert(*hash_t, key.data(), value.data(), valueLen + 1);
            createBuckets(buckets, hash_t, fJosefinSans);
        }

        if(deleteButton.is_clicked(window, CUSTOM_CYAN)){
            if(!std::empty(textBoxes[0].getInputAsString())){
                const size_t idx = (*hash_t)->hash_func(textBoxes[0].getInputAsCharArr(), (*hash_t)->capacity);
                ht_remove(*hash_t, textBoxes[0].getInputAsCharArr());
                buckets[idx].setOutlineColor(NULL_BOX_OUTLINE);
                buckets[idx].writeHashTableText("", "NULL", idx, &fJosefinSans);
                ht_print(*hash_t, print_string_string);
            }
        }

        if(clearButton.is_clicked(window, CUSTOM_CYAN)){
            clear_ht(*hash_t);
            createBuckets(buckets, hash_t, fJosefinSans);
        }

        // dropdown.renderToScreen(window);

        window.display();
    }


    return 0;
}
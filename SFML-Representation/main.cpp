#include "./include/WorldCoords.hpp"
#include "./include/TextBox.hpp"
#include "./include/Button.hpp"
#include "./include/DynamicTextBox.hpp"
#include "./include/CustomColors.hpp"
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <vector>
#include <memory>

void createBuckets(std::vector<TextBox>& boxes, std::unique_ptr<HashTable*>& ht, const font& font){
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
                    // Position Settings, 200 is the distance in pixels from the top of the screen until the first row
                    {50 + static_cast<float>(j) * 150.75f, 200 + static_cast<float>(i) * 145.75f}, 
                    NULL_BOX_FILL, NULL_BOX_TEXT_COLOR, 2
                    );
                
                if((*ht)->buckets[idx] != NULL){
                    boxes[idx].setOutlineColor(color::Green);
                    boxes[idx].writeHashTableText(
                        "K: " + std::string(static_cast<char*>((*ht)->buckets[idx]->key)), 
                        "V: " + std::string(static_cast<char*>((*ht)->buckets[idx]->val)), idx, 
                        &font, color::Yellow, CUSTOM_CYAN, CUSTOM_PURPLE   
                    );
                }else{
                    boxes[idx].setOutlineColor(NULL_BOX_OUTLINE);
                    boxes[idx].writeHashTableText("", "NULL", idx, &font);
                }
            }
        }
    }
}

void renderBucketsToScreen(std::vector<TextBox>& boxes, renderWin& window){
    for(auto& box: boxes){
        box.renderToScreen(window);
    }
}

int main(){
    renderWin window(sf::VideoMode(820, 900), "HashTable Illustration");
    sfview view(window.getDefaultView());

    std::unique_ptr<HashTable*> hash_t = std::make_unique<HashTable*>(ht_create(10, fnv_hash_func, LINEAR_PROBING));
    std::vector<TextBox> boxes((*hash_t)->capacity , TextBox());

    font mJosefinSans;
    if(!mJosefinSans.loadFromFile("./Fonts/JosefinSans-Bold.ttf")){
        std::cout << "Couldn't load font from disk\n";
        return -1;
    }
    UIStyle josefinSansFontStyle = {
        .font = mJosefinSans
    };

    createBuckets(boxes, hash_t, mJosefinSans);
    Button insertButton("Insert", {50, 30}, {100, 50}, josefinSansFontStyle);
    DynamicTextBox keyTextBox({50, 100}, {180, 50}, mJosefinSans, 28);
    DynamicTextBox valueTextBox({350, 100}, {180, 50}, mJosefinSans, 28);
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

        // Update and render the text boxes
        for(auto& textBox : textBoxes){
            textBox.update(window);
        }

        renderBucketsToScreen(boxes, window);
        insertButton.renderButtonToScreen(window);

        if(insertButton.is_clicked(window, CUSTOM_CYAN)){
            const std::string_view& key = textBoxes[0].getInputString();
            const std::string_view& value = textBoxes[1].getInputString();
            const size_t valueLen = textBoxes[1].getInputLength();
            ht_insert(*hash_t, key.data(), value.data(), valueLen);
            createBuckets(boxes, hash_t, mJosefinSans);
            
            // *Check TextBox inner text
            std::cout << textBoxes[1].getInputString() << '\n';
        }

        window.display();
    }


    return 0;
}
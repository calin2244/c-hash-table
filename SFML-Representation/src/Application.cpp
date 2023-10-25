#include "../include/Application.hpp"

typedef void (*PrintHelper)(size_t, const char*, const void*);

void print_string_string(size_t hash, const char* key, const void* val){
    printf("Index:%li, Key: %s, Value: %s\n", hash, (char*)key, (char*)val);
}

void ht_print(HashTable* ht, PrintHelper printHasht){
    for(size_t hash = 0; hash < ht->capacity; ++hash){
        Ht_Item* item = ht->buckets[hash];

        // Open Addressing Check
        if(item && item->is_tombstone)
            continue;

        if(item){
            printHasht(hash, ht->buckets[hash]->key, ht->buckets[hash]->val);
            if(item->next){
                (void)printf("-----------------------\nChained with: \n");
                item = item->next;
                while(item){
                    (void)printf("[%s: %s]", (char*)item->key, (char*)item->val);
                    item = item->next;
                    if(item)
                        (void)printf(", ");
                }
                (void)printf("\n-----------------------\n\n");
            }
        }
    }
}

Application::Application(FontManager& fontManager, size_t tableSize, hashFunc hF, CollisionResolution collR) noexcept: fontManager(fontManager){
    // Create the application window
    window.create(sf::VideoMode(1100, 900), "HashTable Illustration");
    view = window.getDefaultView();

    // Initialize the FontManager and load fonts and the App's global font
    fontManager.loadFont("josefin-sans-bold", "./Fonts/JosefinSans-Bold.ttf");
    fontManager.loadFont("roman-bold", "./Fonts/times_new_roman_bold.ttf");
    appFont = fontManager.getFont("josefin-sans-bold");

    // Initialize the hash table
    hashTable = std::make_unique<HashTable*>(ht_create(tableSize, hF, collR));
    Application::testInsertQueries(*hashTable);

    // Define custom UIStyle elements
    UIStyle josefinSansFontStyle{appFont};
    UIStyle actionButtonsStyle{appFont, color::Black, color::White, CUSTOM_PURPLE};

    // Create and set up GUI elements
    insertButton = Button({50, 30}, {80, 40}, actionButtonsStyle, "Insert", 16);
    deleteButton = Button({150, 30}, {80, 40}, actionButtonsStyle, "Delete", 16);
    clearButton = Button({250, 30}, {80, 40}, actionButtonsStyle, "Clear HT", 16);
    changeFontButton = Button({350, 30}, {100, 40}, actionButtonsStyle, "Change Font", 14);
    
    std::string option;
    switch(collR){
        case CollisionResolution::CHAINING:
            option = "CHAINING";
            break;
        case CollisionResolution::LINEAR_PROBING:
            option = "LP";
            break;
        case CollisionResolution::QUADRATIC_PROBING:
            option = "QP";
            break;
    }

    std::cout << "Option: " << option << '\n';

    dropdown = Dropdown({550, 40}, {"CHAINING", "LP", "QP"}, option, &appFont);

    // Create and set up DynamicTextBoxes
    textBoxes.emplace_back(DynamicTextBox{{50, 130}, {180, 50}, appFont, 28, "./sprites/clear-text.png"});
    textBoxes.emplace_back(DynamicTextBox{{350, 130}, {180, 50}, appFont, 28, "./sprites/clear-text.png"});
    textBoxes[0].setLabel("Key", josefinSansFontStyle, 20.f);
    textBoxes[1].setLabel("Value", josefinSansFontStyle, 20.f);

    createBuckets(buckets, hashTable, window.getSize().x, &appFont);
}

Application::~Application() noexcept{
    if(*hashTable != NULL){
        free_ht(&(*hashTable));
    }
}

void Application::run() {
    while (window.isOpen()) {
        // Handle events
        handleEvents();

        // Clear the window
        window.clear();

        // Render the application content
        render();

        // Display the rendered content
        window.display();
    }
}

void Application::handleEvents(){
    event event;
    while(window.pollEvent(event)){
        switch(event.type){
            case event::Closed:
                free_ht(&(*hashTable));
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
            case event::Resized:
                createBuckets(buckets, hashTable, window.getSize().x, &appFont);
                break;
            default:
                break;
        }
    }
    dropdown.handleEvent(event);
    insertButton.handleEvent(event, CUSTOM_CYAN);

    // if(insertButton.is_clicked(event, CUSTOM_CYAN)){
    //     if(!textBoxes[0].isEmpty() && !textBoxes[1].isEmpty()){
    //         const std::string_view& key = textBoxes[0].getInputAsString();
    //         const std::string_view& value = textBoxes[1].getInputAsString();
    //         const size_t valueLen = textBoxes[1].getInputLength();
    //         ht_insert(*hashTable, key.data(), value.data(), valueLen + 1);
    //         createBuckets(buckets, hashTable, window.getSize().x, &appFont);
    //     }
    // }

    if(deleteButton.is_clicked2(window, CUSTOM_CYAN)){
        if(!textBoxes[0].isEmpty()){
            const size_t idx = ht_remove(*hashTable, textBoxes[0].getInputAsCharArr());
            std::cout << idx << '\n';
            ht_print(*hashTable, print_string_string);
            if(idx != SIZE_MAX){
                updateBucket(buckets, idx, Operation::DELETION);
            }
        }
    }

    // if(clearButton.is_clicked(event, CUSTOM_CYAN)){
    //     ht_clear(*hashTable);
    //     createBuckets(buckets, hashTable, window.getSize().x, &appFont);
    // }

    // if(changeFontButton.is_clicked(event, CUSTOM_CYAN)){
    //     if(fontManager.getActiveFont() == "josefin-sans-bold"){
    //         appFont = fontManager.getFont("roman-bold");
    //     }
    //     else{
    //         appFont = fontManager.getFont("josefin-sans-bold");
    //     }
    // }
}

void Application::render(){
    // Update and render the text buckets
    for(auto& textBox : textBoxes){
        textBox.update(window);
    }

    // Render HashTable's Buckets to screen
    renderBucketsToScreen(buckets, window);
    renderActionButtonsToScreen<Button>(window, insertButton, deleteButton, clearButton, changeFontButton);
    dropdown.draw(window);
}

void Application::createBuckets(std::vector<TextBox>& buckets, std::unique_ptr<HashTable*>& ht, float screenWidth, font* font){
    // Calculate the number of columns based on the screen width and desired bucket size
    float bucketWidth = 150.75f; // Adjust this value as needed
    int cols = static_cast<int>(screenWidth / bucketWidth);

    if(buckets.size() != (*ht)->capacity){
        buckets.resize((*ht)->capacity);
        // Initialize the new elements after resizing
        for(auto& bucket: buckets){
            bucket = TextBox(font);
        }
    }


    int rows = (buckets.size() + cols - 1) / cols;

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            size_t idx = j + i * cols;
            if(idx < buckets.size()){
                buckets[idx].generateTextBox(
                    {130, 95},
                    {50 + static_cast<float>(j) * bucketWidth, 220 + static_cast<float>(i) * 145.75f},
                    NULL_BOX_FILL, NULL_BOX_TEXT_COLOR, 2
                );

                // *Tombstone check for OPEN-ADDRESSING Collision Resolutions
                if((*ht)->buckets[idx] != NULL && !(*ht)->buckets[idx]->is_tombstone){
                    buckets[idx].setOutlineColor(color::Green);
                    buckets[idx].writeHashTableText(
                        "K: " + std::string(static_cast<char*>((*ht)->buckets[idx]->key)),
                        "V: " + std::string(static_cast<char*>((*ht)->buckets[idx]->val)), idx,
                        color::Yellow, CUSTOM_CYAN, CUSTOM_PURPLE
                    );
                }else{
                    buckets[idx].setOutlineColor(NULL_BOX_OUTLINE);
                    buckets[idx].writeHashTableText("", "NULL", idx);
                }
            }
        }
    }
}

void Application::testInsertQueries(HashTable* hashT){
    ht_insert(hashT, "Eu", "Aries", 6);
    ht_insert(hashT, "Tu", "Taurus", 7);
    ht_insert(hashT, "El", "Gemini", 8);
    ht_insert(hashT, "Ea", "Cancer", 8);
    // ht_insert(hashT, "Noi", "Leo", 4);
    // ht_insert(hashT, "Voi", "Virgo", 6);
}

void Application::updateBucket(std::vector<TextBox>& buckets, const size_t idx, Operation flag, std::string_view key, std::string_view val) noexcept{
    switch(flag){
        case Operation::INSERTION:
            // TODO
            break;
        case Operation::DELETION:
            buckets[idx].setOutlineColor(NULL_BOX_OUTLINE);
            buckets[idx].writeHashTableText("", "NULL", idx);
            break;
        default:
            break;
    }
}

void Application::renderBucketsToScreen(std::vector<TextBox>& buckets, renderWin& window) noexcept{
    for(auto& box: buckets){
        box.renderToScreen(window);
    }
}

void Application::setHashTableSize(size_t size) noexcept{
    // Store the current collision resolution and hash function
    CollisionResolution currentCollResolution = (*hashTable)->coll_resolution;
    auto currentHashFunc  = (*hashTable)->hash_func;

    if (*hashTable != NULL){
        free_ht(&(*hashTable));
    }

    // Create a new HashTable with the desired size, using the stored values
    *hashTable = ht_create(size, currentHashFunc, currentCollResolution);
    // TODO: delete, just for testing purposes
    testInsertQueries(*hashTable);
    createBuckets(buckets, hashTable, window.getSize().x, &appFont);
}



template <typename... Buttons>
void Application::renderActionButtonsToScreen(renderWin& window, const Buttons&... buttons) noexcept{
    (buttons.renderToScreen(window), ...);
}

// TODO: think how to approach this(from a user's perspective)
void Application::loadFont(font& font){
    return;
}
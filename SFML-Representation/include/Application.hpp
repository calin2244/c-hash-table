#pragma once

#include "./TextBox.hpp"       // Include necessary headers for TextBox, Button, etc.
#include "./Button.hpp"
#include "./DynamicTextBox.hpp"
#include "./Dropdown.hpp"
#include "./FontManager.hpp"
#include "../../src/hash_map.h"
#include <vector>
#include <memory>

using namespace SFMLAliases;

enum class Operation: uint16_t{
    INSERTION = 0x01,
    DELETION = 0x02
};

typedef size_t (*hashFunc)(const char*, size_t);

class Application {
    renderWin window;
    sfview view;
    FontManager& fontManager;
    font appFont;
    std::unique_ptr<HashTable*> hashTable;
    std::vector<TextBox> buckets;
    std::vector<DynamicTextBox> textBoxes;
    Button insertButton;
    Button deleteButton;
    Button clearButton;
    Button changeFontButton;
    Dropdown dropdown;

    void handleEvents();
    void render();
    void createBuckets(std::vector<TextBox>& buckets, std::unique_ptr<HashTable*>& ht, float screenWidth, font* font);
    void testInsertQueries(HashTable* hashT);
    void updateBucket(std::vector<TextBox>& buckets, const size_t idx, Operation flag, std::string_view key = nullptr, std::string_view val = nullptr) noexcept;
    void renderBucketsToScreen(std::vector<TextBox>& buckets, renderWin& window) noexcept;

    template <typename... Buttons>
    void renderActionButtonsToScreen(renderWin& window, const Buttons&... buttons) noexcept;

public:
    Application(FontManager& fontManager, size_t tableSize = 13, hashFunc hF = fnv_hash_func, CollisionResolution collR = LINEAR_PROBING) noexcept;
    ~Application() noexcept;
    void run();
    void setHashTableSize(size_t size) noexcept;
    void loadFont(font& font);
};

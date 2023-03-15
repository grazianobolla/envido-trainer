#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>

#include "log.h"

enum TYPE
{
    SWORD,
    CUP,
    COIN,
    CLUB
};

struct Card
{
    TYPE type;
    int number;
    sf::Sprite sprite;

    bool used = false;
    bool hidden = false;
};

class Deck
{
public:
    const int DECK_SIZE = 40;
    void create(int size_w, int size_h, std::string tex_path, float scale);
    void generate();
    void reset();

    float size_x = 0, size_y = 0;

    Card *get_unique_random();
    Deck() {}

private:
    sf::Texture deck_texture;
    std::vector<Card> deck;
    float draw_scale = 1;
    int card_w, card_h;
};

void Deck::reset()
{
    for (Card &card : deck)
    {
        card.used = false;
        card.hidden = false;
    }

    log("deck restarted");
}

void Deck::create(int size_w, int size_h, std::string tex_path, float scale)
{
    deck_texture.loadFromFile(tex_path);
    deck_texture.setSmooth(true);

    card_w = size_w;
    card_h = size_h;
    draw_scale = scale;

    size_x = card_w * draw_scale;
    size_y = card_h * draw_scale;

    log("texture " << tex_path << " loaded");
}

Card *Deck::get_unique_random()
{
    while (true)
    {
        int i = std::rand() % this->deck.size();
        Card *c = &this->deck[i];

        if (c->used == true)
            continue;

        c->used = true;
        return c;
    }
}

// generate deck of spanish cards
void Deck::generate()
{
    for (int type = 0; type < 4; type++)
    {
        for (int number = 1; number <= 12; number++)
        {
            if (number == 9 || number == 8)
                continue;

            int x = (number - 1) * this->card_w;
            int y = type * this->card_h;

            sf::Sprite sprite(
                this->deck_texture,
                sf::IntRect(x, y, this->card_w, this->card_h));

            sprite.setScale(draw_scale, draw_scale);

            Card card{(TYPE)type, number, sprite};
            deck.push_back(card);
        }
    }

    log("deck generated");
}
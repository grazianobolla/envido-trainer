#include "engine.h"
#include "deck.h"
#include "calculator.h"

const int GUESSING_AMOUNT = 3;
const int HAND_AMOUNT = 3 + 1; // hand plus 'muestra'

class EnvidoTrainer : public Engine
{
    Deck deck;
    Card *playing_cards[HAND_AMOUNT];
    Card guessing_cards[GUESSING_AMOUNT];

    bool game_ready = false;

    virtual void start()
    {
        deck.create(208, 319, "deck_texture.png", 0.5f);
        deck.generate();

        game_cycle();
    }

    virtual void input(sf::Event *event)
    {
        if (event->type == sf::Event::KeyPressed)
        {
            if (event->key.code == sf::Keyboard::Space)
            {
                game_cycle();
            }
        }

        if (event->type == sf::Event::MouseButtonPressed)
        {
            if (event->mouseButton.button == sf::Mouse::Button::Left)
            {
                Card *c = get_card_under_cursor();

                if (c != NULL)
                {
                    card_clicked(c);
                }
            }
        }
    }

    virtual void render(sf::RenderWindow *window)
    {
        if (!game_ready)
            return;

        for (int i = 0; i < HAND_AMOUNT; i++)
        {
            window->draw(playing_cards[i]->sprite);
        }

        for (int i = 0; i < GUESSING_AMOUNT; i++)
        {
            window->draw(guessing_cards[i].sprite);
        }
    }

    void generate_playing_cards(int hide_index)
    {
        // generate 'muestra' and hand cards
        for (int i = 0; i < HAND_AMOUNT; i++)
        {
            playing_cards[i] = deck.get_unique_random();
            playing_cards[i]->sprite.setPosition(i * deck.size_x, 0);
        }

        // hide one of the hand cards
        playing_cards[hide_index]->hidden = true;

        log("generated hand and muestra");
    }

    void generate_guessing_cards(int hide_index)
    {
        // generate guessing cards (one is always the hidden one)
        guessing_cards[0] = *playing_cards[hide_index];
        guessing_cards[0].sprite.setPosition(0, deck.size_y);

        for (int i = 1; i < GUESSING_AMOUNT; i++)
        {
            guessing_cards[i] = *deck.get_unique_random();
            guessing_cards[i].sprite.setPosition(i * deck.size_x, deck.size_y);
        }

        log("generated guess cards");
    }

    void game_cycle()
    {
        game_ready = false;
        deck.reset();

        int hidden_index = (std::rand() % 3) + 1;
        generate_playing_cards(hidden_index);
        generate_guessing_cards(hidden_index);
        log("envido: " << get_hand_value(playing_cards));

        game_ready = true;
    }

    Card *get_card_under_cursor()
    {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (Card &c : guessing_cards)
        {
            sf::FloatRect bounds = c.sprite.getGlobalBounds();
            if (bounds.contains(mouse))
            {
                return &c;
            }
        }

        return nullptr;
    }

    void card_clicked(Card *card)
    {
        log(card->number);
    }
};

int main()
{
    EnvidoTrainer engine;
    engine.create(800, 600, "Envido Trainer", sf::Color(51, 102, 0));
    engine.run();
    return 0;
}
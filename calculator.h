#pragma once
#include <vector>
#include <map>

#include "deck.h"

int get_single_envido_value(int number)
{
    if (number == 10 || number == 11 || number == 12)
    {
        return 0;
    }

    return number;
}

int get_pieza_value(int number)
{
    if (number == 2)
        return 30;

    if (number == 4)
        return 29;

    if (number == 5)
        return 28;

    if (number == 10 || number == 11)
        return 27;

    log("get_pieza_value error");
    return 0;
}

std::vector<int> get_pieza_count(Card **hand)
{
    Card *muestra = hand[0];
    std::vector<int> pieza_indexes;

    for (int i = 1; i < 4; i++)
    {
        Card *card = hand[i];
        if (card->type == muestra->type)
        {
            if (get_pieza_value(card->number) > 0)
            {
                pieza_indexes.push_back(i);
            }
        }
    }

    return pieza_indexes;
}

bool is_flor(Card **hand)
{
    Card *muestra = hand[0];

    // case all same suit
    if ((hand[1]->type == hand[2]->type) && (hand[1]->type == hand[3]->type))
    {
        return true;
    }

    auto piezas = get_pieza_count(hand);

    // check for 'flor' (case 2+ piezas)
    if (piezas.size() >= 2)
    {
        return true;
    }

    // case pieza and two same suit
    if (piezas.size() == 1)
    {
        std::vector<int> types;
        int pieza_index = piezas[0];

        for (int i = 1; i < 4; i++)
        {
            if (i == pieza_index)
                continue;

            types.push_back(hand[i]->type);
        }

        if (types[0] == types[1])
        {
            return true;
        }
    }

    return false;
}

int get_hand_value(Card **hand)
{
    if (is_flor(hand) == false)
    {
        auto pieza_indexes = get_pieza_count(hand);
        if (pieza_indexes.size() > 1)
        {
            log("error! not flor but more than 1 pieza");
            return 99;
        }

        log("piezas count " << pieza_indexes.size());

        if (pieza_indexes.size() == 1)
        {
            int pieza_value = get_pieza_value(hand[pieza_indexes[0]]->number);
            std::vector<int> remaining_values;

            for (int i = 1; i < 4; i++)
            {
                if (i == pieza_indexes[0])
                    continue;

                remaining_values.push_back(hand[i]->number);
            }

            int a = get_single_envido_value(remaining_values[0]);
            int b = get_single_envido_value(remaining_values[1]);

            if (a > b)
                pieza_value += a;
            else
                pieza_value += b;

            return pieza_value;
        }

        if (pieza_indexes.size() == 0)
        {
            int envido_value = 0;
            int add_type = -1;

            std::map<int, int> cards; // suit count
            for (int i = 1; i < 4; i++)
            {
                cards[hand[i]->type] += 1;
            }

            for (auto const &item : cards)
            {
                if (item.second == 2) // two cards are the same
                {
                    envido_value += 20;
                    add_type = item.first;
                    break;
                }
            }

            for (int i = 1; i < 4; i++)
            {
                if (hand[i]->type == add_type)
                {
                    int number = hand[i]->number;
                    if (get_single_envido_value(number) == 0)
                    {
                        continue;
                    }

                    envido_value += number;
                }
            }

            return envido_value;
        }
    }

    return 999;
}
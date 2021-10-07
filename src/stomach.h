#pragma once
#ifndef CATA_SRC_STOMACH_H
#define CATA_SRC_STOMACH_H

#include <map>

#include "calendar.h"
#include "type_id.h"

class Character;
class game;
class JsonIn;
class JsonOut;

// Separate struct for nutrients so that we can easily perform arithmetic on
// them
struct nutrients {
    /** amount of kcal this food has */
    int kcal = 0;

    /** vitamins potentially provided by this comestible (if any) */
    std::map<vitamin_id, int> vitamins;

    /** Replace the values here with the minimum (or maximum) of themselves and the corresponding
     * values taken from r. */
    void min_in_place( const nutrients &r );
    void max_in_place( const nutrients &r );

    int get_vitamin( const vitamin_id & ) const;

    bool operator==( const nutrients &r ) const;
    bool operator!=( const nutrients &r ) const {
        return !( *this == r );
    }

    nutrients &operator+=( const nutrients &r );
    nutrients &operator-=( const nutrients &r );
    nutrients &operator*=( int r );
    nutrients &operator/=( int r );

    friend nutrients operator*( nutrients l, int r ) {
        l *= r;
        return l;
    }

    friend nutrients operator/( nutrients l, int r ) {
        l /= r;
        return l;
    }
};

// Contains all information that can pass out of (or into) a stomach
struct food_summary {
    nutrients nutr;
};

#endif // CATA_SRC_STOMACH_H

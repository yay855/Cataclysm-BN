#include <algorithm>
#include <cmath>
#include <utility>

#include "cata_utility.h"
#include "character.h"
#include "json.h"
#include "player.h"
#include "stomach.h"
#include "vitamin.h"

void nutrients::min_in_place( const nutrients &r )
{
    kcal = std::min( kcal, r.kcal );
    for( const std::pair<const vitamin_id, vitamin> &vit_pair : vitamin::all() ) {
        const vitamin_id &vit = vit_pair.first;
        int other = r.get_vitamin( vit );
        if( other == 0 ) {
            vitamins.erase( vit );
        } else {
            auto our_vit = vitamins.find( vit );
            if( our_vit != vitamins.end() ) {
                our_vit->second = std::min( our_vit->second, other );
            }
        }
    }
}

void nutrients::max_in_place( const nutrients &r )
{
    kcal = std::max( kcal, r.kcal );
    for( const std::pair<const vitamin_id, vitamin> &vit_pair : vitamin::all() ) {
        const vitamin_id &vit = vit_pair.first;
        int other = r.get_vitamin( vit );
        if( other != 0 ) {
            int &val = vitamins[vit];
            val = std::max( val, other );
        }
    }
}

int nutrients::get_vitamin( const vitamin_id &vit ) const
{
    auto it = vitamins.find( vit );
    if( it == vitamins.end() ) {
        return 0;
    }
    return it->second;
}

bool nutrients::operator==( const nutrients &r ) const
{
    if( kcal != r.kcal ) {
        return false;
    }
    // Can't just use vitamins == r.vitamins, because there might be zero
    // entries in the map, which need to compare equal to missing entries.
    for( const std::pair<const vitamin_id, vitamin> &vit_pair : vitamin::all() ) {
        const vitamin_id &vit = vit_pair.first;
        if( get_vitamin( vit ) != r.get_vitamin( vit ) ) {
            return false;
        }
    }
    return true;
}

nutrients &nutrients::operator+=( const nutrients &r )
{
    kcal += r.kcal;
    for( const std::pair<const vitamin_id, int> &vit : r.vitamins ) {
        vitamins[vit.first] += vit.second;
    }
    return *this;
}

nutrients &nutrients::operator-=( const nutrients &r )
{
    kcal -= r.kcal;
    for( const std::pair<const vitamin_id, int> &vit : r.vitamins ) {
        vitamins[vit.first] -= vit.second;
    }
    return *this;
}

nutrients &nutrients::operator*=( int r )
{
    kcal *= r;
    for( std::pair<const vitamin_id, int> &vit : vitamins ) {
        vit.second *= r;
    }
    return *this;
}

nutrients &nutrients::operator/=( int r )
{
    kcal = divide_round_up( kcal, r );
    for( std::pair<const vitamin_id, int> &vit : vitamins ) {
        vit.second = divide_round_up( vit.second, r );
    }
    return *this;
}

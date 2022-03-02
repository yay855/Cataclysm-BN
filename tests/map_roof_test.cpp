#include <memory>
#include <sstream>
#include <vector>

#include "avatar.h"
#include "catch/catch.hpp"
#include "enums.h"
#include "game.h"
#include "game_constants.h"
#include "map.h"
#include "map_helpers.h"
#include "point.h"
#include "type_id.h"

static const std::string &to_string( bool b )
{
    static const std::string truth( "true" );
    static const std::string falsehood( "false" );
    return b ? truth : falsehood;
}

static std::ostream &operator<<( std::ostream &os, const bash_results &results )
{
    std::stringstream subresult_ss;
    for( const bash_results &sub : results.subresults ) {
        subresult_ss << sub;
    }
    os << string_format( "{ \"did_bash\": %s, \"success\": %s, \"bashed_solid\": %s, \"subresults\": [\n  %s\n] }",
                         to_string( results.did_bash ), to_string( results.success ), to_string( results.bashed_solid ),
                         subresult_ss.str() );
    return os;
}

TEST_CASE( "Successfully bashing down a roof doesn't leave ledges above unpassable terrain" )
{
    constexpr const tripoint wall_loc = tripoint( 60, 60, 0 );
    constexpr const tripoint roof_loc = wall_loc + tripoint_above;
    clear_map();

    const std::vector<ter_t> &all_terrain = ter_t::get_all();
    map &here = get_map();
    REQUIRE( here.has_zlevels() );

    for( const ter_t &t : all_terrain ) {
        if( !t.roof || t.roof->bash.destroy_only ) {
            continue;
        }

        int success_count = 0;
        std::array<int, 4> values_to_check = {
            t.roof->bash.str_min,
            t.roof->bash.str_max,
            t.roof->bash.str_min_supported,
            t.roof->bash.str_max_supported
        };
        std::sort( values_to_check.begin(), values_to_check.end() );
        std::string recur;
        for( int str : values_to_check ) {
            if( str < 0 ) {
                continue;
            }

            here.ter_set( wall_loc, t.id );
            here.ter_set( roof_loc, t.roof );

            static const std::array<float, 2> mults = {{
                    0.0f, 1.0f
                }
            };
            for( float f : mults ) {
                bash_params params{
                    str, true, false, true, f, false
                };
                bash_results result = here.bash_ter_furn( roof_loc, params );
                if( result.success ) {
                    const ter_id &new_ter = here.ter( roof_loc );
                    std::stringstream result_ss;
                    result_ss << result;
                    CAPTURE( t.id.str() );
                    CAPTURE( result_ss.str() );
                    CHECK( new_ter.id().str() != t.roof.str() );
                    CHECK( new_ter.id().str() == t_open_air.id().str() );
                    CHECK( here.passable( wall_loc ) );
                    CHECK( !result.subresults.empty() );
                    success_count++;
                }
            }
        }
        CAPTURE( t.id.str() );
        CAPTURE( values_to_check );
        CHECK( success_count > 0 );
    }
}

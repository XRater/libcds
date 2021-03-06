#include "test_nc_feldman_hashset_hp.h"

#include <cds/container/nc_feldman_hashset_dhp.h>
#include <cds/container/iterable_list_dhp.h>

namespace {
    namespace cc = cds::container;
    typedef cds::gc::DHP gc_type;

    class FeldmanHashSet_DHP : public cds_test::nc_feldman_hashset_hp
    {
    protected:
        typedef cds_test::nc_feldman_hashset_hp base_class;

        struct traits : public cc::iterable_list::traits
        {
            typedef cds::atomicity::item_counter item_counter;
            typedef base_class::less less;
        };

        typedef cc::IterableList<gc_type, int_item, traits> list;        

        void SetUp()
        {
            struct set_traits: public cc::feldman_hashset::traits
            {
                typedef get_hash hash_accessor;
                typedef list list_type;
                typedef base_class::equal value_equal;
            };
            typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

            cds::gc::dhp::smr::construct( set_type::c_nHazardPtrCount );
            cds::threading::Manager::attachThread();
        }

        void TearDown()
        {
            cds::threading::Manager::detachThread();
            cds::gc::dhp::smr::destruct();
        }
    };


    TEST_F( FeldmanHashSet_DHP, defaulted )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s;
        test( s );
    }


    TEST_F( FeldmanHashSet_DHP, compare )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef cmp compare;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 4, 5 );
        test( s );
    }

    TEST_F( FeldmanHashSet_DHP, less )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef std::less<int> less;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 3, 2 );
        test( s );
    }

    TEST_F( FeldmanHashSet_DHP, cmpmix )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef std::less<int> less;
            typedef cmp compare;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 4, 4 );
        test( s );
    }

    TEST_F( FeldmanHashSet_DHP, item_counting )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef cmp compare;
            typedef std::less<int> less;
            typedef simple_item_counter item_counter;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 3, 3 );
        test( s );
    }

    TEST_F( FeldmanHashSet_DHP, backoff )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef cmp compare;
            typedef cds::atomicity::item_counter item_counter;
            typedef cds::backoff::yield back_off;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 8, 2 );
        test( s );
    }

    TEST_F( FeldmanHashSet_DHP, stat )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef cds::backoff::yield back_off;
            typedef cc::feldman_hashset::stat<> stat;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 1, 1 );
        test( s );
    }

    TEST_F( FeldmanHashSet_DHP, byte_cut )
    {
        struct set_traits: public cc::feldman_hashset::traits
        {
            typedef get_hash hash_accessor;
            typedef list list_type;
            typedef base_class::equal value_equal;
            typedef cds::algo::byte_splitter<int> hash_splitter;
            typedef cmp compare;
        };
        typedef cc::FeldmanHashSet< gc_type, int_item, set_traits > set_type;

        set_type s( 8, 8 );
        test( s );
    }

} // namespace

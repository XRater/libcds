#include "test_intrusive_nc_feldman_hashset_hp.h"

#include <cds/intrusive/nc_feldman_hashset_hp.h>
#include <cds/intrusive/iterable_list_hp.h>

namespace {
    namespace ci = cds::intrusive;
    typedef cds::gc::HP gc_type;

    class IntrusiveFeldmanHashSet_HP : public cds_test::intrusive_nc_feldman_hashset_hp
    {
    protected:
        typedef cds_test::intrusive_nc_feldman_hashset_hp base_class;
    
        struct traits : public ci::iterable_list::traits
        {
            typedef cds::atomicity::item_counter item_counter;
            typedef base_class::less less;
        };

        typedef ci::IterableList<gc_type, int_item, traits> list;        
        
    protected:

        void SetUp()
        {
            struct traits : public ci::feldman_hashset::traits
            {
                typedef base_class::hash_accessor hash_accessor;
                typedef base_class::equal value_equal;
                typedef cmp compare;
                typedef mock_disposer disposer;
                typedef list list_type;
            };

            typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

            // +1 - for guarded_ptr
            cds::gc::hp::GarbageCollector::Construct( set_type::c_nHazardPtrCount + 1, 1, 16 );
            cds::threading::Manager::attachThread();
        }

        void TearDown()
        {
            cds::threading::Manager::detachThread();
            cds::gc::hp::GarbageCollector::Destruct( true );
        }
    };

    TEST_F( IntrusiveFeldmanHashSet_HP, compare )
    {        
        struct traits : public ci::feldman_hashset::traits
        {
            typedef base_class::hash_accessor hash_accessor;
            typedef base_class::equal value_equal;
            typedef cmp compare;
            typedef mock_disposer disposer;
            typedef list list_type;
        };

        typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

        set_type s;
        test( s );
    }


    TEST_F( IntrusiveFeldmanHashSet_HP, less )
    {
 
        struct traits : public ci::feldman_hashset::traits
        {
            typedef base_class::hash_accessor hash_accessor;
            typedef base_class::equal value_equal;
            typedef std::less<int> less;
            typedef mock_disposer disposer;
            typedef list list_type;
        };

        typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

        set_type s( 5, 2 );
        test( s );
    }

    TEST_F( IntrusiveFeldmanHashSet_HP, cmpmix )
    {
        struct traits : public ci::feldman_hashset::traits
        {
            typedef base_class::hash_accessor hash_accessor;
            typedef base_class::equal value_equal;
            typedef cmp compare;
            typedef std::less<int> less;
            typedef mock_disposer disposer;
            typedef simple_item_counter item_counter;
            typedef list list_type;
        };

        typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

        set_type s( 2, 5 );
        test( s );
    }

    TEST_F( IntrusiveFeldmanHashSet_HP, backoff )
    {
        struct traits : public ci::feldman_hashset::traits
        {
            typedef base_class::hash_accessor hash_accessor;
            typedef base_class::equal value_equal;
            typedef cmp compare;
            typedef mock_disposer disposer;
            typedef cds::backoff::empty back_off;
            typedef ci::opt::v::sequential_consistent memory_model;
            typedef list list_type;
        };

        typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

        set_type s( 8, 3 );
        test( s );
    }

    TEST_F( IntrusiveFeldmanHashSet_HP, stat )
    {
        struct traits : public ci::feldman_hashset::traits
        {
            typedef base_class::hash_accessor hash_accessor;
            typedef base_class::equal value_equal;
            typedef cmp compare;
            typedef mock_disposer disposer;
            typedef ci::feldman_hashset::stat<> stat;
            typedef list list_type;
        };

        typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

        set_type s( 8, 3 );
        test( s );
    }

    TEST_F( IntrusiveFeldmanHashSet_HP, byte_cut )
    {
        struct traits: public ci::feldman_hashset::traits
        {
            typedef base_class::hash_accessor hash_accessor;
            typedef base_class::equal value_equal;
            typedef cds::algo::byte_splitter< int > hash_splitter;
            typedef cmp compare;
            typedef std::less<int> less;
            typedef mock_disposer disposer;
            typedef simple_item_counter item_counter;
            typedef list list_type;
        };

        typedef ci::FeldmanHashSet< gc_type, int_item, traits > set_type;

        set_type s( 8, 8 );
        test( s );
    }
} // namespace


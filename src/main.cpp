
#include <iostream>
#include <vector>

#include "relative_ptr.hpp"

struct Node {
  int                  value;
  relative_ptr< Node > left, right;
};

void print_tree( Node* root, size_t indent_level = 0 ) {
  for ( size_t i = 0; i < indent_level; ++i ) {
    printf( "  " );
  }
  printf( "%i\n", root->value );
  if ( root->left )
    // explicit casting needed (see 'include/relative_ptr.hpp' for reason)
    print_tree( static_cast< Node* >( root->left ), indent_level + 1 );
  if ( root->right )
    // explicit casting needed
    print_tree( static_cast< Node* >( root->right ), indent_level + 1 );
}

auto main( ) -> int {
  std::vector< Node > vec = { };

  vec.resize( 3 );
  vec[0].value = 0;
  vec[1].value = 2;
  vec[2].value = 1;

  vec[0].left  = &vec[2];
  vec[0].right = &vec[1];

  printf( "before resize:\n" );
  print_tree( &vec[0] );

  vec.resize( 10 );

  printf( "after resize:\n" );
  print_tree( &vec[0] );
}
#include <fstream>
#include <iostream>
#include <random>

#include "relative_ptr.hpp"

struct Node {
  int                  value;
  relative_ptr< Node > left, right;
};

constexpr uint32_t node_count            = 1024;
static Node        node_pool[node_count] = { };
static size_t      last_node             = 0;

void print_tree( Node* root, size_t level = 0 ) {
  for ( size_t i = 0; i < level; ++i ) {
    printf( "  " );
  }
  printf( "%i\n", root->value );
  if ( root->left )
    print_tree( root->left, level + 1 );
  if ( root->right )
    print_tree( root->right, level + 1 );
}

std::istream& operator>>( std::istream& in, Node& node ) {
  in >> node.value;
  in >> node.left;
  in >> node.right;
  return in;
}

auto main( ) -> int {
  std::fstream file { };
  file.open( "./tree_save.txt", std::ios_base::in );

  if ( !file.is_open( ) ) {
    std::cerr << "ERROR: could not open ./tree_save.txt\n";
    exit( 1 );
  }

  while ( file >> node_pool[last_node++] ) { }

  auto tree = &node_pool[0];
  print_tree( tree );
}
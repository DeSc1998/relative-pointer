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

Node* generate_random_tree( size_t depth = 1 ) {
  if ( depth > 0 ) {
    std::random_device r { };
    std::mt19937       rd { r( ) };
    Node*              ptr = node_pool + last_node++;
    ptr->value             = rd( ) % ( 1 << 16 );
    ptr->left              = generate_random_tree( depth - 1 );
    ptr->right             = generate_random_tree( depth - 1 );
    return ptr;
  } else {
    return nullptr;
  }
}

std::ostream& operator<<( std::ostream& out, const Node& node ) {
  return out << node.value << ' ' << node.left << ' ' << node.right;
}

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

auto main( ) -> int {
  auto tree = generate_random_tree( 3 );
  print_tree( tree );

  std::fstream file { };
  file.open( "./tree_save.txt", std::ios_base::out );

  if ( !file.is_open( ) ) {
    std::cerr << "ERROR: could not open ./tree_save.txt\n";
    exit( 1 );
  }

  for ( size_t i = 0; i < node_count; ++i ) {
    file << node_pool[i] << '\n';
  }

  file.flush( );
}
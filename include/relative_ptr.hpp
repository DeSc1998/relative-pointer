#pragma once

#include <concepts>
#include <istream>
#include <limits>
#include <ostream>

template < typename Type, std::signed_integral Size = long >
class relative_ptr {
public:
  using size_type  = Size;
  using value_type = Type;

private:
  static constexpr size_type default_null
    = std::numeric_limits< size_type >::min( );

  size_type ptr = default_null;

  static size_type encode( size_type raw ) { return raw ^ default_null; }
  static size_type decode( size_type raw ) { return raw ^ default_null; }

  static size_type ptr_diff( const value_type* ptr, const size_type& dest ) {
    const char* rel = reinterpret_cast< const char* >( &dest );
    const char* abs = reinterpret_cast< const char* >( ptr );
    return static_cast< size_type >( abs - rel );
  }

public:
  relative_ptr( )                     = default;
  relative_ptr( const relative_ptr& ) = default;
  relative_ptr( relative_ptr&& )      = default;

  relative_ptr( const size_type& rel )
    : ptr( rel ) { }

  relative_ptr( const value_type* ptr )
    : ptr( ptr != nullptr ? encode( ptr_diff( ptr, this->ptr ) )
                          : default_null ) { }

  relative_ptr& operator=( const relative_ptr& ) = default;
  relative_ptr& operator=( relative_ptr&& )      = default;

  relative_ptr& operator=( const value_type* ptr ) {
    this->ptr
      = ptr != nullptr ? encode( ptr_diff( ptr, this->ptr ) ) : default_null;
    return *this;
  }

  operator bool( ) const { return decode( ptr ) != 0; }
  operator bool( ) { return decode( ptr ) != 0; }

  operator const value_type*( ) const {
    return reinterpret_cast< const value_type* >(
      reinterpret_cast< const char* >( &ptr ) + decode( ptr ) );
  }

  operator value_type*( ) {
    return reinterpret_cast< value_type* >( reinterpret_cast< char* >( &ptr )
                                            + decode( ptr ) );
  }

  value_type*       operator->( ) { return *this; }
  const value_type* operator->( ) const { return *this; }

  value_type& operator*( ) { return *static_cast< value_type* >( *this ); }

  const value_type& operator*( ) const {
    return *static_cast< const value_type* >( *this );
  }

  template < typename T, typename S >
  friend std::ostream& operator<<( std::ostream&, const relative_ptr< T, S >& );

  template < typename T, typename S >
  friend std::istream& operator>>( std::istream&, relative_ptr< T, S >& );
};

template < typename T, typename S >
std::ostream& operator<<( std::ostream& out, const relative_ptr< T, S >& ptr ) {
  return out << ptr.ptr;
}

template < typename T, typename S >
std::istream& operator>>( std::istream& in, relative_ptr< T, S >& ptr ) {
  typename relative_ptr< T, S >::size_type tmp { };
  in >> tmp;
  ptr = relative_ptr< T, S >( tmp );
  return in;
}

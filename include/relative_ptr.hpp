#pragma once

#include <concepts>
#include <limits>

template < typename Type, std::signed_integral Size = int >
class relative_ptr {
  using size_type  = Size;
  using value_type = Type;

  size_type ptr = default_null;

protected:
  static constexpr size_type default_null
    = std::numeric_limits< size_type >::min( );

  virtual size_type encode( size_type raw ) const { return raw ^ default_null; }

  virtual size_type decode( size_type raw ) const { return raw ^ default_null; }

public:
  relative_ptr( const value_type* ptr ) {
    const char* rel = reinterpret_cast< char* >( &this->ptr );
    const char* abs = reinterpret_cast< const char* >( ptr );
    this->ptr       = encode( static_cast< size_type >( abs - rel ) );
  }

  relative_ptr( )                     = default;
  relative_ptr( const relative_ptr& ) = default;
  relative_ptr( relative_ptr&& )      = default;

  relative_ptr& operator=( const value_type* ptr ) {
    const char* rel = reinterpret_cast< char* >( &this->ptr );
    const char* abs = reinterpret_cast< const char* >( ptr );
    this->ptr       = encode( static_cast< size_type >( abs - rel ) );
    return *this;
  }

  relative_ptr& operator=( const relative_ptr& ) = default;
  relative_ptr& operator=( relative_ptr&& )      = default;

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

  const value_type* operator->( ) const {
    return reinterpret_cast< const value_type* >(
      reinterpret_cast< const char* >( &ptr ) + decode( ptr ) );
  }

  value_type* operator->( ) {
    return reinterpret_cast< value_type* >( reinterpret_cast< char* >( &ptr )
                                            + decode( ptr ) );
  }

  value_type& operator*( ) {
    const char*       tmp = reinterpret_cast< char* >( &ptr );
    const value_type* loc
      = reinterpret_cast< value_type* >( tmp + decode( ptr ) );
    return *loc;
  }

  const value_type& operator*( ) const {
    const char* const       tmp = reinterpret_cast< const char* >( &ptr );
    const value_type* const loc
      = reinterpret_cast< const value_type* >( tmp + decode( ptr ) );
    return *loc;
  }

  virtual ~relative_ptr( ) { }
};

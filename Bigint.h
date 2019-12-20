/*
 * Assignment 1: Bigint - Computer Programming 2 (COMP2711, COMP8801) 
 * Jonathan Souter - 2136593 
 */

#ifndef BIGINT_H_
#define BIGINT_H_

#include <iostream>
#include <string>

// =============================================== //
// =============== GLOBAL VARIABLES ============== //
// =============================================== //

#define DIGITS 256

// ================================================ //
// =============== CLASS DEFINITION =============== //
// ================================================ //

class Bigint
{
public:
  // ===== constructors ===== //

  Bigint();      // Creates a Bigint initialised to 0.
  Bigint(int n); // creates a bigint initialised to n

  // ===== i/o operators ===== //

  friend std::ostream &operator<<(std::ostream &out, const Bigint &n);
  friend std::istream &operator>>(std::istream &in, Bigint &n);

  // ===== arithmetic operators ===== //

  friend Bigint operator+(const Bigint &n1, const Bigint &n2);
  friend Bigint operator-(const Bigint &n1, const Bigint &n2);
  friend Bigint operator*(const Bigint &n1, const Bigint &n2);
  friend Bigint operator/(const Bigint &n1, const Bigint &n2);
  friend Bigint operator%(const Bigint &n1, const Bigint &n2);

  // ===== bitwise operators ===== //

  friend Bigint operator<<(const Bigint &n1, const int &n2);
  friend Bigint operator>>(const Bigint &n1, const int &n2);

  // ===== relational operators ===== //

  friend bool operator>(const Bigint &n1, const Bigint &n2);
  friend bool operator<(const Bigint &n1, const Bigint &n2);
  friend bool operator==(const Bigint &n1, const Bigint &n2);
  friend bool operator!=(const Bigint &n1, const Bigint &n2);
  friend bool operator<=(const Bigint &n1, const Bigint &n2);
  friend bool operator>=(const Bigint &n1, const Bigint &n2);

  // ===== assignment operators ===== //

  void operator=(const Bigint &n1);

  // ===== reinitialises the bigint ===== //

  void clear();

  // ===== setters and getters ===== //

  void set_radix(int rad);
  int get_radix();
  void set_digit(int digit, int index);
  int get_digit(int index);

private:
  int radix_;
  bool negative_;
  int length_;
  int digits_[DIGITS];

  // basic arithmetic operators that assume that inputs are positive
  // used by public arithmetic operators
  friend Bigint add(const Bigint &n1, const Bigint &n2);
  friend Bigint subtract(const Bigint &n1, const Bigint &n2);
  friend Bigint multiply(const Bigint &n1, const Bigint &n2);
  friend Bigint divide(Bigint &n1, Bigint &n2);

  // process functions
  bool is_zero();       // more resource efficient and safer than x == Bigint(0)
  void round_up();      // rounds up all digits that are greater than radix
  void update_length(); // counts all digits in a Bigint and updates the length variable

  // testing functions
  int return_int();    // returns the Bigint as an integer if possible
  void raw_contents(); // displays the raw contents of the Bigint array
};

#endif // BIGINT_H_

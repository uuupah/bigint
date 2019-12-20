/*
 * Assignment 1: Bigint - Computer Programming 2 (COMP2711, COMP8801) 
 * Jonathan Souter - 2136593 
 */

#include <iostream>
#include <string>
#include <cmath>
#include "Bigint.h"

// ================================================ //
// =============== GLOBAL VARIABLES =============== //
// ================================================ //

const int INIT_VAL = 0;
const char NUMBERS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// ============================================= //
// =============== CLASS METHODS =============== //
// ============================================= //

// ===== constructors ===== //

// Creates a Bigint initialised to 0
Bigint::Bigint()
{
    for (int i = DIGITS - 1; i >= 0; --i)
    {
        digits_[i] = INIT_VAL;
    }

    radix_ = 10;
    negative_ = false;
    length_ = 1;
}

// Creates a bigint initialised to n
Bigint::Bigint(int n)
{
    int i = DIGITS - 1;
    negative_ = false;
    radix_ = 10;

    clear();

    if (n < 0)
    {
        n *= -1;
        negative_ = true;
    }

    for (int i = DIGITS - 1; i >= 0 && n > 0; i-- )
    {
        int digit = n % radix_;
        n /= radix_;
        digits_[i] = digit;
    }

    update_length();
}

// ===== i/o operators ===== //

// Write bigint to standard out
std::ostream &operator<<(std::ostream &out, const Bigint &n)
{
    bool leading_zero = true;
    std::string s = "";

    for (int i = 0; i < DIGITS; i++)
    {
        // leading zero check
        if (n.digits_[i] != 0 && leading_zero == true)
        {
            leading_zero = false;

            if (n.negative_)
            {
                s += '-';
            }
        }

        if (leading_zero == false) 
        {
            s += NUMBERS[n.digits_[i]];
        }
    }

    // makes the string a zero if it is empty
    if (s == "")
    {
        return out << 0;
    }

    return out << s;
}

// Read bigint from standard in
std::istream &operator>>(std::istream &in, Bigint &n)
{
    // All characters are assumed to be valid digits.
    std::string s;
    if (in >> s)
    {
        for (uint i = 0; i < s.length(); ++i)
        {
            // check for hyphen denoting negative input
            if (s[i] == '-')
            {
                n.negative_ = true;
                n.digits_[i] = 0;
                continue;
            }

            for (uint j = 0; j < n.radix_; j++)
            {
                s[i] = toupper(s[i]); 
                if (s[i] == NUMBERS[j])
                {
                    n.digits_[DIGITS - s.length() + i] = j;
                    break;
                }
            }
        }
    }

    if (n.negative_)
    {
        n.length_ = s.length() - 1;
    }
    else
    {
        n.length_ = s.length();
    }

    // avoiding negative zeros
    if (n.is_zero())
    {
        n.negative_ = false;
    }

    return in;
}

// ===== arithmetic operators ===== //

// Adds n1 and n2 and returns a Bigint result
Bigint operator+(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_);

    int polarity = 0;

    if (n1.negative_)
    {
        polarity += 1;
    }
    if (n2.negative_)
    {
        polarity += 2;
    }

    switch (polarity)
    {
    case 0: // both positive
        result = add(n1, n2);
        break;
    case 1: // n1 negative
        result = subtract(n1, n2);
        result.negative_ = true;
        break;
    case 2: // n2 negative
        result = subtract(n1, n2);
        break;
    case 3: // both negative
        result = add(n1, n2);
        result.negative_ = true;
        break;
    }

    result.update_length();
    return result;
}

// General Bigint addition function
// ! Assumes inputs are positive !
Bigint add(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_);
    int iters;

    if (n1.length_ > n2.length_)
    {
        iters = n1.length_;
    }
    else
    {
        iters = n2.length_;
    }

    // adds all elements in both functions
    for (int i = DIGITS - 1; i >= iters; i--)
    {
        result.digits_[i] += n1.digits_[i] + n2.digits_[i];
    }

    result.round_up();

    return result;
}

// Subtracts n2 from n1 and returns a Bigint result
Bigint operator-(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_);

    int polarity = 0;

    if (n1.negative_)
    {
        polarity += 1;
    }
    if (n2.negative_)
    {
        polarity += 2;
    }

    switch (polarity)
    {
    case 0: // both positive
        result = subtract(n1, n2);
        break;
    case 1: // n1 negative
        result = add(n1, n2);
        result.negative_ = true;
        break;
    case 2: // n2 negative
        result = add(n1, n2);
        break;
    case 3: // both negative
        result = subtract(n1, n2);
        result.negative_ = true;
        break;
    }

    result.update_length();
    return result;
}

// General Bigint subtraction function
// ! Assumes inputs are positive !
Bigint subtract(const Bigint &n1, const Bigint &n2)
{
    int i = 0;
    int n2_size, n1_size, sub = 0, carry = 0;
    Bigint result;
    result.set_radix(n1.radix_);

    while (n1.digits_[i] == 0)
    {
        i++;
    }
    n1_size = DIGITS - i;

    while (n2.digits_[i] == 0)
    {
        i++;
    }
    n2_size = DIGITS - i;

    for (int j = DIGITS - 1; j > DIGITS - n1_size - 1; j--)
    {
        sub = n1.digits_[j] - n2.digits_[j] - carry;

        if (sub < 0)
        {
            sub += result.radix_;
            carry = 1;
        }
        else
        {
            carry = 0;
        }

        result.digits_[j] = sub;
    }

    return result;
}

// Multiplies n2 by n1 and returns a bigint result
Bigint operator*(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_);

    result = multiply(n1, n2);

    if (n1.negative_ != n2.negative_)
    {
        result.negative_ = true;
    }

    result.update_length();
    return result;
}

// General Bigint multiplication function
// ! Assumes inputs are positive !
Bigint multiply(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_);
    int location;

    for (int i = DIGITS - 1; i >= DIGITS - n2.length_; i--) // n2 counter
    {
        for (int j = DIGITS - 1; j >= DIGITS - n1.length_; j--) // n1 counter
        {
            location = DIGITS - (DIGITS - i) - (DIGITS - j);
            result.digits_[location + 1] += n1.digits_[j] * n2.digits_[i];
        }
    }

    result.round_up();

    return result;
}

// Divides n2 by n1 and returns a bigint result
Bigint operator/(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_);
    Bigint m1 = n1, m2 = n2;

    if (n1 == Bigint(0) || n2 == Bigint(0))
    {
        return result;
    }

    if (n1 == n2)
    {
        result = Bigint(1);
        result.update_length();
        return result;
    }

    result = divide(m1, m2);

    if (n1.negative_ != n2.negative_ && result != Bigint(0))
    {
        result.negative_ = true;
    }

    result.update_length();
    return result;
}

// General division function
// ! Assumes inputs are positive ! 
Bigint divide(Bigint &n1, Bigint &n2)
{
    Bigint result = 1;
    result.set_radix(n1.radix_);
    Bigint m2 = n2;
    int flag = false;

    // breakout for when recursive calculation reaches its end
    if (n1 == Bigint(0) || n2 == Bigint(0) || n2 > n1)
    {
        return Bigint(0);
    }

    // bitshift left until the divisor reaches the dividend
    // calculate the number of bitshifts with m2
    while (m2 < n1)
    {
        result = result << 1;
        m2 = m2 << 1;

        flag = true;
    }

    // bitshift right once to normalise
    if (flag)
    {
        result = result >> 1;
        m2 = m2 >> 1;
    }

    // recursively divide remainder that cannot be bitshifted at this scale
    result = result + ((n1 - m2) / n2);

    return result;
}

// Finds remainder of n1 / n2 and returns a Bigint result
Bigint operator%(const Bigint &n1, const Bigint &n2)
{
    Bigint result;
    result.set_radix(n1.radix_); // clunky 

    result = n1 - ((n1 / n2) * n2);
    result.update_length();
    return result;
}

// ===== bitwise operators ===== //

// Bitshift n1 left by n2 steps (doubling)
Bigint operator<<(const Bigint &n1, const int &n2)
{
    Bigint m1 = n1;

    for (int i = 0; i < DIGITS; i++)
    {
        m1.digits_[i] = m1.digits_[i] << n2;
    }
    m1.round_up();

    m1.update_length();
    return m1;
}

// Bitshift n1 right by n2 steps (halving)
Bigint operator>>(const Bigint &n1, const int &n2)
{
    Bigint m1 = n1;

    for (int i = 0; i < n2; i++) // bitshift n2 times
    {
        for (int j = 0; j < DIGITS; j++) // iterate through bigint
        {
            if ((m1.digits_[j] % 2 != 0) && m1.digits_[j] > 0) // carry if n1.digits_[i] is odd
            {
                m1.digits_[j] -= 1; // carry

                if (j != DIGITS - 1)
                {
                    m1.digits_[j + 1] += m1.radix_;
                }
            }

            m1.digits_[j] = m1.digits_[j] >> 1;
        }
    }

    m1.update_length();
    return m1;
}

// ===== relational operators ===== //

// Returns true if n1 is greater than n2
bool operator>(const Bigint &n1, const Bigint &n2)
{

    bool result;    

    // simple checks first to save unnecessary processing
    // if n1 == n2 then n1 !> n2
    if (n1 == n2)
    {
        return false;
    }

    // n1 is less than zero and n2 is not then n1 !> n2
    if (n1.negative_ && !n2.negative_)
    {
        return false;
    }

    // n1 is greater than zero and n2 is not then n1 > n2
    if (!n1.negative_ && n2.negative_)
    {
        return true;
    }   

    // actual greater than check
    for (int i = 0; i < DIGITS; i++)
    {
        if (n1.digits_[i] > n2.digits_[i])
        {
            result = true;
            break;
        }
        else if (n1.digits_[i] < n2.digits_[i])
        {
            result = false;
            break;
        }
    }

    // if both values are negative, then flip the result
    if (n1.negative_ && n2.negative_)
    {
        return !result;
    }

    return result;
}

// Returns true if n1 is less than n2
bool operator<(const Bigint &n1, const Bigint &n2)
{
    return n2 > n1;
}

// Returns true if n1 is equal to n2
bool operator==(const Bigint &n1, const Bigint &n2)
{
    // if one value is negative and the other is not then they cannot be equal (there are no negative zeros)
    if (n1.negative_ != n2.negative_)
    {
        return false;
    }

    Bigint m2 = n2;

    for (int i = 0; i < DIGITS; i++)
    {
        if (n1.digits_[i] != m2.digits_[i])
        {
            return false;
        }
    }

    return true;
}

// Returns true if n1 is not equal to n2
bool operator!=(const Bigint &n1, const Bigint &n2)
{
    return !(n1 == n2);
}

// Returns true if n1 is equal to or less than n2
bool operator<=(const Bigint &n1, const Bigint &n2)
{
    return (n1 < n2 || n1 == n2);
}

// Returns true if n1 is equal to or greater than n2
bool operator>=(const Bigint &n1, const Bigint &n2)
{
    return (n1 > n2 || n1 == n2);
}

// ===== assignment operators ===== //

// Assigns bigint value n
void Bigint::operator=(const Bigint &n)
{
    for (int i = 0; i < DIGITS; i++)
    {
        digits_[i] = n.digits_[i];
    }

    radix_ = n.radix_;
    negative_ = n.negative_;
    update_length();
}

// ===== other methods ===== //

// Reinitialises the Bigint
void Bigint::clear()
{
    for (int i = DIGITS - 1; i >= 0; --i)
    {
        digits_[i] = INIT_VAL;
    }

    negative_ = false;
    length_ = 1;
}

// Sets radix_ to rad
void Bigint::set_radix(int rad)
{
    radix_ = rad;
}

// Returns radix_
int Bigint::get_radix()
{
    return radix_;
}

// Sets digit_ at location index to digit
void Bigint::set_digit(int digit, int index)
{
    digits_[index] = digit;
}

// Returns digit_ at index
int Bigint::get_digit(int index)
{
    return digits_[index];
}

// ===== private methods ===== //

// Returns true if bigint is equal to zero
bool Bigint::is_zero()
{
    for (int i = 0; i < DIGITS; i++)
    {
        if (digits_[i] != 0)
        {
            return false;
        }
    }
    return true;
}

// Rounds all digits in bigint up if their contained digit is larger than radix
void Bigint::round_up()
{
    for (int i = DIGITS - 1; i > 0; i--) // i > 0 because rounding should not occur on result[0] to avoid out of bounds error
    {
        while (digits_[i] >= radix_)
        {
            digits_[i] -= radix_;
            digits_[i - 1] += 1;
        }
    }
}

// Counts all digits in a Bigint and updates the length variable
void Bigint::update_length()
{
    length_ = DIGITS;
    for (int i = 0; digits_[i] == 0 && i < DIGITS; i++)
    {
        length_--;
    }
}

// Returns an integer equal to bigint if bigint is small enough
int Bigint::return_int()
{
    int result = 0;
    uint temp = 0;

    for (int i = 0; i < 10; i++)
    {
        temp += digits_[DIGITS - i - 1] * pow(10, i);
    }

    if (temp > INT32_MAX)
    {
        exit(1);
    }

    result = temp;

    if (negative_)
    {
        return result * -1;
    }

    return result;
}

// Displays the raw contents of the Bigint array
void Bigint::raw_contents()
{
    bool leading_zero = true;

    std::cout << "{ ";

    for (int i = 0; i < DIGITS; i++)
    {
        if (leading_zero && (digits_[i] != 0))
        {
            leading_zero = false;
        }
        if (leading_zero)
        {
            continue;
        }

        if (i < DIGITS - 1)
        {
            std::cout << digits_[i] << " , ";
        }
        else
        {
            std::cout << digits_[i];
        }
    }

    std::cout << " }" << std::endl;
}
### bigint
  this project consists of a header and class file for an integer of arbitrary length (set as 256 digits by default but modifiable within Bigint.h). 
  
### operators

#### binary arithmetic
```
123456789 + 987654321   // results in 1111111110
987654321 - 123456789   // results in 864197532
123456789 * 987654321   // results in 121932631112635269
987654321 / 123456789   // results in 8
```

#### relational operators
```
123456789 < 987654321   // results in true
123456789 > 987654321   // results in false
123456789 == 987654321  // results in false
123456789 != 987654321  // results in true
123456789 <= 987654321  // results in true
123456789 >= 987654321  // results in false
```

#### bitwise operators
```
123456789 << 3          // results in 493827160
123456789 >> 3          // results in 30864197
```

#### other functions
```
Bigint();                                                     // creates a bigint initialised to 0.
Bigint(int n);                                                // creates a bigint initialised to n
std::ostream &operator<<(std::ostream &out, const Bigint &n); // sends the bigint to std out
std::istream &operator>>(std::istream &in, Bigint &n);        // reads a bigint to std in
void operator=(const Bigint &n1);                             // assigns a bigint to another bigint
void clear;                                                   // reinsantiates the bigint
void set_radix(int rad);                                      // sets the base of the bigint
int get_radix();
void set_digit(int digit, int index);
int get_digit(int index):
```

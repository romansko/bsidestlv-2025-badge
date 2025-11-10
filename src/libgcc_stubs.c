// Minimal libgcc stubs for rv32ec/ilp32e
// These are needed because standard libgcc has ISA mismatch with rv32ec

// Signed 32-bit multiplication
__attribute__((used)) int __mulsi3(int a, int b)
{
    unsigned int ua, ub;
    int sign = 1;
    unsigned int result = 0;
    
    // Handle sign and convert to unsigned
    if (a < 0) {
        ua = (unsigned int)(-a);
        sign = -sign;
    } else {
        ua = (unsigned int)a;
    }
    
    if (b < 0) {
        ub = (unsigned int)(-b);
        sign = -sign;
    } else {
        ub = (unsigned int)b;
    }
    
    // Multiply using shift and add (unsigned arithmetic)
    while (ub != 0) {
        if (ub & 1) {
            result += ua;
        }
        ua <<= 1;
        ub >>= 1;
    }
    
    // Apply sign
    if (sign < 0) {
        return -(int)result;
    } else {
        return (int)result;
    }
}

// Unsigned 32-bit division
__attribute__((used)) unsigned int __udivsi3(unsigned int a, unsigned int b)
{
    if (b == 0) {
        return 0; // Division by zero - return 0
    }
    
    unsigned int quotient = 0;
    unsigned int remainder = 0;
    
    for (int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((a >> i) & 1);
        if (remainder >= b) {
            remainder -= b;
            quotient |= (1U << i);
        }
    }
    
    return quotient;
}

// Unsigned 32-bit modulo
__attribute__((used)) unsigned int __umodsi3(unsigned int a, unsigned int b)
{
    if (b == 0) {
        return 0; // Division by zero - return 0
    }
    
    unsigned int remainder = 0;
    
    for (int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((a >> i) & 1);
        if (remainder >= b) {
            remainder -= b;
        }
    }
    
    return remainder;
}


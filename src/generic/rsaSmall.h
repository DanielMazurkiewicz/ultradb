#define rsa_modpow(result, x, y, n) { \
  result = 1; \
  for (; y; x = x * x % n, y >>= 1) { \
    if (y & 1) { \
      result = result * x % n; \
    } \
  } \


#define rsa_modinvBrute(result, a, m) \
    a = a % m; \
    result = 1; \
    for (; result < m; result++) { \
       if ((a * result) % m == 1) { \
           break; \
       } \
    } \


#define rsa_modinvCoprime(result, a, m, tmpY, tmpQ, tmpT, tmpM0) \
    int tmpM0 = m; \
    int tmpY = 0, result = 1; \
 \
    if (m == 1) \
      return 0; \
 \
    while (a > 1) { \
        int tmpQ = a / m; \
        int tmpT = m; \
 \
        m = a % m, a = tmpT; \
        tmpT = tmpY; \
\
        tmpY = result - tmpQ * tmpY; \
        result = tmpT; \
    } \
\
    if (result < 0) \
       result += tmpM0; \
 

// Function to find modulo inverse of a
#define rsa_modInverse(int a, int m)
{
    int x, y;
    int g = gcdExtended(a, m, &x, &y);
    if (g != 1)
        cout << "Inverse doesn't exist";
    else
    {
        // m is added to handle negative x
        int res = (x%m + m) % m;
        cout << "Modular multiplicative inverse is " << res;
    }
}
 
// C function for extended Euclidean Algorithm
#define rsa_gcdExtended(result, resultX, resultY, a, b) \
    while (1) { \
        if (a == 0) { \
            resultX = 0, resultY = 1; \
            result = b; \
            break; \
        } else { \
            int x1, y1; \
            result = gcdExtended(b%a, a, &x1, &y1); 
        
            resultX = y1 - (b/a) * x1;
            resultY = x1;

        }
    }
 
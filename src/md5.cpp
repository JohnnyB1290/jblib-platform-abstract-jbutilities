// Code by: B-Con (http://b-con.us) 
// Released under the GNU GPL 
// MD5 Hash Digest implementation (little endian byte order) 

// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "md5.hpp"
#include "string.h"


// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - c) ++b; a += c; 
#define ROTLEFT(a,b) ((a << b) | (a >> (32-b))) 

#define F(x,y,z) ((x & y) | (~x & z)) 
#define G(x,y,z) ((x & z) | (y & ~z)) 
#define H(x,y,z) (x ^ y ^ z) 
#define I(x,y,z) (y ^ (x | ~z)) 

#define FF(a,b,c,d,m,s,t) { a += F(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); }
#define GG(a,b,c,d,m,s,t) { a += G(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); }
#define HH(a,b,c,d,m,s,t) { a += H(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); } 
#define II(a,b,c,d,m,s,t) { a += I(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); } 


void MD5_CTX_t::Transform(uchar data[])
{  
   uint a,b,c,d,m[16],i,j; 
   
   // MD5 specifies big endian byte order, but this implementation assumes a little 
   // endian byte order CPU. Reverse all the bytes upon input, and re-reverse them 
   // on output (in md5_final()). 
   for (i=0,j=0; i < 16; ++i, j += 4) 
      m[i] = (data[j]) + (data[j+1] << 8) + (data[j+2] << 16) + (data[j+3] << 24); 
   
   a = this->state[0];
   b = this->state[1];
   c = this->state[2];
   d = this->state[3];
   
   FF(a,b,c,d,m[0],  7,0xd76aa478); 
   FF(d,a,b,c,m[1], 12,0xe8c7b756); 
   FF(c,d,a,b,m[2], 17,0x242070db); 
   FF(b,c,d,a,m[3], 22,0xc1bdceee); 
   FF(a,b,c,d,m[4],  7,0xf57c0faf); 
   FF(d,a,b,c,m[5], 12,0x4787c62a); 
   FF(c,d,a,b,m[6], 17,0xa8304613); 
   FF(b,c,d,a,m[7], 22,0xfd469501); 
   FF(a,b,c,d,m[8],  7,0x698098d8); 
   FF(d,a,b,c,m[9], 12,0x8b44f7af); 
   FF(c,d,a,b,m[10],17,0xffff5bb1); 
   FF(b,c,d,a,m[11],22,0x895cd7be); 
   FF(a,b,c,d,m[12], 7,0x6b901122);
   FF(d,a,b,c,m[13],12,0xfd987193); 
   FF(c,d,a,b,m[14],17,0xa679438e); 
   FF(b,c,d,a,m[15],22,0x49b40821); 
   
   GG(a,b,c,d,m[1],  5,0xf61e2562); 
   GG(d,a,b,c,m[6],  9,0xc040b340); 
   GG(c,d,a,b,m[11],14,0x265e5a51); 
   GG(b,c,d,a,m[0], 20,0xe9b6c7aa);
   GG(a,b,c,d,m[5],  5,0xd62f105d); 
   GG(d,a,b,c,m[10], 9,0x02441453); 
   GG(c,d,a,b,m[15],14,0xd8a1e681); 
   GG(b,c,d,a,m[4], 20,0xe7d3fbc8);
   GG(a,b,c,d,m[9],  5,0x21e1cde6); 
   GG(d,a,b,c,m[14], 9,0xc33707d6); 
   GG(c,d,a,b,m[3], 14,0xf4d50d87); 
   GG(b,c,d,a,m[8], 20,0x455a14ed);
   GG(a,b,c,d,m[13], 5,0xa9e3e905); 
   GG(d,a,b,c,m[2],  9,0xfcefa3f8); 
   GG(c,d,a,b,m[7], 14,0x676f02d9); 
   GG(b,c,d,a,m[12],20,0x8d2a4c8a);
   
   HH(a,b,c,d,m[5],  4,0xfffa3942); 
   HH(d,a,b,c,m[8], 11,0x8771f681); 
   HH(c,d,a,b,m[11],16,0x6d9d6122); 
   HH(b,c,d,a,m[14],23,0xfde5380c); 
   HH(a,b,c,d,m[1],  4,0xa4beea44); 
   HH(d,a,b,c,m[4], 11,0x4bdecfa9); 
   HH(c,d,a,b,m[7], 16,0xf6bb4b60); 
   HH(b,c,d,a,m[10],23,0xbebfbc70); 
   HH(a,b,c,d,m[13], 4,0x289b7ec6); 
   HH(d,a,b,c,m[0], 11,0xeaa127fa); 
   HH(c,d,a,b,m[3], 16,0xd4ef3085); 
   HH(b,c,d,a,m[6], 23,0x04881d05); 
   HH(a,b,c,d,m[9],  4,0xd9d4d039); 
   HH(d,a,b,c,m[12],11,0xe6db99e5); 
   HH(c,d,a,b,m[15],16,0x1fa27cf8); 
   HH(b,c,d,a,m[2], 23,0xc4ac5665); 
      
   II(a,b,c,d,m[0],  6,0xf4292244); 
   II(d,a,b,c,m[7], 10,0x432aff97); 
   II(c,d,a,b,m[14],15,0xab9423a7); 
   II(b,c,d,a,m[5], 21,0xfc93a039); 
   II(a,b,c,d,m[12], 6,0x655b59c3); 
   II(d,a,b,c,m[3], 10,0x8f0ccc92); 
   II(c,d,a,b,m[10],15,0xffeff47d); 
   II(b,c,d,a,m[1], 21,0x85845dd1); 
   II(a,b,c,d,m[8],  6,0x6fa87e4f); 
   II(d,a,b,c,m[15],10,0xfe2ce6e0); 
   II(c,d,a,b,m[6], 15,0xa3014314); 
   II(b,c,d,a,m[13],21,0x4e0811a1); 
   II(a,b,c,d,m[4],  6,0xf7537e82); 
   II(d,a,b,c,m[11],10,0xbd3af235); 
   II(c,d,a,b,m[2], 15,0x2ad7d2bb); 
   II(b,c,d,a,m[9], 21,0xeb86d391); 
   
   this->state[0] += a;
   this->state[1] += b;
   this->state[2] += c;
   this->state[3] += d;
}  

MD5_CTX_t::MD5_CTX_t(void)
{  
	this->Reset();
}  

void MD5_CTX_t::Reset(void)
{
	memset(this->data,0,64);
	this->datalen = 0;
	this->bitlen[0] = 0;
	this->bitlen[1] = 0;
	this->state[0] = 0x67452301;
	this->state[1] = 0xEFCDAB89;
	this->state[2] = 0x98BADCFE;
	this->state[3] = 0x10325476;
}

void MD5_CTX_t::Update(uchar data[], uint len)
{  
   uint i;
   
   for (i=0; i < len; ++i) { 
      this->data[this->datalen] = data[i];
      this->datalen++;
      if (this->datalen == 64) {
    	 this->Transform(this->data);
         DBL_INT_ADD(this->bitlen[0],this->bitlen[1],512);
         this->datalen = 0;
      }  
   }  
}  

void MD5_CTX_t::Final(uchar hash[])
{  
   uint i;
   
   i = this->datalen;
   
   // Pad whatever data is left in the buffer. 
   if (this->datalen < 56) {
      this->data[i++] = 0x80;
      while (i < 56) 
         this->data[i++] = 0x00;
   }  
   else if (this->datalen >= 56) {
      this->data[i++] = 0x80;
      while (i < 64) 
         this->data[i++] = 0x00;
      this->Transform(this->data);
      memset(this->data,0,56);
   }  
   
   // Append to the padding the total message's length in bits and transform. 
   DBL_INT_ADD(this->bitlen[0],this->bitlen[1],8 * this->datalen);
   this->data[56] = this->bitlen[0];
   this->data[57] = this->bitlen[0] >> 8;
   this->data[58] = this->bitlen[0] >> 16;
   this->data[59] = this->bitlen[0] >> 24;
   this->data[60] = this->bitlen[1];
   this->data[61] = this->bitlen[1] >> 8;
   this->data[62] = this->bitlen[1] >> 16;
   this->data[63] = this->bitlen[1] >> 24;
   this->Transform(this->data);
   
   // Since this implementation uses little endian byte ordering and MD uses big endian, 
   // reverse all the bytes when copying the final state to the output hash. 
   for (i=0; i < 4; ++i) { 
      hash[i]    = (this->state[0] >> (i*8)) & 0x000000ff;
      hash[i+4]  = (this->state[1] >> (i*8)) & 0x000000ff;
      hash[i+8]  = (this->state[2] >> (i*8)) & 0x000000ff;
      hash[i+12] = (this->state[3] >> (i*8)) & 0x000000ff;
   }  
}   

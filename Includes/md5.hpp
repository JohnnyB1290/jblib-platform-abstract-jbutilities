/*
 * md5.hpp
 *
 *  Created on: 07.11.2017
 *      Author: Stalker1290
 */

#ifndef MD5_HPP_
#define MD5_HPP_

// Bah, signed variables are for wimps 
#define uchar unsigned char 
#define uint unsigned int 

class MD5_CTX_t
{
public:
	MD5_CTX_t(void);
	void Reset(void);
	void Update(uchar data[], uint len);
	void Final(uchar hash[]);
private:
	void Transform(uchar data[]);
	uchar data[64];
	uint datalen;
	uint bitlen[2];
	uint state[4];
};



#endif /* MD5_HPP_ */

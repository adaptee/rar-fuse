#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "filename.h"

/***************************************************************************
 *  origianl code  *

 void EncodeFileName::Decode(char *Name,
 byte *EncName, size_t EncSize,
 wchar *NameW, size_t MaxDecSize)
 {
    .......
 }

 ***************************************************************************/

void convert_weird_to_unicode(  char *natives,
                                byte *weirds, size_t w_size,
                                wchar_t *unicodes, size_t u_size)
{
    byte flags = 0;
    int flagbits = 0;

    size_t w_pos=0;
    size_t u_pos=0;
    byte first_byte = weirds[w_pos++];

    while (w_pos < w_size && u_pos < u_size)
    {
        if (flagbits == 0)
        {
            flags = weirds[w_pos++];
            flagbits = 8;
        }
        switch(flags>>6)
        {
        case 0:
            unicodes[u_pos++] = weirds[w_pos++];
            break;
        case 1:
            unicodes[u_pos++] = weirds[w_pos++] + (first_byte<<8);
            break;
        case 2:
            unicodes[u_pos++] = weirds[w_pos] + (weirds[w_pos+1]<<8);
            w_pos += 2;
            break;
        case 3:
            {
                int length = weirds[w_pos++];
                if (length & 0x80)
                {
                    byte correction = weirds[w_pos++];
                    for (length=(length&0x7f)+2; length > 0 && u_pos < u_size; length--, u_pos++)
                        unicodes[u_pos] = ((natives[u_pos] + correction) & 0xff) + (first_byte<<8);
                }
                else
                {
                    for (length += 2; length > 0 && u_pos < u_size; length--, u_pos++)
                        unicodes[u_pos] = natives[u_pos];
                }
            }
            break;
        }
        flags <<= 2;
        flagbits -= 2;
    }

    unsigned int end = (u_pos < u_size) ? u_pos : (u_size-1) ;
    unicodes[end] = 0;
}


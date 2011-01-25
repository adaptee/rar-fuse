#ifndef DECODE_FILENAME_GUARD
#define DECODE_FILENAME_GUARD

#include "defs.h"

void convert_weird_to_unicode(  const char *natives,
                                const byte *weirds,
				size_t w_size,
                                wchar_t *unicodes,
				size_t u_size);

#endif /* end of include guard: DECODE-FILENAME_GUARD */



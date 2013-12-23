
#include <UnitTest++/UnitTest++.h>

#include <bitvector.h>

#include "util.h"


#define BLOCKSIZE 8
#include "opchain.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 16
#include "opchain.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 32
#include "opchain.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 64
#include "opchain.inc.h"
#undef BLOCKSIZE


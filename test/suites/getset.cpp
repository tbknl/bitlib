
#include <UnitTest++/UnitTest++.h>

#include <bitvector.h>

#include "util.h"


#define BLOCKSIZE 8
#include "getset.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 16
#include "getset.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 32
#include "getset.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 64
#include "getset.inc.h"
#undef BLOCKSIZE



#include <UnitTest++/UnitTest++.h>
#include <typeinfo>

#include <bitvector.h>

#include "util.h"


#define BLOCKSIZE 8
#include "operations.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 16
#include "operations.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 32
#include "operations.inc.h"
#undef BLOCKSIZE
#define BLOCKSIZE 64
#include "operations.inc.h"
#undef BLOCKSIZE


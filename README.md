# Requirements
[nlohmann json](https://github.com/nlohmann/json)  
[boost](https://www.boost.org/)

# Example
```
#include "cpglib/print.h"
#define debug_inline(x...) _debug_print(0, #x, x);
#define debug_block(x...) _debug_print(1, #x, x);
#define debug_json(x...) _debug_print(2, #x, x);
```
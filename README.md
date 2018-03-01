# QDBM Library for Mongoose OS https://mongoose-os.com/

This library provides support for the embedded database [QDBM](http://fallabs.com/qdbm/index.html) for use on the esp8266 and esp32 platforms with some limitations.

## Language support
Only the 'C' files are compiled at this time.

## File System limitations

The library depends on the `lstat` and `ftruncate` functions, which are not implemented on Mongoose OS. There is no support in the virtual file system for symbolic links or truncating files, therefore the following behavior is implemented:

* `lstat` invokes the `stat` function.
* `ftruncate` only truncates the file to 0 bytes. Any other length is not supported. All QDBM functions pass 0, so this should work.

These functions are implemented under the [support](src/support) folder.

Additionally, locking a file is not supported, so ensure `DP_ONOLCK` mode is set when opening a database.

## Example

```
#include "mgos.h"
#include <depot.h>
#include <stdlib.h>
#include <stdio.h>

#define NAME     "mikio"
#define NUMBER   "000-1234-5678"
#define DBNAME   "book"

enum mgos_app_init_result mgos_app_init(void) {
    DEPOT *depot;
    char *val;

    /* open the database */
    if(!(depot = dpopen(DBNAME, DP_OWRITER | DP_OCREAT | DP_ONOLCK, -1))){
        fprintf(stderr, "dpopen: %s\n", dperrmsg(dpecode));
        return;
    }

    /* store the record */
    if(!dpput(depot, NAME, -1, NUMBER, -1, DP_DOVER)){
        fprintf(stderr, "dpput: %s\n", dperrmsg(dpecode));
    }

    /* retrieve the record */
    if(!(val = dpget(depot, NAME, -1, 0, -1, NULL))){
        fprintf(stderr, "dpget: %s\n", dperrmsg(dpecode));
    } else {
        printf("Name: %s\n", NAME);
        printf("Number: %s\n", val);
        free(val);
    }

    /* close the database */
    if(!dpclose(depot)){
        fprintf(stderr, "dpclose: %s\n", dperrmsg(dpecode));
        return;
    }

    return MGOS_APP_INIT_SUCCESS;
}
```



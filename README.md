# QDBM Library for Mongoose OS https://mongoose-os.com/

This library provides support for the embedded database [QDBM](http://fallabs.com/qdbm/index.html) for use on the esp8266 and esp32 platforms with some limitations.

The library depends on the `lstat` and `ftruncate` functions, which are not supported on Mongoose OS. The SPIFFS file system does not support symbolic links or truncating files. To provide compatibility, these functions are implemented with the following behavior:

* `lstat` invokes the `stat` function.
* `ftruncate` will return an error as it is not supported. This means the `DP_OTRUNC` mode is not supported when opening a database for writing. 

These functions are implemented under the [support](src/support) folder.

Additionally, locking a file is not supported by SPIFFS, so ensure `DP_ONOLCK` mode is set when opening a databse.

Example:

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



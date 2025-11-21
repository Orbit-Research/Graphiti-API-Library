#include <windows.h>
#include <stdio.h>
#include "capi.h"

int main() {
    GraphitiHandle* graphiti = graphiti_create();

    if(!graphiti) {
        printf("Failed to connect\n");
        return 1;
    }
    
    // if(!graphiti_startUpVCP(graphiti, "COM4", false, false)) {
    //     printf("Failed to connect\n");
    //     return 1;
    // }

    if(!graphiti_startUpHID(graphiti, 0x1FC9, 0x8217, false, false)) {
        printf("Failed to connect\n");
        return 1;
    }

    graphiti_sleep(graphiti,2);

    graphiti_getSoftwareVersion(graphiti);

    graphiti_sleep(graphiti,2);

    char software_version[100];
    graphiti_getNextOutputEvent(graphiti, software_version, 100);
    if(software_version[0] == '\0') {
        printf("No output");
    } else {
        printf(software_version);
    }

    printf("\n");

    graphiti_sleep(graphiti,2);

    graphiti_updateSinglePixel(graphiti, 10, 20, 2, 0);

    graphiti_sleep(graphiti,2);

    graphiti_updateSinglePixel(graphiti, 10, 20, 0, 0);

    graphiti_sleep(graphiti,2);

    // graphiti_shutDownVCP(graphiti, false, false);
    graphiti_shutDownHID(graphiti, false, false);

    graphiti_sleep(graphiti,2);

    graphiti_destroy(graphiti);

    return 0;
}
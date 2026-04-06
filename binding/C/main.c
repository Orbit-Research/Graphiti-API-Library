/**
  ******************************************************************************
  * @file    main.c
  * @author  Orbit Research 
  * @version v1.0
  * @date    08-12-2025
  * @brief   Example C program demonstrating Graphiti C wrapper usage.
  *
  * @details
  * Simple CLI example showing initialization of the Graphiti C API, connecting
  * to a device, querying basic information, and performing a controlled shutdown.
  *
  ******************************************************************************
  * @attention
  * Copyright (c) 2026 Orbit Research LLC
  * Distributed under GNU General Public License V3.0
  ******************************************************************************
  */

#include <windows.h>
#include <stdio.h>
#include "capi.h"
/**
 * @function main
 * @brief Entry point for the Graphiti C API example application
 * @return int - application exit status
 */
int main() {
    setvbuf(stdout, NULL, _IONBF, 0); // unbuffered stdout for immediate logs
    printf("Starting Graphiti C example...\n");

    /**
     * @function graphiti_create
     * @brief Creates and initializes a Graphiti device handle
     * @return GraphitiHandle* - valid handle on success, NULL on failure
     */
    printf("Calling graphiti_create()...\n");
    GraphitiHandle* graphiti = graphiti_create();
    printf("graphiti_create() returned %p\n", (void*)graphiti);

    if(!graphiti) {
        printf("graphiti_create failed\n");
        return 1;
    }

    
    // if(!graphiti_startUpVCP(graphiti, "COM4", false, false)) {
    //     printf("Failed to connect\n");
    //     return 1;
    // }
    
    /**
     * @function graphiti_startUpHID
     * @brief Starts communication with the Graphiti device over HID
     * Supports both old and new device VID/PID.
     */
    int useOldDevice = 1; // Set 1 for Marble device, 0 for Marble Plus device
    unsigned short vid, pid;
    int interfaceNum = 1;
    if (useOldDevice) {
        vid = 0x1FC9; pid = 0x8217;
        interfaceNum = 0;
    } else {
        vid = 0x0483; pid = 0xA366;
        interfaceNum = 1;
    }

    // If your graphiti_startUpHID supports interface number, use it:
    // if(!graphiti_startUpHID(graphiti, vid, pid, interfaceNum, false, false)) {
    //     printf("Failed to connect\n");
    //     return 1;
    // }

    // Use the 5-argument version:
    printf("Calling graphiti_startUpHID(0x%04X,0x%04X,false,false)...\n", vid, pid);
    int success = graphiti_startUpHID(graphiti, vid, pid, false, false);
    printf("graphiti_startUpHID returned %d\n", success);

    if(!success){
        printf("Failed to connect to HID device.\n");
        graphiti_destroy(graphiti);
        return 1;
    }

    /**
     * @function graphiti_sleep
     * @brief Delays execution to allow device processing
     * @param handle Graphiti device handle
     * @param seconds Delay duration in seconds
     */
    graphiti_sleep(graphiti,10);

    /**
     * @function graphiti_getSoftwareVersion
     * @brief Requests the software version from the device
     * @param handle Graphiti device handle
     */
    graphiti_getSoftwareVersion(graphiti);

    graphiti_sleep(graphiti,10);

    char software_version[100];
    graphiti_getNextOutputEvent(graphiti, software_version, 100);
    int passed = 0;
    if(software_version[0] != '\0' && (software_version[0] == 'V' || (software_version[0] == 'M' && software_version[1] == 'B'))) {
        passed = 1;
    }
    printf("get_Software_Version_Correctness: %s\n", passed ? "passed" : "failed");

    graphiti_sleep(graphiti,10);

    /**
     * @function graphiti_updateSinglePixel
     * @brief Raises a single tactile pixel to a specified height
     * @param handle Graphiti device handle
     * @param row Pixel row index
     * @param column Pixel column index
     * @param height Pixel height value
     * @param blinkRate Pixel blink rate
     */
    graphiti_updateSinglePixel(graphiti, 10, 20, 2, 0);

    graphiti_sleep(graphiti,10);

     /**
     * @function graphiti_updateSinglePixel
     * @brief Resets a single tactile pixel to zero height
     * @param handle Graphiti device handle
     * @param row Pixel row index
     * @param column Pixel column index
     * @param height Pixel height value
     * @param blinkRate Pixel blink rate
     */
    graphiti_updateSinglePixel(graphiti, 10, 20, 0, 0);

    graphiti_sleep(graphiti,10);
    
    /**
     * @function graphiti_shutDownHID
     * @brief Closes the HID connection with the device
     * @param handle Graphiti device handle
     * @param force Force shutdown flag
     * @param wait Wait-for-completion flag
     */
    graphiti_shutDownHID(graphiti, false, false);

    graphiti_sleep(graphiti,10);

    /**
     * @function graphiti_destroy
     * @brief Releases all resources associated with the Graphiti handle
     * @param handle Graphiti device handle
     */
    graphiti_destroy(graphiti);

    return 0;
}
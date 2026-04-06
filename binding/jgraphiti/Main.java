/**
 ******************************************************************************
 * @file    Main.java
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Example Java runner for Graphiti JNI bindings.
 *
 * @details
 * Small example that shows how to instantiate the `Graphiti` Java wrapper
 * and call into the native library via JNI for basic startup and queries.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 ******************************************************************************/
package binding.jgraphiti;

import java.util.Set;

public class Main {

    /**
     * @function waitForResponse
     * @brief Wait for and log a string-based response from the Graphiti device
     * @param graphiti : Graphiti - The Graphiti instance for retrieving responses
     * @param handle : long - Device handle identifying the connected device
     * @param testName : String - Name of the test to display in success/failure message
     * @return void
     */
    private static void waitForResponse(Graphiti graphiti, long handle, String testName) {
        String output = null;

        for (int i = 0; i < 10; i++) {
            output = graphiti.getNextOutputEvent(handle);

            if (output != null && !output.isEmpty() && !output.equals("NA")) {
                System.out.println(testName + " SUCCESS: " + output);
                return;
            }

            graphiti.sleep(handle, 1);
        }

        System.out.println(testName + " FAILED (No response)");
    }

    /**
     * @function main
     * @brief Main entry point for Graphiti JNI test runner
     * @details Demonstrates complete workflow: HID device connection, initialization,
     *          display operations, query tests, pixel status checks, key input handling,
     *          touch event detection, and graceful shutdown.
     * @param args : String[] - Command-line arguments (currently unused)
     * @return void
     */
    public static void main(String[] args) {

        Graphiti graphiti = new Graphiti();
        long handle = graphiti.getHandle();

        boolean connected = false;

        //  CONNECT 
        if (graphiti.startUpHID(handle, 0x0483, 0xA366, true, true)) {
            connected = true;
            System.out.println("Connected to NEW device.");
        } else if (graphiti.startUpHID(handle, 0x1FC9, 0x8217, true, true)) {
            connected = true;
            System.out.println("Connected to OLD device.");
        }

        if (!connected) {
            throw new RuntimeException("Device not connected");
        }

        // INIT
        graphiti.startResponseThread(handle);
        graphiti.setKeyEvent(handle, true);
        graphiti.setTouchEvent(handle, true);

        graphiti.sleep(handle, 2);

        System.out.println("\n START TESTS\n");

        // DISPLAY TEST
        byte[] displayData = new byte[2400];

        for (int i = 0; i < displayData.length; i++) {
            displayData[i] = 4; // raise all pins
        }

        graphiti.updateDisplay(handle, displayData);
        System.out.println("update_Display EXECUTED");
        graphiti.sleep(handle, 5);

        graphiti.setDisplay(handle);
        System.out.println("set_Display EXECUTED");
        graphiti.sleep(handle, 5);

        graphiti.clearDisplay(handle);
        System.out.println("clear_Display EXECUTED");
        graphiti.sleep(handle, 5);

        graphiti.setCursor(handle, 5, 5, 3, 5, 10);
        System.out.println("set_Cursor EXECUTED");
        graphiti.sleep(handle, 5);

        //QUERY TESTS
        graphiti.getSoftwareVersion(handle);
        waitForResponse(graphiti, handle, "Software_Version");

        graphiti.getHardwareVersion(handle);
        waitForResponse(graphiti, handle, "Hardware_Version");

        graphiti.getSerialNumber(handle);
        waitForResponse(graphiti, handle, "Serial_Number");

        graphiti.getBatteryStatus(handle);
        waitForResponse(graphiti, handle, "Battery_Status");

        graphiti.getDateAndTime(handle);
        waitForResponse(graphiti, handle, "Date_Time");

        graphiti.getDeviceName(handle);
        waitForResponse(graphiti, handle, "Device_Name");

        graphiti.getResolutionInformation(handle);
        waitForResponse(graphiti, handle, "Resolution");

        // PIXEL STATUS TEST (FIXED) 

        // All pixels
        graphiti.getAllPixelsPositionStatus(handle);
        graphiti.sleep(handle, 3);

        int[] allPixels = graphiti.getDisplayStatusEvent(handle);

        if (allPixels != null && allPixels.length > 0) {
            System.out.println("All_Pixels_Status SUCCESS: " + allPixels.length + " values");
        } else {
            System.out.println("All_Pixels_Status FAILED");
        }

        // Single pixel
        graphiti.getSinglePixelPositionStatus(handle, 1, 1);
        graphiti.sleep(handle, 1);

        int[] singlePixel = graphiti.getDisplayStatusEvent(handle);

        if (singlePixel != null && singlePixel.length > 0) {
            System.out.println("Single_Pixel_Status SUCCESS: " + singlePixel[0]);
        } else {
            System.out.println("Single_Pixel_Status FAILED");
        }

        //  KEY EVENT TEST 
        System.out.println("\nPress any key on device...\n");

        for (int i = 0; i < 15; i++) {
            Set<String> keys = graphiti.getNextKeyEvent(handle);

            if (keys != null && !keys.isEmpty()) {
                System.out.println("Key Pressed: " + keys);
                break;
            }

            graphiti.sleep(handle, 1);
        }

        //  TOUCH TEST 
        System.out.println("\nTouch device surface...\n");

        for (int i = 0; i < 10; i++) {
            String touch = graphiti.getNextGestureEvent(handle);

            if (touch != null && !touch.isEmpty()) {
                System.out.println("Touch Event: " + touch);
                break;
            }

            graphiti.sleep(handle, 1);
        }

        System.out.println("\nALL TESTS COMPLETED \n");

        //  CLEANUP 
        graphiti.sleep(handle, 2);

        graphiti.stopResponseThread(handle);
        graphiti.shutDownHID(handle, false, false);
        graphiti.close();

        System.out.println("Connection closed.");
    }
}


//     String port = "COM11";  
//     Graphiti graphiti = new Graphiti();
//     long handle = graphiti.getHandle();

//     System.out.println("Connecting to Graphiti over VCP on " + port + "...");

//     boolean vcpConnected = graphiti.startUpVCP(handle, port, false, false);
//     if (!vcpConnected) {
//         System.out.println("VCP connection failed!");
//         return;
//     }

//     System.out.println("VCP Connected successfully!");

    
//     graphiti.sleep(handle, 5);  

//     System.out.println("Requesting Software Version...");
//     graphiti.getSoftwareVersion(handle);

    
//     String output = null;
//     for (int i = 0; i < 20; i++) {  
//         output = graphiti.getNextOutputEvent(handle);
//         if (output != null && !output.isEmpty() && !output.equals("NA")) {
//             System.out.println("Received valid output: " + output);
//             break;
//         }
//         System.out.println("No output yet... waiting (try " + (i+1) + "/20)");
//         graphiti.sleep(handle, 1);
//     }

//     if (output == null || output.equals("NA")) {
//         System.out.println("No valid output received after all tries.");
//     }

    
//     graphiti.shutDownVCP(handle, false, false);
//     graphiti.close();

//     System.out.println("Connection closed.");
// }
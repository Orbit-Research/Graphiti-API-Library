/**
  ******************************************************************************
  * @file    Main.java
  * @company Orbit Research
  * @version v1.0
  * @date    08-12-25
  * @brief   Example Java application demonstrating Graphiti VCP communication.
  *
  * @details
  * Creates a Graphiti instance, opens a Virtual COM Port (VCP) connection,
  * retrieves software version, reads next output event, and gracefully shuts
  * down the VCP connection.
  *
  ******************************************************************************
  * @attention
  * Copyright (c) 2026 Orbit Research LLC
  * Distributed under GNU General Public License V3.0
  ******************************************************************************
  */

package binding.Java_vcp;
import binding.jgraphiti.Graphiti;

public class Main {
    /**
     * @function main
     * @brief Executes the Graphiti Java binding example application
     * @param args Command-line arguments
     */
    public static void main(String[] args) {

        /**
         * @function Graphiti
         * @brief Creates a new Graphiti Java API instance
         * @return Graphiti object instance
         */

        Graphiti graphiti = new Graphiti();

        /**
         * @function getHandle
         * @brief Retrieves the native Graphiti device handle
         * @return long - native device handle
         */
        long handle = graphiti.getHandle();

        System.out.println("Correct folder");

        /**
         * @function startUpVCP
         * @brief Starts communication with the device using Virtual COM Port
         * @param handle Graphiti device handle
         * @param port COM port identifier
         * @param enableKeyEvent Enable or disable key events
         * @param enableTouchEvent Enable or disable touch events
         * @return boolean - true on successful connection
         */
        graphiti.startUpVCP(handle, "COM12", false, false);

        /**
         * @function sleep
         * @brief Pauses execution to allow device processing
         * @param handle Graphiti device handle
         * @param seconds Sleep duration in seconds
         */
        graphiti.sleep(handle, 2);

        /**
         * @function getSoftwareVersion
         * @brief Requests the software version from the connected device
         * @param handle Graphiti device handle
         */  
        graphiti.getSoftwareVersion(handle);

        graphiti.sleep(handle, 2);

        /**
         * @function getNextOutputEvent
         * @brief Reads the next output event received from the device
         * @param handle Graphiti device handle
         * @return String - device output event
         */
        System.err.println(graphiti.getNextOutputEvent(handle));

        graphiti.sleep(handle, 2);
         /**
         * @function shutDownVCP
         * @brief Safely shuts down the Virtual COM Port connection
         * @param handle Graphiti device handle
         * @param force Force shutdown flag
         * @param wait Wait for completion flag
         */

        graphiti.shutDownVCP(handle, false, false);

        /**
         * @function close
         * @brief Releases all Graphiti Java resources
         */
        graphiti.close();
    }
}

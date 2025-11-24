package binding.jgraphiti;

import java.util.Set;

public class Graphiti {
    static { System.loadLibrary("libGraphitiJNI"); }
    private long nativeHandle;

    public long getHandle() {
        return nativeHandle;
    }

    public static class PinInfo {
        public int rowID;
        public int columnID;
        public int height;
        public int blinkRate;
    }

    public static class DrawEvent {
        public int length;
        public PinInfo[] pins;
    }

    public Graphiti() { nativeHandle = graphiti_create(); }
    public void close() { graphiti_destroy(nativeHandle); }

    public native long graphiti_create();
    public native void graphiti_destroy(long handle);

    public native boolean startUpVCP(long handle, String port, boolean keyEvents, boolean touchEvents);
    public native void shutDownVCP(long handle, boolean key, boolean touch);

    public native boolean startUpHID(long handle, int vendor_id, int product_id, boolean keyEvents, boolean touchEvents);
    public native void shutDownHID(long handle, boolean key, boolean touch);

    public native int index(long handle, int row, int col);
    public native void setPin(long handle, int row, int col, int height);
    public native void clearScreen(long handle);
    public native void sleep(long handle, int time);

    //Event Handlers

    public native String getNextOutputEvent(long handle);
    public native String getNextGestureEvent(long handle);
    public native int[] getDisplayStatusEvent(long handle);
    public native Set<String> getNextKeyEvent(long handle);
    public native DrawEvent getNextDrawEvent(long handle); 

    public native void startResponseThread(long handle);
    public native void stopResponseThread(long handle);

    public native void sendACK(long handle);
    public native void sendNACK(long handle);
    public native void getSoftwareVersion(long handle);
    public native void getHardwareVersion(long handle);
    public native void getSerialNumber(long handle);
    public native void getBatteryStatus(long handle);
    public native void getResolutionInformation(long handle);
    public native void getDeviceOrientation(long handle);
    public native void getHeightInformation(long handle);
    public native void getDeviceName(long handle);

    public native void updateDisplay(long handle, byte[] screen);
    public native void setDisplay(long handle);
    public native void clearDisplay(long handle);

    public native void updateSinglePixel(long handle, int row, int column, int height, int blinkRate);
    public native void updateSingleRow(long handle, int row, byte[] rowData);
    public native void updateSingleColumn(long handle, int column, byte[] columnData);

    public native void getAllPixelsPositionStatus(long handle);
    public native void getSinglePixelPositionStatus(long handle, int row, int column);
    public native void getSingleRowPixelPositionStatus(long handle, int row);
    public native void getSingleColumnPixelPositionStatus(long handle, int column);

    public native void showMessage(long handle, String message, byte[] selectFlags, int blinkRate);
    public native void setCursor(long handle, int row, int column, int height, int length, int blinkRate);

    public native void sendImageInterruptible(long handle, String name, String filepath);
    public native void sendImageBlocking(long handle, String name, String filepath);

    public native void setKeyEvent(long handle, boolean enabled);
    public native void setTouchEvent(long handle, boolean enabled);
    public native void getLastTouchPointStatus(long handle);

    public native void vibratorControlCommand(long handle, int frequencyRange, int dutyCycle, int duration);

    public native void getDateAndTime(long handle);
    public native void setDateAndTime(long handle, int day, int month, int year, int hour, int minute, int second);
}


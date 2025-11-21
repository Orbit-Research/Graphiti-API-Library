package binding.jgraphiti;

public class Main {
    public static void main(String[] args) {
        Graphiti graphiti = new Graphiti();

        long handle = graphiti.getHandle();
        
        //graphiti.startUpVCP(handle, "COM4", false, false);
        graphiti.startUpHID(handle,0x1FC9, 0x8217, false, false);

        graphiti.sleep(handle, 2);

        graphiti.getSoftwareVersion(handle);

        graphiti.sleep(handle, 2);

        System.err.println(graphiti.getNextOutputEvent(handle));

        graphiti.sleep(handle, 2);

        //graphiti.shutDownVCP(handle, false, false);
        graphiti.shutDownHID(handle, false, false);

        graphiti.close();
    }
}

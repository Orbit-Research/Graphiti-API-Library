package binding.examplej;
import binding.jgraphiti.Graphiti;

public class Main {
    public static void main(String[] args) {
        Graphiti graphiti = new Graphiti();

        long handle = graphiti.getHandle();

        System.out.println("Correct folder");
        
        graphiti.startUpVCP(handle, "COM4", false, false);

        graphiti.sleep(handle, 2);

        graphiti.getSoftwareVersion(handle);

        graphiti.sleep(handle, 2);

        System.err.println(graphiti.getNextOutputEvent(handle));

        graphiti.sleep(handle, 2);

        graphiti.shutDownVCP(handle, false, false);

        graphiti.close();
    }
}

package toposortparser;

import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author J. Baum
 */
public class Thread {

    private int id;
    private int processedNodes;
    private Map<String,Double> timings;
    
    public Thread() {
        timings = new HashMap<>();
    }

    public int getId() {
        return id;
    }

    public int getProcessedNodes() {
        return processedNodes;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setProcessedNodes(int processedNodes) {
        this.processedNodes = processedNodes;
    }
    
     public void addTiming(String name, double value) {
        this.timings.put(name, value);
    }
    
    public void removeTiming(String name) {
        this.timings.remove(name);
    }
    
    public Map<String,Double> getTimings() {
        Map<String, Double> outputTimings = new HashMap<>();
        for (String key : this.timings.keySet()) {
            outputTimings.put(key, this.timings.get(key));
        }
        
        return outputTimings;
    }
}

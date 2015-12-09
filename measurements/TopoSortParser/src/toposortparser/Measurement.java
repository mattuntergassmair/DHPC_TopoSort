package toposortparser;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author J. Baum
 */
public class Measurement {

    private long date;
    private int numberOfThreads;
    private int processors;
    private String hostname;

    private double totalTime;
    private String algorithm;
    private List<Thread> threads;
    private Graph graph;
    private boolean optimistic;
    private boolean enableAnalysis;
    private boolean verbose;
    private boolean debug;
    private int errorCode;

    public Measurement() {

        threads = new ArrayList<>();
    }

    public long getDate() {
        return date;
    }

    public void setDate(long date) {
        this.date = date;
    }

    public int getNumberOfThreads() {
        return numberOfThreads;
    }

    public void setNumberOfThreads(int numberOfThreads) {
        this.numberOfThreads = numberOfThreads;
    }

    public int getProcessors() {
        return processors;
    }

    public void setProcessors(int processors) {
        this.processors = processors;
    }

    public double getTotalTime() {
        return totalTime;
    }

    public void setTotalTime(double totalTime) {
        this.totalTime = totalTime;
    }

    public String getAlgorithm() {
        return algorithm;
    }

    public void setAlgorithm(String algorithm) {
        this.algorithm = algorithm;
    }

    public boolean isOptimistic() {
        return optimistic;
    }

    public void setOptimistic(boolean optimistic) {
        this.optimistic = optimistic;
    }

    public boolean isEnableAnalysis() {
        return enableAnalysis;
    }

    public void setEnableAnalysis(boolean enableAnalysis) {
        this.enableAnalysis = enableAnalysis;
    }

    public List<Thread> getThreads() {
        return threads;
    }

    public void addThread(Thread thread) {
        this.threads.add(thread);
    }

    public Graph getGraph() {
        return graph;
    }

    public void setGraph(Graph graph) {
        this.graph = graph;
    }

    public boolean isVerbose() {
        return verbose;
    }

    public void setVerbose(boolean verbose) {
        this.verbose = verbose;
    }

    public boolean isDebug() {
        return debug;
    }

    public void setDebug(boolean debug) {
        this.debug = debug;
    }

    public String getHostname() {
        return hostname;
    }

    public void setHostname(String hostname) {
        this.hostname = hostname;
    }

    public int getErrorCode() {
        return errorCode;
    }

    public void setErrorCode(int errorCode) {
        this.errorCode = errorCode;
    }
    
    

    @Override
    public String toString() {
        String output = "";

        output += "date: " + this.date + "\n";
        output += "numberOfThreads: " + this.numberOfThreads + "\n";
        output += "processors: " + this.processors + "\n";
        

        output += "threads: \n";
        for (Thread temp : this.threads) {
            output += "\t thread\n";
            output += "\t\t id: " + temp.getId() + "\n";
            output += "\t\t processedNodes: " + temp.getProcessedNodes() + "\n";

            output += "\t\t timings: \n";
            for (String key : temp.getTimings().keySet()) {
                output += "\t\t\t " + key + ": " + temp.getTimings().get(key) + "\n";
            }
        }

        output += "graph: \n";
        output += "\t type: " + this.graph.getType() + "\n";
        output += "\t numberOfNodes: " + this.graph.getNumberOfNodes() + "\n";
        output += "\t numberOfEdges: " + this.graph.getNumberOfEdges() + "\n";

        output += "optimistic: " + this.optimistic + "\n";
        output += "enableAnalysis: " + this.enableAnalysis + "\n";
        output += "verbose: " + this.verbose + "\n";
        output += "debug: " + this.debug + "\n";

        return output;
    }

}

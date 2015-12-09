package toposortparser;

import java.math.BigInteger;

/**
 *
 * @author J. Baum
 */
public class Graph {

    private String type;
    private int numberOfNodes;
    private BigInteger numberOfEdges;
    private BigInteger depth;
    private double density;


    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public BigInteger getNumberOfEdges() {
        return numberOfEdges;
    }

    public void setNumberOfEdges(BigInteger numberOfEdges) {
        this.numberOfEdges = numberOfEdges;
    }

    public int getNumberOfNodes() {
        return numberOfNodes;
    }

    public void setNumberOfNodes(int numberOfNodes) {
        this.numberOfNodes = numberOfNodes;
    }

    public BigInteger getDepth() {
        return depth;
    }

    public void setDepth(BigInteger depth) {
        this.depth = depth;
    }

    public double getDensity() {
        return density;
    }

    public void setDensity(double density) {
        this.density = density;
    }
    
    

}

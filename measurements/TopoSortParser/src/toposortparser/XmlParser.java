package toposortparser;

import java.io.File;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 *
 * @author J. Baum
 */
public class XmlParser {

    public List<Measurement> parseXml(String filePath) {
        List<Measurement> measurementList = new ArrayList<>();
        try {

            File fXmlFile = new File(filePath);
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            Document doc = dBuilder.parse(fXmlFile);

            doc.getDocumentElement().normalize();

            NodeList measurements = doc.getElementsByTagName("measurement");

            for (int temp = 0; temp < measurements.getLength(); temp++) {

                Node nNode = measurements.item(temp);

                Measurement measurement = new Measurement();
                measurementList.add(measurement);
                for (int j = 0; j < nNode.getChildNodes().getLength(); j++) {

                    Node node = nNode.getChildNodes().item(j);

                    switch (node.getNodeName()) {
                        case "date":
                            measurement.setDate(Long.parseLong(node.getTextContent()));

                            break;
                        case "numberOfThreads":
                            measurement.setNumberOfThreads(Integer.parseInt(node.getTextContent()));
                            break;
                        case "processors":
                            measurement.setProcessors(Integer.parseInt(node.getTextContent()));
                            break;
                        case "totalTime":
                            measurement.setTotalTime(Double.parseDouble(node.getTextContent()));
                            break;
                        case "algorithm":
                            measurement.setAlgorithm(node.getTextContent());
                            break;
                        case "threads":
                            parseThreads(node, measurement);
                            break;
                        case "graph":
                            parseGraph(node, measurement);
                            break;
                        case "optimistic":
                            measurement.setOptimistic(Boolean.parseBoolean(node.getTextContent()));
                            break;
                        case "enableAnalysis":
                            measurement.setEnableAnalysis(Boolean.parseBoolean(node.getTextContent()));
                            break;
                        case "verbose":
                            measurement.setVerbose(Boolean.parseBoolean(node.getTextContent()));
                            break;
                        case "debug":
                            measurement.setDebug(Boolean.parseBoolean(node.getTextContent()));
                            break;
                    }

                }

                System.out.println(measurement);

            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        return measurementList;

    }

    private void parseTimings(Node node, Thread thread) {

        for (int i = 0; i < node.getChildNodes().getLength(); i++) {
            Node nNode = node.getChildNodes().item(i);
            if (nNode.getNodeName().equals("timing")) {
                thread.addTiming(
                        nNode.getAttributes().getNamedItem("name").getNodeValue(),
                        Double.parseDouble(nNode.getTextContent())
                );

            }
        }
    }

    private void parseThreads(Node node, Measurement measurement) {
        for (int i = 0; i < node.getChildNodes().getLength(); i++) {
            Node nNode = node.getChildNodes().item(i);
            if (nNode.getNodeName().equals("thread")) {

                Thread thread = new Thread();
                for (int j = 0; j < nNode.getChildNodes().getLength(); j++) {

                    Node threadChild = nNode.getChildNodes().item(j);

                    switch (threadChild.getNodeName()) {
                        case "id":
                            thread.setId(Integer.parseInt(threadChild.getTextContent()));
                            break;
                        case "processedNodes":
                            thread.setProcessedNodes(Integer.parseInt(threadChild.getTextContent()));
                            break;
                        case "timings":
                            parseTimings(threadChild, thread);
                            break;
                    }

                }

                measurement.addThread(thread);

            }
        }
    }

    private void parseGraph(Node node, Measurement measurement) {

        Graph graph = new Graph();
        
        for (int i = 0; i < node.getChildNodes().getLength(); i++) {
            
            Node nNode = node.getChildNodes().item(i);
            
            switch (nNode.getNodeName()) {
                case "type":
                    graph.setType(nNode.getTextContent());
                    break;
                case "numberOfNodes":
                    graph.setNumberOfNodes(Integer.parseInt(nNode.getTextContent()));
                    break;
                case "numberOfEdges":
                    graph.setNumberOfEdges(new BigInteger(nNode.getTextContent()));
                    break;
            }
        }
        
        measurement.setGraph(graph);
    }
}

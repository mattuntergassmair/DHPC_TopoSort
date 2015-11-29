package toposortparser;

/**
 *
 * @author J. Baum
 */
public class TopoSortParser {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        XmlParser xmlParser = new XmlParser();
        

        DbManager dbManager = new DbManager();
        
        dbManager.openConnection(true);
        dbManager.createDatabase();
        
        for (Measurement temp : xmlParser.parseXml("results/resultfile.xml")) {
            dbManager.insertMeasurement(temp);
        }
        
        dbManager.closeConnection();
     
    }

}

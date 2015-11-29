package toposortparser;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author J. Baum
 */
public class TopoSortParser {
    
    private static List<String> files = new ArrayList<>();

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        initFileList();
        
        XmlParser xmlParser = new XmlParser();
        

        DbManager dbManager = new DbManager();
        
        dbManager.openConnection(true);
        dbManager.createDatabase();
        
        for (String filename : files) {
            System.out.println(filename);
            for (Measurement temp : xmlParser.parseXml("results/" + filename)) {
                dbManager.insertMeasurement(temp);
            }
        }
        
        dbManager.closeConnection();
     
    }
    
    private static void initFileList() {
        
        File folder = new File("results/");
        File[] listOfFiles = folder.listFiles();
        files = new ArrayList<>();

        for (File file : listOfFiles) {
            if (file.isFile() && file.getName().endsWith(".xml")) {
                files.add(file.getName());
            }
        }
    }

}

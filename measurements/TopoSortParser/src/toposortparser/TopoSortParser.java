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
        
        boolean deleteDatabase = true;
        
        if (args.length >= 1 && args[0].equals("--noDelete")) {
            deleteDatabase = false;
        }
        
        dbManager.openConnection(deleteDatabase);
        
        if (deleteDatabase) {
            System.out.println("Started parser in \"delete\" mode:");
            dbManager.createDatabase();
        } else {
            System.out.println("Started parser in \"noDelete\" mode:");
        }
        
        
        
        int size = files.size();
        
        int fileCounter = 0;
        for (String filename : files) {
            
            for (Measurement temp : xmlParser.parseXml("../data/" + filename)) {
                dbManager.insertMeasurement(temp);
            }
            fileCounter++;
            
            if (size > 0) {
                System.out.println(fileCounter + "/" + size + " (" + (fileCounter / (size / 100.0)) + "%) of files parsed");
            }
        }
        
        System.out.println(fileCounter + " files parsed.");
        
        dbManager.closeConnection();
     
    }
    
    private static void initFileList() {
        
        File folder = new File("../data/");
        File[] listOfFiles = folder.listFiles();
        files = new ArrayList<>();

        for (File file : listOfFiles) {
            if (file.isFile() && file.getName().endsWith(".xml")) {
                files.add(file.getName());
            }
        }
    }

}

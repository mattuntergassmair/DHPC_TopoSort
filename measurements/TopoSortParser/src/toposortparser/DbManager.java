/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package toposortparser;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

/**
 *
 * @author J. Baum
 */
public class DbManager {

    private Connection connection;

    public void openConnection(boolean deleteDatabase) {
        if (deleteDatabase) {
            File file = new File("measurements.db");
            file.delete();
        }
        
        
        try {
            Class.forName("org.sqlite.JDBC");
            connection = DriverManager.getConnection("jdbc:sqlite:measurements.db");
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        System.out.println("Opened database successfully");
    }

    public void createDatabase() {
        Statement stmt = null;

        try {
            stmt = connection.createStatement();

            stmt.executeUpdate(readCreateSql());
            stmt.close();
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        System.out.println("Database created successfully");
    }

    public void insertMeasurement(Measurement measurement) {
        Statement stmt = null;
        try {
            connection.setAutoCommit(false);
            
            stmt = connection.createStatement();
            String sql = "INSERT INTO measurements ("
                    + "date,number_of_threads,processors,comment,total_time,"
                    + "algorithm,graph_type,graph_num_nodes,graph_num_edges) "
                    + "VALUES ("
                    + measurement.getDate() + ", "
                    + measurement.getNumberOfThreads() + ", "
                    + measurement.getProcessors() + ", "
                    + "'', "
                    + measurement.getTotalTime() + ", "
                    + "'" + measurement.getAlgorithm() + "'" + ", "
                    + "'" + measurement.getGraph().getType() + "'" + ", "
                    + measurement.getGraph().getNumberOfNodes() + ", "
                    + measurement.getGraph().getNumberOfEdges()
                    + ");";
            stmt.executeUpdate(sql);
            
            int measurementId = this.getLastMeasurementId();
            
            for (Thread temp : measurement.getThreads()) {
                insertThread(temp, measurementId);
                int lastThreadId = this.getLastThreadId();
                for (String name : temp.getTimings().keySet()) {
                    insertTiming(name, temp.getTimings().get(name), lastThreadId);
                }
            }
            
            

            stmt.close();
            connection.commit();
            
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        System.out.println("Measurement inserted successfully");
    }
    
    private void insertThread(Thread thread, int measurementId) {
        Statement stmt = null;
        try {
            connection.setAutoCommit(false);
            
            stmt = connection.createStatement();
            String sql = "INSERT INTO threads ("
                    + "measurement_id, thread_id, processed_nodes) "
                    + "VALUES ("
                    + measurementId + ", "
                    + thread.getId() + ", "
                    + thread.getProcessedNodes()
                    + ");";

            stmt.executeUpdate(sql);
            
            stmt.close();
            
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
    }
    
    private void insertTiming(String name, double value, int threadId) {
        Statement stmt = null;
        try {
            connection.setAutoCommit(false);
            
            stmt = connection.createStatement();
            String sql = "INSERT INTO timings ("
                    + "thread_id, name, value) "
                    + "VALUES ("
                    + threadId + ", "
                    + "'" + name + "', "
                    + value
                    + ");";

            stmt.executeUpdate(sql);
            
            stmt.close();
            
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
    }
    
    private int getLastMeasurementId() {
        Statement stmt = null;
        int id = -1;
        try {
            connection.setAutoCommit(false);
            
            stmt = connection.createStatement();
            String sql = "SELECT MAX(id) as id FROM measurements;";
            try (ResultSet rs = stmt.executeQuery(sql)) {
                rs.next();
                id = rs.getInt("id");
            }
            stmt.close();
            connection.commit();
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        
        return id;
    }
    
    private int getLastThreadId() {
        Statement stmt = null;
        int id = -1;
        try {
            connection.setAutoCommit(false);
            
            stmt = connection.createStatement();
            String sql = "SELECT MAX(id) as id FROM threads;";
            try (ResultSet rs = stmt.executeQuery(sql)) {
                rs.next();
                id = rs.getInt("id");
            }
            stmt.close();
            connection.commit();
        } catch (Exception e) {
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
        
        return id;
    }

    public void closeConnection() {
        try {
            connection.close();
        } catch (SQLException ex) {
            System.err.println(ex.getClass().getName() + ": " + ex.getMessage());
        }
    }

    private String readCreateSql() {
        String output = "";
        BufferedReader br = null;

        try {

            String sCurrentLine;

            br = new BufferedReader(new FileReader("schema.sql"));

            while ((sCurrentLine = br.readLine()) != null) {
                output += sCurrentLine;
            }

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null) {
                    br.close();
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }

        return output;
    }

}

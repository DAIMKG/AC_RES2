import org.apache.jena.query.*;
import org.apache.jena.tdb.TDBFactory;

import java.io.*;
import java.util.Date;

import static javafx.application.Platform.exit;

/**
 * Created by serena on 16/12/10.
 */
public class Query3 {
    public static void main(String [] argv) {
        if (argv.length != 1)
        {
            System.out.println("usage: java -jar  Query3.jar sparql_query_file_path");
            exit();
        }
        Double t1 = System.currentTimeMillis()/1000.0;
        //String directory = "/home/litian/data/yago";
        String directory = "/users/serena/desktop/yago2";
        Dataset dataset = TDBFactory.createDataset(directory);
        String rdf = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
        String base = "http://yago-knowledge.org/resource/";
        dataset.begin(ReadWrite.READ);
        // Start READ transaction.
        //   No updates or changes to the dataset are possible while this
        //   dataset is used for a read transaction.
        //   An application can have other Datasets, in the same JVM,
        //   tied to the same TDB database performing read or write
        //   transactions concurrently.
        // Double t1 = System.currentTimeMillis()/1000.0;
        StringBuffer buffer = new StringBuffer();
        try {
             //buffer = null;
            //InputStream is = new FileInputStream(argv[0]);
            InputStream is = new FileInputStream("/Users/serena/Documents/workspace/my_reverse/fact_sparql");
            String line = null;
            BufferedReader reader = new BufferedReader(new InputStreamReader(is));
            line = reader.readLine();
            System.out.println(line);
            int cnt = 0;

            while (line != null) {
                System.out.println(cnt++);
                buffer.append(line);
                buffer.append("\n");
                line = reader.readLine();
            }
            reader.close();
            is.close();
        }catch(IOException e) {
            e.printStackTrace();
        }

            // Do some queries
            /*String sparqlQueryString1 = "SELECT ?d ?c ?a WHERE { ?d " + "<" + base + "isMarriedTo>" + " ?c  . " + "\n"
                    + "?d " + "<" + base + "isLeaderOf>" + " ?a    ." + "\n"
        + "?a <" + rdf+ "type> " + "<" + base +"wordnet_country_108544813>    ." + "\n" +
                    "?c <"+ rdf + "type> " + "<" + base + "wordnet_first_lady_110092880> .}" + "\n";*/
            /*String sparqlQueryString1 = "PREFIX rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#>" + "\n"
                    + "PREFIX base: <http://yago-knowledge.org/resource/>" + "\n";
        sparqlQueryString1 += "SELECT ?1 ?2 ?3 WHERE { ?1 base:isMarriedTo ?3  . " + "\n"
                + "?3 base:isLeaderOf ?2    ." + "\n"
                + "?2 rdf:type ?4 ." + "\n" +
                "?1 rdf:type base:wordnet_first_lady_110092880 ." + "\n" + "OPTIONAL {?2 base:dealsWith base:Japan}" +
        "\n" + "}";*/
        String sparqlQueryString1 = buffer.toString();
        System.out.println(sparqlQueryString1.toString());
        Query query = QueryFactory.create(sparqlQueryString1) ;
        QueryExecution qexec = QueryExecutionFactory.create(query, dataset) ;
        ResultSet results = qexec.execSelect() ;
        //String temp = argv[0];
        String temp = "fact1_out";
        File out = new File(temp);
        OutputStream outf = null;
        try {
            out.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
           outf = new FileOutputStream(out);

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        Double t2 = System.currentTimeMillis()/1000.0;
        Double t3 = t2-t1;
        //ResultSetFormatter.out(outf,results);
        ResultSetFormatter.outputAsCSV(outf,results);
        System.out.println ("time used is: " + t3.toString());
        //ResultSetFormatter.out(results) ;
        qexec.close() ;

        dataset.close();
    }
}

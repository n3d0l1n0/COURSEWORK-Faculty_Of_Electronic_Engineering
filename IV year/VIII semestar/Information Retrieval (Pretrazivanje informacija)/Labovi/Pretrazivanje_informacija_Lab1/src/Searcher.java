import org.apache.lucene.document.Document;
import java.util.HashMap;
import java.util.Map;
import org.apache.lucene.document.LongPoint;
import org.apache.lucene.queryparser.flexible.standard.StandardQueryParser;
import org.apache.lucene.queryparser.flexible.standard.config.PointsConfig;
import org.apache.lucene.search.Query;
import java.text.NumberFormat;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.StoredFields;
import org.apache.lucene.index.Term;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.BooleanClause;
import org.apache.lucene.search.BooleanQuery;
import org.apache.lucene.search.TermQuery;
import org.apache.lucene.document.LongPoint;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.Directory;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.analysis.standard.StandardAnalyzer;

import java.io.IOException;
import java.nio.file.Paths;

public class Searcher {

    public static void main(String[] args) throws Exception {
        
        if (args.length != 1) {
            throw new IllegalArgumentException("Searcher <index dir>");
        }

        String indexDir = args[0];
        testQueries(indexDir);
    }

    public static void testQueries(String indexDir) throws Exception {
        Directory dir = FSDirectory.open(Paths.get(indexDir));
        IndexReader ir = DirectoryReader.open(dir);
        IndexSearcher is = new IndexSearcher(ir);
        StandardAnalyzer analyzer = new StandardAnalyzer();
  
        //7.
        
        System.out.println("\n LOGICKI UPIT SA NOT, AND I OR");

        BooleanQuery.Builder booleanQueryBuilder = new BooleanQuery.Builder();    
        BooleanQuery.Builder orQueryBuilder = new BooleanQuery.Builder();
        orQueryBuilder.add(new TermQuery(new Term("contents", "children")), BooleanClause.Occur.SHOULD);
        orQueryBuilder.add(new TermQuery(new Term("contents", "had")), BooleanClause.Occur.SHOULD);
        
        booleanQueryBuilder.add(orQueryBuilder.build(), BooleanClause.Occur.MUST);
        booleanQueryBuilder.add(new TermQuery(new Term("contents", "a")), BooleanClause.Occur.MUST);
        booleanQueryBuilder.add(new TermQuery(new Term("contents", "greska")), BooleanClause.Occur.MUST_NOT);
        
        Query query1Obj = booleanQueryBuilder.build();
        System.out.println("Objektni model bez parsera:");
        izvrsiUpit(is, ir, query1Obj);

        QueryParser parser1 = new QueryParser("contents", analyzer);
        String queryString1 = "(children OR had) AND a NOT greska";
        Query query1Parsed = parser1.parse(queryString1);
        System.out.println("Objektni model sa parsiranjem:");
        izvrsiUpit(is, ir, query1Parsed);

        //8. 
        
        System.out.println("\n POINT RANGE QUERY");

	     Query query2Obj = LongPoint.newRangeQuery("size", 3000L, 500000L);
	     System.out.println("Objektni model bez parsera:");
	     izvrsiUpit(is, ir, query2Obj);
	
	     StandardQueryParser parser2 = new StandardQueryParser(analyzer);
	     Map<String, PointsConfig> pointsConfigMap = new HashMap<>();
	     pointsConfigMap.put("size", new PointsConfig(NumberFormat.getIntegerInstance(), Long.class));
	     parser2.setPointsConfigMap(pointsConfigMap);
	     Query query2Parsed =parser2.parse("size:[3000 TO 500000]", "size");
	
	     System.out.println("Objektni model sa parsiranjem:");
	     izvrsiUpit(is, ir, query2Parsed);
	
	     ir.close();
	     dir.close();
    }

    private static void izvrsiUpit(IndexSearcher is, IndexReader ir, Query q) throws IOException {
        long start = System.currentTimeMillis();
        TopDocs hits = is.search(q, 10);
        long end = System.currentTimeMillis();
        
        System.out.println("  Upit: " + q.toString());
        System.out.println("  Pronadjeno " + hits.totalHits + " dokumenata (za " + (end - start) + " ms):");
        
        StoredFields storedFields = ir.storedFields();
        for(ScoreDoc scoreDoc : hits.scoreDocs) {
            Document doc = storedFields.document(scoreDoc.doc);
            String putanja = doc.get("fullpath");
            String velicina = doc.get("size"); 
            System.out.println("    -> " + putanja + " (Velicina: " + velicina + " bajtova)");
        }
        System.out.println();
    }
}
import org.apache.lucene.document.Document;
import java.io.IOException;
import java.nio.file.Paths;

import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.StoredFields;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.Explanation;
import org.apache.lucene.search.BoostQuery;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.Directory;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.search.similarities.ClassicSimilarity;
import org.apache.lucene.search.similarities.Similarity;

public class SearcherClassic {

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            throw new IllegalArgumentException("Upotreba: java " + SearcherClassic.class.getName() + " <index dir>");
        }
        String indexDir = args[0];
        testQueries(indexDir);
    }

    public static void testQueries(String indexDir) throws Exception {
        Directory dir = FSDirectory.open(Paths.get(indexDir));
        IndexReader ir = DirectoryReader.open(dir);
        IndexSearcher is = new IndexSearcher(ir);
        StandardAnalyzer analyzer = new StandardAnalyzer();

        Similarity similarity = new ClassicSimilarity();
        is.setSimilarity(similarity);

        System.out.println("\nCLASSIC SIMILARITY: UPIT NAD NASLOVOM");
        QueryParser parserNaslov = new QueryParser("naslov", analyzer);
        Query queryNaslov = parserNaslov.parse("Romeo Julia"); 
        izvrsiUpit(is, ir, queryNaslov, true); 

        System.out.println("\nCLASSIC SIMILARITY: UPIT NAD SADRZAJEM");
        QueryParser parserSadrzaj = new QueryParser("sadrzaj", analyzer);
        Query querySadrzaj = parserSadrzaj.parse("Romeo Julia");
        izvrsiUpit(is, ir, querySadrzaj, true);

        float izracunatiBoostFaktor = 5.609358f; 

        System.out.println("\nCLASSIC SIMILARITY: BOOST-OVAN UPIT");
        Query boostedQuerySadrzaj = new BoostQuery(querySadrzaj, izracunatiBoostFaktor);
        izvrsiUpit(is, ir, boostedQuerySadrzaj, false);

        ir.close();
        dir.close();
    }

    private static void izvrsiUpit(IndexSearcher is, IndexReader ir, Query q, boolean prikaziExplanation) throws IOException {
        long start = System.currentTimeMillis();
        TopDocs hits = is.search(q, 10);
        long end = System.currentTimeMillis();
        
        System.out.println("Upit: " + q.toString());
        System.out.println("Pronadjeno " + hits.totalHits + " dokumenata (za " + (end - start) + " ms):");
        
        StoredFields storedFields = ir.storedFields();
        for(ScoreDoc scoreDoc : hits.scoreDocs) {
            Document doc = storedFields.document(scoreDoc.doc);
            String naslov = doc.get("naslov"); 
            String velicina = doc.get("size"); 
            
            System.out.println(" -> Dokument: " + naslov + " | Velicina: " + velicina + " | SCORE: " + scoreDoc.score);
            
            if (prikaziExplanation) {
                System.out.println("---------------------------------------------------------");
                Explanation exp = is.explain(q, scoreDoc.doc);
                System.out.println(exp.toString());
                System.out.println("---------------------------------------------------------");
            }
        }
    }
}
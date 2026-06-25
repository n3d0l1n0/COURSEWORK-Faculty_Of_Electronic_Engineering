import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.LongPoint;
import org.apache.lucene.document.StoredField;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import org.apache.tika.Tika;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.nio.file.Paths;

public class Indexer {

    private IndexWriter writer;

    public static void main(String[] args) throws Exception {

        if (args.length != 2) {
            throw new IllegalArgumentException("Fali: <indexDir> <dataDir>");
        }

        String indexDir = args[0];
        String dataDir = args[1];

        long start = System.currentTimeMillis();

        Indexer indexer = new Indexer(indexDir);

        int numIndexed;

        try {
            numIndexed = indexer.index(dataDir, new DocumentsFilter());
        } finally {
            indexer.close();
        }

        long end = System.currentTimeMillis();

        System.out.println();
        System.out.println("Indexirano dokumenata: " + numIndexed);
        System.out.println("Vreme indeksiranja: " + (end - start) + " ms");
    }

    public Indexer(String indexDir) throws IOException {

        Directory dir = FSDirectory.open(Paths.get(indexDir));

        writer = new IndexWriter(dir, new IndexWriterConfig(new StandardAnalyzer()));
    }

    public void close() throws IOException {
        writer.close();
    }

    public int index(String dataDir,FileFilter filter) throws Exception {

        File[] files = new File(dataDir).listFiles();

        if (files == null) {
            return 0; }

        for (File f : files) {

            if (!f.isDirectory()
                    && !f.isHidden()
                    && f.exists()
                    && f.canRead()
                    && (filter == null
                    || filter.accept(f))) {

                indexFile(f);
            }
        }

        return writer.getDocStats().numDocs;
    }

    private void indexFile(File f) throws Exception {

        System.out.println("Indexing: " + f.getCanonicalPath());
        Document doc = getDocument(f);
        writer.addDocument(doc);
    }

    protected Document getDocument(File f) throws Exception {

        Document doc = new Document();
        Tika tika = new Tika();
        String content = tika.parseToString(f);

        doc.add(new TextField("contents",content,Field.Store.NO));
        doc.add(new StringField("filename",f.getName(),Field.Store.YES));
        doc.add(new StringField("fullpath",f.getCanonicalPath(),Field.Store.YES));
        long fileSize = f.length();
        doc.add(new LongPoint("size",fileSize));
        doc.add(new StoredField("size", String.valueOf(fileSize)));

        return doc;
    }

    private static class DocumentsFilter implements FileFilter {

        @Override
        public boolean accept(File path) {

            String name = path.getName().toLowerCase();

            return name.endsWith(".pdf")
                    || name.endsWith(".doc")
                    || name.endsWith(".docx")
                    || name.endsWith(".ppt")
                    || name.endsWith(".pptx")
                    || name.endsWith(".html")
                    || name.endsWith(".htm");
        }
    }
}
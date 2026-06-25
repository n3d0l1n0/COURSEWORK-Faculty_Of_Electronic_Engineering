using CommonServiceLocator;
using SolrNet;
using SolrNet.Commands.Parameters;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace LV4_PI
{

    public partial class Form1 : Form
    {
        private ISolrOperations<BookDocument> solr;
        private int currentPage = 1;
        private const int pageSize = 5;

        private void Search()
        {
            string field = cmbField.Text.ToLower();
            string query = txtSearch.Text.Trim();

            ISolrQuery solrQuery;

            if (string.IsNullOrEmpty(query))
            {
                solrQuery = SolrQuery.All;
            }
            else
            {
                if (field == "filesize" || field == "modified" || field == "indexed_date")
                {
                    solrQuery = new SolrQueryByField(field, query);
                }
                else
                {
                    solrQuery = new SolrQuery($"{field}:*{query}*");
                }
            }

            try
            {
                QueryOptions options = new QueryOptions();
                options.Rows = pageSize;
                options.Start = (currentPage - 1) * pageSize;

                var results = solr.Query(solrQuery, options);
                lblTotal.Text = "Ukupno pronadjeno: " + results.NumFound;
                dgvResults.DataSource = results.Select(x => new
                {
                    x.FileName,
                    x.FileSize,
                    x.Modified,
                    x.IndexedDate,
                    x.Path
                }).ToList();

                lblPage.Text = "Stranica " + currentPage;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Greška prilikom pretrage: " + ex.Message, "Solr Greška", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        public Form1()
        {
            InitializeComponent();

            Startup.Init<BookDocument>("http://localhost:8983/solr/knjige");

            solr = ServiceLocator.Current.GetInstance<ISolrOperations<BookDocument>>();
            cmbField.SelectedIndex = 0;
        }

        private void btnIndex_Click(object sender, EventArgs e)
        {
            string folder = txtFolder.Text;

            var files = Directory.GetFiles(folder, "*.txt");

            foreach (var file in files)
            {
                string content = File.ReadAllText(file);

                BookDocument doc =  new BookDocument();

                doc.Id = Guid.NewGuid().ToString();
                doc.FileName = Path.GetFileName(file);
                doc.Content = content;
                doc.FileSize = new FileInfo(file).Length;
                doc.Modified = File.GetLastWriteTime(file);
                doc.IndexedDate = DateTime.Now;
                doc.Path = file;

                solr.Add(doc);
            }

            solr.Commit();
            MessageBox.Show("Indeksiranje završeno");
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            Search();
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            currentPage++;
            Search();
        }

        private void btnPrevious_Click(object sender, EventArgs e)
        {
            if (currentPage > 1)
            {
                currentPage--;
                Search();
            }
        }

        private void dgvResults_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            string path = dgvResults.Rows[e.RowIndex].Cells["Path"].Value.ToString();
            System.Diagnostics.Process.Start(path);
        }
    }
}

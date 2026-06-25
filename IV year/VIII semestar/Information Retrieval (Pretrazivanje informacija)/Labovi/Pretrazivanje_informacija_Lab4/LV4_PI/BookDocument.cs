using SolrNet.Attributes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LV4_PI
{
    internal class BookDocument
    {
        [SolrUniqueKey("id")]
        public string Id { get; set; }

        [SolrField("filename")]
        public string FileName { get; set; }

        [SolrField("content")]
        public string Content { get; set; }

        [SolrField("filesize")]
        public long FileSize { get; set; }

        [SolrField("modified")]
        public DateTime Modified { get; set; }

        [SolrField("indexed_date")]
        public DateTime IndexedDate { get; set; }

        [SolrField("path")]
        public string Path { get; set; }
    }
}

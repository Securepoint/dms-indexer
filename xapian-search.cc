
#include "xapian-search.h"
#include "helpers/xapian-helpers.h"
#include "helpers/base64.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string.h>

using namespace std;

std::vector < string > get_slot_values(Xapian::Database db,
  sp_xapian_slot slot)
{

  std::vector < string > result;

  try {
    Xapian::Enquire enq(db);

    Xapian::ValueCountMatchSpy catspy(slot);
    Xapian::Query query;

    enq.add_matchspy(&catspy);
    enq.set_query(query.MatchAll);
    enq.get_mset(1, db.get_doccount());

    for (Xapian::TermIterator i = catspy.values_begin();
      i != catspy.values_end(); i++) {
      stringstream tmp;
      tmp << *i << " :(" << i.get_termfreq() << ")";
      result.push_back(tmp.str());
    }

  }
  catch(const Xapian::Error & e) {
    if (!memcmp(e.get_type(), "DatabaseModifiedError", 21)) {
      cerr << "reopening database" << endl;
      db.reopen();
      result = get_slot_values(db, slot);
    } else
      cerr << "xapian Error on  : " << __func__ << e.get_description();
  }

  return result;
}

string get_docdata(Xapian::Database xdb, int docid, char *uid, int mimeidx)
{

  Xapian::Document doc;
  try {
    doc = xdb.get_document(docid);
  }
  catch( ...) {
    /* if getting from docid fails, try to get it by slot value */
    //cerr << "xapian Error on  : " << __func__ << e.get_description();
    try {
      ostringstream convert;
      convert << mimeidx;
      doc =
        get_document_from_slotvalue(xdb, SLOT_ID_UID, uid,
        convert.str().c_str());
    }
    catch( ...) {
      return "";
    }
  }
  return (doc.get_data());

}
inline void queryPaserSetPrefix(Xapian::QueryParser * qp)
{

  qp->add_prefix("category", prefix_category);
  qp->add_boolean_prefix("bool_category", prefix_category);
  qp->add_prefix("filetype", prefix_filetype);
  qp->add_boolean_prefix("bool_filetype", prefix_filetype);
  qp->add_prefix("filename", prefix_filename);
  qp->add_boolean_prefix("bool_filename", prefix_filename);
  qp->add_prefix("folder", prefix_folder);
  qp->add_boolean_prefix("bool_folder", prefix_folder);
  qp->add_boolean_prefix("bool_ltafolder", prefix_lta);
  qp->add_prefix("subject", prefix_subject);
  qp->add_boolean_prefix("bool_subject", prefix_subject);
  qp->add_prefix("from", prefix_from);
  qp->add_boolean_prefix("bool_from", prefix_from);
  qp->add_prefix("references", prefix_references);
  qp->add_boolean_prefix("bool_references", prefix_references);
  qp->add_prefix("to", prefix_to);
  qp->add_boolean_prefix("bool_to", prefix_to);
  qp->add_prefix("cc", prefix_cc);
  qp->add_boolean_prefix("bool_cc", prefix_cc);
  qp->add_prefix("mimetype", prefix_mimetype);

}

std::set < std::string > predictWord(Xapian::Database & xdb,
  const string & partialWord, const string & query, const int maxSize)
{
  std::set < std::string > predictionSet;
  try {
    string predicQuery(partialWord);
    if (predicQuery.length() != predicQuery.find_last_of("*")) {
      predicQuery += "*";
    }
    if (!query.empty()) {
      predicQuery += " AND ";
      predicQuery += query;
    }
    Xapian::Query wholequery;
    Xapian::QueryParser qp;
    Xapian::Enquire enquire(xdb);

    Xapian::DateValueRangeProcessor daterange(SLOT_ID_DATE);
    Xapian::Stem stemmer("german");
    qp.set_database(xdb);
    qp.set_stemmer(stemmer);
    qp.set_stemming_strategy(Xapian::QueryParser::STEM_NONE);
    qp.set_default_op(wholequery.OP_OR);
    queryPaserSetPrefix(&qp);
    qp.add_valuerangeprocessor(&daterange);

    wholequery = qp.parse_query(predicQuery,
      qp.FLAG_WILDCARD | qp.FLAG_SPELLING_CORRECTION | qp.
      FLAG_BOOLEAN | qp.FLAG_LOVEHATE | qp.
      FLAG_AUTO_SYNONYMS | qp.FLAG_AUTO_MULTIWORD_SYNONYMS);

    //cout << "Parsed query is: " << wholequery.get_description() << endl;

    enquire.set_query(wholequery);
    Xapian::MSet matched = enquire.get_mset(0, xdb.get_doccount());
    string toLowerPartialWord(partialWord);
    std::transform(partialWord.begin(), partialWord.end(),
      toLowerPartialWord.begin(),::tolower);

    int cnt = 0;
    for (Xapian::MSetIterator i = matched.begin(); i != matched.end(); i++) {
      Xapian::TermIterator termIt = enquire.get_matching_terms_begin(i);
      for (; cnt < maxSize && termIt != enquire.get_matching_terms_end(i);
        termIt++) {
        bool wordIsLarger = (partialWord.length() < (*termIt).length());
        bool startsWithPartialWord =
          ((*termIt).compare(0, toLowerPartialWord.size(),
            toLowerPartialWord) == 0);
        if (wordIsLarger && startsWithPartialWord) {
          predictionSet.insert(*termIt);
          ++cnt;
        }
      }

    }

  }
  catch(const Xapian::Error & e) {
    cerr << e.get_msg() << endl;
    if (!memcmp(e.get_type(), "DatabaseModifiedError", 21)) {
      cerr << "reopening database" << endl;
      xdb.reopen();
      predictionSet = predictWord(xdb, partialWord, query, maxSize);
    }
  }
  return predictionSet;
}

std::vector < SP_SearchResult > search_document(SP_Search Search,
  unsigned int startset, unsigned int *totalresults, string * corrected)
{

  std::vector < SP_SearchResult > Results;
  int relevancedoc;

  try {
    Xapian::Enquire enquire(Search.db);
    Xapian::QueryParser qp;
    Xapian::MSet matches;
    Xapian::RSet rset;

    Xapian::Query wholequery;
    Xapian::DateValueRangeProcessor daterange(SLOT_ID_DATE);
    Xapian::NumberValueRangeProcessor sizerange(SLOT_ID_SIZE, "sizerange", true);

    Xapian::Stem stemmer("german");
    qp.set_database(Search.db);
    qp.set_stemmer(stemmer);
    qp.set_stemming_strategy(Xapian::QueryParser::STEM_SOME);
    qp.set_default_op(wholequery.OP_OR);

    queryPaserSetPrefix(&qp);

    qp.add_valuerangeprocessor(&daterange);
    qp.add_valuerangeprocessor(&sizerange);

    /* if no text in query, Search just for ALL */
    if (Search.get_query().empty()) {
      wholequery = Xapian::Query::MatchAll;
    } else {
      Xapian::QueryParser::feature_flag parserflags;
      /* check for wildcards in query string and set WILDCARD flag */
      if (string::npos != Search.get_query().find('*'))
        parserflags = qp.FLAG_WILDCARD;

      wholequery =
        qp.parse_query(Search.get_query(),
        parserflags | qp.FLAG_SPELLING_CORRECTION | qp.
        FLAG_BOOLEAN | qp.FLAG_LOVEHATE | qp.
        FLAG_AUTO_SYNONYMS | qp.FLAG_AUTO_MULTIWORD_SYNONYMS | qp.
        FLAG_PHRASE);

    }
    if (0 < corrected->length()) {
      corrected->assign(base64_encode(reinterpret_cast < const unsigned char *>(corrected), corrected->length()));
    }
    // Find the top 10 results for the query.
    enquire.set_query(wholequery);
    corrected->append(base64_encode(reinterpret_cast < const unsigned char *> (enquire.get_description().c_str()), (enquire.get_description().length() < 350) ? enquire.get_description().length() : 350 ));

    const std::vector < std::pair < sp_xapian_slot, bool > >sortby =
      Search.getSortbySlotReverse();
    Xapian::MultiValueKeyMaker mk;
    if (!sortby.empty()) {

      for (std::vector < std::pair < sp_xapian_slot,
        bool > >::const_iterator it = sortby.begin(); it != sortby.end();
        ++it) {
        mk.add_value(it->first, it->second);
      }
      enquire.set_sort_by_key_then_relevance(&mk, false);
    } else {
      enquire.set_sort_by_relevance_then_value(SLOT_ID_DATE, true);
    }

    /* filter out duplicates by... */
    if (Search.get_collapse() == "uid") {
      enquire.set_collapse_key(SLOT_ID_UID, 1);
    } else if (Search.get_collapse() == "msgid") {
      enquire.set_collapse_key(SLOT_ID_MSGID, 1);
    } else if (Search.get_collapse() == "hash") {
      enquire.set_collapse_key(SLOT_ID_MD5HASH, 1);
    }

    /* add relevance set if given */
    if (0 < Search.get_relevance_size()) {
      while (0 < (relevancedoc = Search.pop_relevance_doc())) {
        rset.add_document(relevancedoc);
      }
      matches =
        enquire.get_mset(startset, Search.get_howmanyresults(), 0, &rset);
    } else {
      matches = enquire.get_mset(startset, Search.get_howmanyresults());
    }

    /* RESULTS */
    *totalresults = matches.get_matches_estimated();

    /* iterate over matches */
    for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); i++) {
      Xapian::Document doc = i.get_document();
      std::vector < string > doc_mached_words;
      string datesecs = doc.get_value(SLOT_ID_DATE_SEC);

      while ((1 < datesecs.length()) && ('0' == datesecs.at(0)))
        datesecs.erase(0, 1);

      if (0 == datesecs.length())
        datesecs = "0";

      SP_SearchResult searchResult(*i,
        doc.get_value(SLOT_ID_UID),
        doc.get_value(SLOT_ID_MIMEID),
        i.get_percent(),
        doc.get_value(SLOT_ID_DATE),
        doc.get_value(SLOT_ID_DOCNAME),
        doc.get_value(SLOT_ID_FILETYPE),
        doc.get_value(SLOT_ID_CATEGORY),
        doc.get_value(SLOT_ID_FOLDER),
        doc.get_value(SLOT_ID_SIZE),
        doc.get_value(SLOT_ID_USER),
        doc.get_value(SLOT_ID_DATE_FROM_SID),
        datesecs, doc_mached_words,
        doc.get_value(SLOT_ID_KEYWORDS), doc.get_value(SLOT_ID_MIMETYPE));
      searchResult.set_email_from(doc.get_value(SLOT_ID_EMAIL_FROM));
      searchResult.set_email_to(doc.get_value(SLOT_ID_EMAIL_TO));
      searchResult.set_email_cc(doc.get_value(SLOT_ID_EMAIL_CC));
      searchResult.set_email_bcc(doc.get_value(SLOT_ID_EMAIL_BCC));
      searchResult.set_email_subject(doc.get_value(SLOT_ID_EMAIL_SUBJECT));

      Results.push_back(searchResult);
    }
  }
  catch(const Xapian::Error & e) {
    cerr << "xapian Error in: " << __func__ << e.get_description() << endl <<
      "type=" << e.get_type() << endl;
    /* check if DB was Modified by indexer and we need to reopen */
    if (!memcmp(e.get_type(), "DatabaseModifiedError", 21)) {
      cerr << "reopening database" << endl;
      Search.db.reopen();
      Results = search_document(Search, startset, totalresults, corrected);
    } else {
      cerr << "undefined exception" << endl;
    }

  }
  return (Results);
}

#ifndef __XAPIAN_SEARCH_H
#define __XAPIAN_SEARCH_H

#include <xapian.h>
#include "xapian-exports.h"

using namespace std;

class SP_Search {
private:
  string query_string;
  string filetype;
  string category;
  string collapse;
  int howmanyresults;
      std::vector < int >relevance;
      vector < std::pair < sp_xapian_slot, bool > >sortbySlotReverse;

public:
      Xapian::Database db;

      SP_Search(const char *path) {
    this->db = Xapian::Database(path);
    this->howmanyresults = 10;
  } SP_Search(Xapian::Database DB) {
    this->db = Xapian::Database(DB);
    this->howmanyresults = 10;
  }

  void add_query(string Add) {
    this->query_string = Add;
  }
  void add_filetype(string Add) {
    this->filetype = Add;
  }
  void add_category(string Category) {
    this->category = Category;
  }
  void push_relevance_doc(int docid) {
    this->relevance.push_back(docid);
  }
  string get_query() const {
    return query_string;
  }
  int get_howmanyresults() const {
    return howmanyresults;
  }
  string get_filetype() const {
    return filetype;
  }
  string get_category() const {
    return category;
  }
  int pop_relevance_doc() {
    int element = 0;
    if (!relevance.empty()) {
      element = relevance.back();
      relevance.pop_back();
    }
    return element;
  }
  size_t get_relevance_size() const {
    return (relevance.size());
  }
  const string & get_collapse() const {
    return collapse;
  }
  void set_collapse(const string & Collapse) {
    this->collapse = Collapse;
  }
  void set_howmanyresults(int num_results) {
    this->howmanyresults = num_results;
  }

  const vector < std::pair < sp_xapian_slot, bool > >&getSortbySlotReverse() const {
    return sortbySlotReverse;
  }
  void setSortbySlotReverse(const vector < std::pair < sp_xapian_slot,
    bool > >&sortbySlotReverse) {
    this->sortbySlotReverse = sortbySlotReverse;
  }
};

class SP_SearchResult {
private:
  int docid;
  string uid;
  string mimeid;
  int percent;
  string date;
  string docname;
  string filetype;
  string category;
  string folder;
  int size;
  string username;
  string received_date;
  string date_secs;
  string email_from;
  string email_to;
  string email_cc;
  string email_bcc;
  string email_subject;
         std::vector < string > matched_words;
  string keywords;
  string mimetype;

public:

     
     
     SP_SearchResult(int Docid, string Uid, string Mimeid,
    int Percent, string Date, string Docname, string Filetype,
    string Category, string Folder, string serializedSize, string Username,
    string Received_date, string Date_secs,
    std::vector < string > Matched_words, string Keywords, string Mimetype) {
    this->docid = Docid;
    this->uid = Uid;
    this->mimeid = Mimeid;
    this->percent = Percent;
    this->date = Date;
    this->docname = Docname;
    this->filetype = Filetype;
    this->category = Category;
    this->folder = Folder;
    this->size = Xapian::sortable_unserialise(serializedSize);
    this->username = Username;
    this->received_date = Received_date;
    this->date_secs = Date_secs;
    this->matched_words = Matched_words;
    this->keywords = Keywords;
    this->mimetype = Mimetype;
  } int get_docid() const {
    return (docid);
  }
  const char *get_mimetype() const {
    return (mimetype.c_str());
  }
  const char *get_uid() const {
    return (uid.c_str());
  }
  const char *get_mimeid() const {
    return (mimeid.c_str());
  }
  int get_percent() const {
    return (percent);
  }
  const char *get_date() const {
    return (date.c_str());
  }
  const char *get_docname() const {
    return (docname.c_str());
  }
  const char *get_filetype() const {
    return (filetype.c_str());
  }
  const char *get_category() const {
    return (category.c_str());
  }
  const char *get_folder() const {
    return (folder.c_str());
  }
  int get_size() const {
    return (size);
  }
  const char *get_username() const {
    return (username.c_str());
  }
  const char *get_received_date() const {
    return (received_date.c_str());
  }
  const char *get_date_secs() const {
    return (date_secs.c_str());
  }
  const std::vector < string > &get_matched_words() const {
    return matched_words;
  }
  const char *get_email_bcc() const {
    return (email_bcc.c_str());
  }
  void set_email_bcc(const string & emailBcc) {
    email_bcc = emailBcc;
  }

  const char *get_email_cc() const {
    return (email_cc.c_str());
  }
  void set_email_cc(const string & emailCc) {
    email_cc = emailCc;
  }

  const char *get_email_from() const {
    return (email_from.c_str());
  }
  void set_email_from(const string & emailFrom) {
    email_from = emailFrom;
  }

  const char *get_email_subject() const {
    return (email_subject.c_str());
  }
  void set_email_subject(const string & emailSubject) {
    email_subject = emailSubject;
  }

  const char *get_email_to() const {
    return (email_to.c_str());
  }
  const char *get_keywords() const {
    return (keywords.c_str());
  }
  void set_email_to(const string & emailTo) {
    email_to = emailTo;
  }
};

inline void queryPaserSetPrefix(Xapian::QueryParser * qp);

std::set < std::string > predictWord(Xapian::Database & xdb,
  const string & partialWord, const string & query, const int maxSize);

std::vector < SP_SearchResult > search_document(SP_Search search,
  unsigned int startset, unsigned int *totalresults, string * corrected);

/* returns a list of all unique slot values */
std::vector < string > get_slot_values(Xapian::Database db,
  sp_xapian_slot slot);

/* first try is get document by docid, if that fails (in case of multiple Databases opended)
 * the document is searched by uid/mimeid and then returned */
string get_docdata(Xapian::Database xdb, int docid, char *uid, int mimeidx);

#endif

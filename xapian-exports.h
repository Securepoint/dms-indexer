#ifndef __XAPIAN_EXPORTS_H
#define __XAPIAN_EXPORTS_H
#include<map>
#include<string>

typedef enum {
  SLOT_ID_DATE = 10,
  SLOT_ID_UID = 11,
  SLOT_ID_DOCNAME = 12,
  SLOT_ID_USER = 13,
  SLOT_ID_CATEGORY = 14,
  SLOT_ID_FOLDER = 15,
  SLOT_ID_MIMEID = 16,
  SLOT_ID_FILETYPE = 17,
  SLOT_ID_MSGID = 18,
  SLOT_ID_REFERENCES = 19,
  SLOT_ID_REPLYTO = 20,
  SLOT_ID_LTA = 21,
  SLOT_ID_MSG_SID = 22,
  SLOT_ID_SIZE = 23,
  SLOT_ID_IMAPID = 24,
  SLOT_ID_MD5HASH = 25,
  SLOT_ID_UNIQUEID = 26,
  SLOT_ID_DATE_FROM_SID = 27,
  SLOT_ID_LTA_UID = 28,
  SLOT_ID_KEYWORDS = 29,
  SLOT_ID_DATE_SEC = 30,
  SLOT_ID_EMAIL_FROM = 31,
  SLOT_ID_EMAIL_TO = 32,
  SLOT_ID_EMAIL_CC = 33,
  SLOT_ID_EMAIL_BCC = 34,
  SLOT_ID_EMAIL_SUBJECT = 35,
  SLOT_ID_MIMETYPE = 36,

  FIRST = SLOT_ID_DATE,
  LAST = SLOT_ID_MIMETYPE
} sp_xapian_slot;

class SpXapianStrSlot {
private:
  SpXapianStrSlot() {
    spXapiStrSlots["date"] = SLOT_ID_DATE;
    spXapiStrSlots["uid"] = SLOT_ID_UID;
    spXapiStrSlots["docname"] = SLOT_ID_DOCNAME;
    spXapiStrSlots["user"] = SLOT_ID_USER;
    spXapiStrSlots["category"] = SLOT_ID_CATEGORY;
    spXapiStrSlots["folder"] = SLOT_ID_FOLDER;
    spXapiStrSlots["mimeid"] = SLOT_ID_MIMEID;
    spXapiStrSlots["filetype"] = SLOT_ID_FILETYPE;
    spXapiStrSlots["msgid"] = SLOT_ID_MSGID;
    spXapiStrSlots["references"] = SLOT_ID_REFERENCES;
    spXapiStrSlots["replyto"] = SLOT_ID_REPLYTO;
    spXapiStrSlots["lta"] = SLOT_ID_LTA;
    spXapiStrSlots["msg_sid"] = SLOT_ID_MSG_SID;
    spXapiStrSlots["size"] = SLOT_ID_SIZE;
    spXapiStrSlots["imapid"] = SLOT_ID_IMAPID;
    spXapiStrSlots["md5hash"] = SLOT_ID_MD5HASH;
    spXapiStrSlots["uniqueid"] = SLOT_ID_UNIQUEID;
    spXapiStrSlots["date_from_sid"] = SLOT_ID_DATE_FROM_SID;
    spXapiStrSlots["lta_uid"] = SLOT_ID_LTA_UID;
    spXapiStrSlots["keywords"] = SLOT_ID_KEYWORDS;
    spXapiStrSlots["date_sec"] = SLOT_ID_DATE_SEC;
    spXapiStrSlots["email_from"] = SLOT_ID_EMAIL_FROM;
    spXapiStrSlots["email_to"] = SLOT_ID_EMAIL_TO;
    spXapiStrSlots["email_cc"] = SLOT_ID_EMAIL_CC;
    spXapiStrSlots["email_bcc"] = SLOT_ID_EMAIL_BCC;
    spXapiStrSlots["email_subject"] = SLOT_ID_EMAIL_SUBJECT;
  } SpXapianStrSlot(const SpXapianStrSlot &) {
  };                            //no copy constuctor
  SpXapianStrSlot & operator =(const SpXapianStrSlot &);        //no copys
  std::map < std::string, sp_xapian_slot > spXapiStrSlots;
public:
  static SpXapianStrSlot & instance() {
    static SpXapianStrSlot _instance;
    return _instance;
  }
  sp_xapian_slot getEnum(const std::string & str, bool & found) {
    sp_xapian_slot ret = FIRST;
    found = false;
    std::map < std::string, sp_xapian_slot >::iterator it;
    it = spXapiStrSlots.find(str);
    if (it != spXapiStrSlots.end()) {
      found = true;
      ret = spXapiStrSlots.find(str)->second;
    }
    return ret;
  }
};

static const char *prefix_filetype = "A";
static const char *prefix_category = "B";
static const char *prefix_filename = "C";
static const char *prefix_folder = "D";

static const char *prefix_references = "E";
static const char *prefix_from = "F";
static const char *prefix_subject = "G";
static const char *prefix_to = "H";
static const char *prefix_cc = "I";
static const char *prefix_lta = "L";
static const char *prefix_mimetype = "M";

static const char *prefix_sid = "P";
#endif

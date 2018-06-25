#include <xapian.h>
#include "../xapian-exports.h"
#include "base32.h"

#include <iostream>

/* check if category is correct base32  #2360 */
std::string check_category(std::string category)
{
  std::string b32tmp = "";
  b32tmp = base32_decode(category);
  if (0 < b32tmp.size()) {
    return category;
  } else {
    /* do base32 encode */
    b32tmp =
      base32_encode((unsigned char *) category.c_str(), category.size());
  }
  return b32tmp;
}

Xapian::Document get_document_from_slotvalue(Xapian::Database xdb,
  sp_xapian_slot slot, char *slotvalue, const char *mimeidx)
{
  Xapian::Document emptydoc;
  try {
    Xapian::Enquire enq(xdb);
    Xapian::MSet matches;

    Xapian::ValueSetMatchDecider dec(slot, true);

    dec.add_value(slotvalue);
    enq.set_query(Xapian::Query::MatchAll);

    matches = enq.get_mset(0, 300, 0, 0, &dec);

    if (0 < matches.get_matches_estimated()) {
      for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); i++) {
        if (0 == i.get_document().get_value(SLOT_ID_MIMEID).compare(mimeidx))
          return i.get_document();
      }
    }
  }
  catch(const Xapian::Error & e) {
    std::cerr << "caught exception in: " << __func__ << std::endl;
    //throw;

  }
  return emptydoc;
}

/* checks if the given mime part is already indexed
 *
 * - returns the docid if Document is present */
int check_for_slotvalue(Xapian::WritableDatabase xdb, sp_xapian_slot slot,
  const char *value)
{

  try {
    Xapian::Enquire enq(xdb);
    Xapian::MSet matches;

    Xapian::ValueSetMatchDecider dec(slot, true);

    dec.add_value(value);
    enq.set_query(Xapian::Query::MatchAll);

    matches = enq.get_mset(0, 1, 0, 0, &dec);

    if (0 < matches.get_matches_estimated()) {
      return (matches.begin().get_document().get_docid());
    }

    return 0;

  }
  catch(const Xapian::Error & e) {
    return (-2);
  }
}

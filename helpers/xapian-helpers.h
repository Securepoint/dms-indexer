#ifndef __XAPIAN_HELPERS_H
#define __XAPIAN_HELPERS_H 1

Xapian::Document get_document_from_slotvalue(Xapian::Database db,
  sp_xapian_slot slot, char *slotvalue, const char *mimeidx);

int check_for_slotvalue(Xapian::WritableDatabase xdb, sp_xapian_slot slot,
  const char *value);

#endif

#include "vice_label_file.hpp"

// ------------------------------------------------------------------------
const char *ViceLabelFile::getName() const {
  return "VICE label file";
}

// ------------------------------------------------------------------------
const char *ViceLabelFile::getDescription() const {
  return "VICE label file as generated by ld65 (ca65)";
}

// ------------------------------------------------------------------------
uint32_t ViceLabelFile::getFeatures() const {
  return 0
    | SymbolFileInterface::Readable
    | SymbolFileInterface::Writable
    | SymbolFileInterface::Symbols
  ;
}

// ------------------------------------------------------------------------
int32_t ViceLabelFile::scoreReadString(const lstring &rows) const {
  if (rows.size() == 0) {
    return -1;
  }

  const string &row = rows[0];
  if (row.length() < 11) {
    return -1;
  }

  if (row[2] != ' ' || row[9] != ' ') {
    return -1;
  }

  if (row[0] == 'a' && row[1] == 'l') {
    return 1;
  }

  return -1;
}

// ------------------------------------------------------------------------
bool ViceLabelFile::read(const lstring &rows, SymbolMap *map) const {
  for (uint32_t i=0; i<rows.size(); i++) {
    const string &row = rows[i];

    if (row.length() < 11) {
      continue;
    }

    if (row[0] != 'a' || row[1] != 'l' || row[2] != ' ') {
      continue;
    }

    map->addLocation(
      hex(substr(row, 3, 6)),
      substr(row, 10, row.length() - 10)
    );
  }

  return true;
}

// ------------------------------------------------------------------------
bool ViceLabelFile::write(nall::file &f, SymbolMap *map) const {
  foreach(symbols, map->symbols) {
    Symbol s = symbols.getSymbol();

    if (!s.isInvalid()) {
      f.print("al ", hex<6>(symbols.address), " ", s.name, "\n");
    }
  }

  return true;
}

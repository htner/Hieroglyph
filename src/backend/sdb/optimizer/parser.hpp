
#pragma once

#include "backend/sdb/common/pg_export.hpp"
#include "utils/palloc.h"

namespace sdb {

class Parser {
public:
	Parser() { 
    MemInit();
  }

  ~Parser() {
    if (parse_context_ != NULL) {
      MemoryContextDelete(parse_context_);
    }
  }

	void MemInit() {
		parse_context_ = AllocSetContextCreate(TopMemoryContext,
										                      "ParseContext",
										                      ALLOCSET_DEFAULT_SIZES);	
	}

	List* Parse(const char* query_string) {
		List* parsetree_list;

		auto oldcontext = MemoryContextSwitchTo(parse_context_);
		parsetree_list = raw_parser(query_string);
    elog_node_display(PG_LOG, "parse results:", parsetree_list, true);
    MemoryContextSwitchTo(oldcontext);
		return parsetree_list;
	}

private:
  MemoryContext parse_context_ = NULL;
};

}

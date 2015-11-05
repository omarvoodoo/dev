
#ifndef __fast_searcher_index_standard_tokenizer_h_included__
#define __fast_searcher_index_standard_tokenizer_h_included__

class Token;

namespace StandardTokenizer {
    // [a-zA-Z][0-9a-zA-Z_-.]+
    Token* tokenize(char* raw_data);
}

#endif /* __fast_searcher_index_standard_tokenizer_h_included__ */

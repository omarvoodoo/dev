
#include "standard_tokenizer.h"
#include "token.h"

namespace StandardTokenizer {
    Token* tokenize(char* raw_data) {
        if(!raw_data)
            return nullptr;

        Token* token_head = nullptr;
        Token* token = nullptr;
        char*  str_pos;
        size_t str_count = 0;
        size_t len = strlen(raw_data);

        for(int i=0; i<len; ++i) {
            if((raw_data[i] >= 'a' && raw_data[i] <= 'z') || (raw_data[i] >= 'A' && raw_data[i] <= 'Z')) {
                if(str_count == 0)
                    str_pos = raw_data+i;
                str_count++;
            } else if((raw_data[i]>='0' && raw_data[i]<='9') || raw_data[i]=='.' || raw_data[i]=='-' || raw_data[i]=='_') {
                if(str_count == 0)
                    continue;
                str_count++;
            } else {
                if(str_count > 0) {
                    if(!token_head) {
                        token_head = new Token(str_pos, str_count);
                        token = token_head;
                    } else {
                        Token* new_token = new Token(str_pos, str_count);
                        token->setNext(new_token);
                        token = new_token;
                    }
                    str_count = 0;
                }
            }
        }

        return token_head;
    }
}


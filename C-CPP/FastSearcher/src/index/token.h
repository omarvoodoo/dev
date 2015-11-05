
#ifndef __fast_searcher_token_h_included__
#define __fast_searcher_token_h_included__

#include <iostream>
#include <memory>

class Token {
    char* data_;    // 토큰 이름
    Token* next_;
public:
    Token(char* str, size_t len) : data_(nullptr), next_(nullptr) {
        
        try {
            data_ = (char*)malloc(len+1);
        } catch(std::bad_alloc& e) {
            printf("malloc error\n");
            throw e;
        }

        memcpy(data_, str, len);
        data_[len] = '\0';
    }

    ~Token() {
        // data_의 메모리 해제는 역색인 처리 루틴에서 한다.
        /*if(data_)
            free(data_);*/
    }

    bool setNext(Token* token) {
        if(!token)
            return false;

        if(next_) {
            if(token->next_) {
                // 새로운 토큰(A)이 중간에 삽입되는 상황에서 A가 이미 다음 토큰과
                // 연결된 상태라면 실패를 반환한다.
                return false;
            } else {
                token->next_ = next_;
            }
        }

        next_ = token;
    }

    Token* next() { return next_; }

    char* data() { return data_; }
};

#endif /* __fast_searcher_token_h_included__ */

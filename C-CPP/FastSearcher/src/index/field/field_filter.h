
#ifndef __fast_searcher_field_filter_h_included__
#define __fast_searcher_field_filter_h_included__

#include <string>
#include <vector>
#include <tchar.h>
#include "field_tokenizer.h"

#define TIMESTAMP_LEN 17
#define TIMESTAMP_NAME _T("__timestamp__")

namespace FieldFilter {
    // Key/Value 쌍으로 이루어진 객체로 필드화의 결과의 형식으로 사용된다.
    // Unit은 객체인 동시에 리스트 역할을 한다.
    class Unit {
        Unit* next_;

    public:
        ~Unit();
        Unit() : next_(nullptr), name_(nullptr), value_(nullptr) {}        
        void setNext(Unit* unit) {
            next_ = unit;
        }

        // 다음 Unit의 포인터를 반환한다.
        // nullptr일 경우 다음 Unit이 없음을 나타낸다.
        Unit* next() { return next_; }
        void  print();

        TCHAR* name_;   // key
        TCHAR* value_;  // value
    };

    namespace Property {
        Unit* date      (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* time      (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* value_t2  (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* equal     (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* ip        (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* date_t2   (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* date_t3   (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
        Unit* date_t4   (FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt);
    }

    // 비정형의 문자열 데이터에서 자체 기준에 만족하는 형태의 데이터를 추출하여
    // Key/Value 형식의 Unit 리스트로 반환한다.
    Unit* filter( TCHAR* raw );

    void getCurrentTimestamp(TCHAR* out);    // YYYYMMDDhhmmss000

    void printTokenStream( FieldTokenizer::Token* token_stream );        
    void printUnitStream( Unit* unit_stream);
}

#endif /* __fast_searcher_field_filter_h_included__ */

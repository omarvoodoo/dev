
#ifndef __fast_searcher_field_filter_h_included__
#define __fast_searcher_field_filter_h_included__

#include <string>
#include <vector>
#include <tchar.h>
#include "field_tokenizer.h"

#define TIMESTAMP_LEN 17
#define TIMESTAMP_NAME _T("__timestamp__")

namespace FieldFilter {
    // Key/Value ������ �̷���� ��ü�� �ʵ�ȭ�� ����� �������� ���ȴ�.
    // Unit�� ��ü�� ���ÿ� ����Ʈ ������ �Ѵ�.
    class Unit {
        Unit* next_;

    public:
        ~Unit();
        Unit() : next_(nullptr), name_(nullptr), value_(nullptr) {}        
        void setNext(Unit* unit) {
            next_ = unit;
        }

        // ���� Unit�� �����͸� ��ȯ�Ѵ�.
        // nullptr�� ��� ���� Unit�� ������ ��Ÿ����.
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

    // �������� ���ڿ� �����Ϳ��� ��ü ���ؿ� �����ϴ� ������ �����͸� �����Ͽ�
    // Key/Value ������ Unit ����Ʈ�� ��ȯ�Ѵ�.
    Unit* filter( TCHAR* raw );

    void getCurrentTimestamp(TCHAR* out);    // YYYYMMDDhhmmss000

    void printTokenStream( FieldTokenizer::Token* token_stream );        
    void printUnitStream( Unit* unit_stream);
}

#endif /* __fast_searcher_field_filter_h_included__ */

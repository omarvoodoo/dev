
#ifndef __fast_searcher_index_field_tokenizer_h_included__
#define __fast_searcher_index_field_tokenizer_h_included__

#include <string>
#include <tchar.h>

#define BIT_ZERO            0x0000000000000000
#define BIT_ALL_PROPERTY    0X1111111111100000
#define BIT_DATE            0x1000000000000000
#define BIT_TIME            0X0100000000000000
#define BIT_KEY             0x0010000000000000
#define BIT_VALUE_T1        0X0001000000000000
#define BIT_VALUE_T2        0X0000100000000000
#define BIT_EQUAL           0X0000010000000000
#define BIT_IP              0x0000001000000000
#define BIT_DATE_T2         0X0000000100000000
#define BIT_DATE_T3         0X0000000010000000
#define BIT_DATE_T4         0X0000000001000000
#define BIT_VALUE_T3        0X0000000000100000

#define RBIT_ALL_PROPERTY    0X0000000000011111
#define RBIT_DATE            0x0111111111111111
#define RBIT_TIME            0X1011111111111111
#define RBIT_KEY             0x1101111111111111
#define RBIT_VALUE_T1        0X1110111111111111
#define RBIT_VALUE_T2        0X1111011111111111
#define RBIT_EQUAL           0X1111101111111111
#define RBIT_IP              0x1111110111111111
#define RBIT_DATE_T2         0X1111111011111111
#define RBIT_DATE_T3         0X1111111101111111
#define RBIT_DATE_T4         0X1111111110111111
#define RBIT_VALUE_T3        0X1111111111011111

namespace FieldTokenizer {
    // 토큰의 형식
    enum type {
        date = 1,       // YYYY/MM/DD                       // 1
        time,                                               // 
        key,                                                // 
        value_t1,       // 일반적인 형태의 문자열           // 영문자 숫자로 시작되어야 하고
                        // 다음의 특수 문자만 허용한다 :?{}.-_ 
                        // 중간에 허용하지 않는 특수문자가 나타날시 직전까지만 값으로 취급한다.
        value_t2 = 5,   // 대괄호([])로 감싸져있는 문자열   // 
        equal,          // Key/Value 구분자                 // 
        ip,                                                 // 
        date_t2,        // YYYY-MM-DD                       // 1
        date_t3,        // MM/DD/YYYY                       // 1
        date_t4 = 10,   // MM/DD                            // 1
        value_t3,       // 숫자만으로 구성되어 있는 형태 // 아직 사용되지 않음
    };

    // 토큰은 분리된 문자열을 표현한다.
    // 토큰은 객체이면서 리스트 역할을 한다.
    class Token {
        Token* next_;

    public:
        Token(TCHAR* token_buf_pos, size_t& token_size, long long& token_type, size_t depth)
            : token_buf_pos_(token_buf_pos), token_size_(token_size), token_type_(token_type), depth_(depth), next_(nullptr) {}

        void setNext( Token* next_token ) {
            next_ = next_token;
        }

        // 다음 토큰의 포인터를 반환한다.
        // nullptr을 반환할 경우 다음 토큰이 없는 것이다.
        Token* next() { return next_; }

        // 속성 검증 함수
        bool checkDate();
        bool checkTime();
        bool checkIp();
        bool checkDateT2();
        bool checkDateT3();
        bool checkDateT4();

        TCHAR* token_buf_pos_;  // 토큰은 문자열 버퍼를 따로 관리하지 않고
                                // 원본 문자열 버퍼를 참조하여 이용한다.
        size_t token_size_;
        long long token_type_;  // 토큰의 형식
        size_t depth_;          // 대괄호를 필드화 할때 필드화 깊이를 나타낸다.
    };

    void print(Token* token);

    // 문자열 데이터를 특정 기호 기준으로 토큰화 한다.
    Token* tokenize( TCHAR* raw, size_t raw_len, size_t depth );
};

#endif /* __fast_searcher_index_field_tokenizer_h_included__ */

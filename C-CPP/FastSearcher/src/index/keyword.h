
#ifndef __fast_searcher_keyword_h_included__
#define __fast_searcher_keyword_h_included__

class Keyword  {
public:
    enum Type { Value, And, Or, Not };

private:
    Keyword::Type type_;
    const char* value_;

public:
    Keyword(Type type) : type_(type), value_(nullptr) {}
    Keyword(Type type, const char* value) : type_(type), value_(value) {}

    Type GetType() { return type_; }
    const char* GetValue() { return value_; }
};

#endif /* __fast_searcher_keyword_h_included__ */

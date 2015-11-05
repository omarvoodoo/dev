
#include <CLucene.h>
#include <tchar.h>
#include <mecab.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define CHECK(eval) if (! eval) { \
   const char *e = tagger ? tagger->what() : MeCab::getTaggerError(); \
   std::cerr << "Exception:" << e << std::endl; \
   delete tagger; \
   return -1; }

using namespace lucene::index;
using namespace lucene::search;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace std;

string MbToUtf8(string src) {
	const int buf_size = 40960;
	wchar_t unicode[buf_size];
	char    utf8[buf_size];

	int unicode_size = MultiByteToWideChar(CP_ACP, 0, src.c_str(), src.size(), unicode, buf_size);
	int utf8_size    = WideCharToMultiByte(CP_UTF8, 0, unicode, unicode_size, utf8, buf_size, NULL, NULL);

	return string(utf8, utf8_size);
}

wstring Utf8ToUnicode(string src) {
	wchar_t strUnicode[4096] = {0,};
	int nLen = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.size(), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.size(), strUnicode, nLen);

	return wstring(strUnicode);
}

int read_file(const char* file_path, char* out_buf, int out_buf_size) {
	FILE* fp = fopen(file_path, "r");
	if(NULL == fp) {
		printf("File open failed, READ MODE\n");
		return -1;
	}

	int len = 0;
	while( !feof(fp) ) {
		len += fread(out_buf+len, sizeof(char), out_buf_size, fp);
	}
	fclose(fp);

	out_buf[len] = 0;

	return len;
}

void foo() {
	lucene::store::RAMDirectory ram;
    lucene::analysis::SimpleAnalyzer a;

    lucene::index::IndexWriter ndx(&ram, &a, true);
    
    lucene::document::Document doc;
    TCHAR buf[4096] = _T("Abcd 안녕 로그온 로그아웃12 hahaha");
    doc.add(*_CLNEW lucene::document::Field(_T("field0"), buf, lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED));
    ndx.addDocument(&doc);
    ndx.close();

    IndexReader* reader = IndexReader::open(&ram);
    IndexSearcher searcher(reader);
    
    const TCHAR* query1 = _T("로그");
    Hits* hits = NULL;
    QueryParser parser(_T("field0"), &a);
    Query* query = parser.parse(query1);
    hits = searcher.search(query);
    size_t size = hits->length();

    for(size_t i=0; i<size; ++i) {
        Document& document = hits->doc(i);
        const TCHAR* value = document.get(_T("field0"));
        _tprintf(value);
    }

    int b= 0;

    ndx.close();
}

int joo() {	
	const int buf_size = 40960;
	char buf[buf_size] = {0,};
    char out_buf[buf_size];
	int read_len = read_file("../../samples/windows_event_00.txt", buf, buf_size);

	if(read_len < 0) {
		printf("read failed\n");
		return -1;
	}

	MeCab::MecabParser parser;

	int size = parser.Parse(buf, out_buf, buf_size);

	if( size > 0 ) {
		FILE* fp = fopen("../../samples/output/windows_event_00.txt", "w");
		if(fp) {
			fwrite(out_buf, sizeof(char), size, fp);
			//fwrite(strUnicode, sizeof(wchar_t), nLen, fp);
		}
		fclose(fp);
	}

	return 0;
}

int help() {
	printf("Usage:\n");
	printf("1. MecabDemo -text \"<string>\"\n");
	printf("2. MecabDemo -file <input file path> <output file path>\n");
	printf("3. MecabDemo -index\n");

	return 0;
}

int buff(char* src, char* out_buf, int out_buf_size) {
	MeCab::MecabParser parser;

	int len = parser.Parse(MbToUtf8(src).c_str(), out_buf, out_buf_size);

	if( len <= 0 )
		return -1;

	return 0;
}

int text(char* src) {
	const int out_buf_size = 40960;
    char out_buf[out_buf_size];
	
	int ret = buff(src, out_buf, out_buf_size);

	if(ret < 0) {
		return -1;
	}

	wstring wstr = Utf8ToUnicode(out_buf);
	wprintf(L"%s\n", wstr.c_str());

	return 0;
}

int file(char* in_file_path, char* out_file_path) {
	const int buf_size = 40960;
	char buf[buf_size] = {0,};
    char out_buf[buf_size];
	int read_len = read_file(in_file_path, buf, buf_size);

	if(read_len < 0) {
		printf("read failed\n");
		return -1;
	}

	MeCab::MecabParser parser;

	int size = parser.Parse(buf, out_buf, buf_size);

	if( size > 0 ) {
		FILE* fp = fopen(out_file_path, "w");
		if(fp) {
			fwrite(out_buf, sizeof(char), size, fp);
			//fwrite(strUnicode, sizeof(wchar_t), nLen, fp);
		} else {
			printf("File open failed, WRITE MODE\n");
		}
		fclose(fp);
	} else {
		printf("Parse failed\n");
	}

	return 0;
}

void printInputMsg() {
	printf("Enter Keyword OR q ==> ");
}

int index( char* in_file_path ) {
	// 입력 파일의 데이터를 줄단위로 색인하고 검색한다.
	lucene::store::RAMDirectory ram;
    lucene::analysis::SimpleAnalyzer a;
    lucene::index::IndexWriter ndx(&ram, &a, true);

	const int buf_size = 40960;
	char buf[buf_size] = {0,};
	int read_len = read_file(in_file_path, buf, buf_size);

	if(read_len < 0) {
		printf("read failed\n");
		return -1;
	}

	wstring raw_data = Utf8ToUnicode(buf);

	// 여기까지는 준비 단계!
	// 지금부터 참고하면됨.

	/** 색인 **/
	MeCab::MecabParser parser;	// 한글 형태소 분석 객체
	wchar_t out_buf[buf_size];
	wstring wstr;
	wstringstream ss(raw_data);
	while(!ss.eof()) {
		getline(ss, wstr);

		if(wstr.empty())
			continue;

		// 한글 형태소 분석		
		int len = parser.Parse(wstr.c_str(), out_buf, buf_size);

		// 결과값이 0또는 -1이라면 분석이 실패한 것이다.
		if( len <= 0 )
			continue;

		wprintf(_T("%s\n"), wstr.c_str());

		lucene::document::Document doc;
		doc.add(*_CLNEW lucene::document::Field(_T("contents"), out_buf, lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED));
		ndx.addDocument(&doc);
	}

	ndx.close();

	/** 검색 **/
	IndexReader* reader = IndexReader::open(&ram);
    IndexSearcher searcher(reader);
	Hits* hits = NULL;
    QueryParser query_parser(_T("contents"), &a);

	printInputMsg();

	char query_str[4096];
	while( scanf("%s", query_str ) != EOF ) {
		if(strcmp(query_str, "q") == 0)
			break;

		wstring wstr = Utf8ToUnicode(MbToUtf8(query_str));	// 콘솔로 입력받기위한 인코딩 부분
															// Dataface에서는 의미없음.

		// 한글 형태소 분석		
		int len = parser.Parse(wstr.c_str(), out_buf, buf_size);

		// 결과값이 0또는 -1이라면 분석이 실패한 것이다.
		if( len <= 0 ) {
			printInputMsg();
			continue;
		}

		Query* query = query_parser.parse(out_buf);
		hits = searcher.search(query);
		size_t size = hits->length();

		if(size <= 0) {
			_tprintf(_T("No Result.\n"));
		} else {
			_tprintf(_T("%d Result found!\n"), size);
			for(size_t i=0; i<size; ++i) {
				Document& document = hits->doc(i);
				const TCHAR* value = document.get(_T("contents"));
				_tprintf(_T("%s\n"), value);
			}
		}

		printInputMsg();
	}
    
	
	return 0;
}

int command(int argc, char* argv[]) {
	// 특정 문자열 값에 대한 결과를 화면에 출력
	// MecabDemo -text <문자열>

	// 특정 경로의 파일 내용을 분석하여 파일로 출력
	// MecabDemo -file <원본 파일 경로> <출력 파일 경로>

	if( argc <= 2 ) {
		return help();
	} else {
		string option(argv[1]);

		if( option == "-text" ) {
			string str;
			for(int i=2; i<argc; i++) {
				str.append(argv[i]);
				str.push_back(' ');
			}

			if(text((char*)str.c_str()) < 0)
				return help();
		} else if( option == "-file" ) {
			if( argc != 4 )
				return help();

			if(file(argv[2], argv[3]) < 0)
				return help();
		} else if( option == "-index" ) {
			if( argc != 3 )
				return help();

			if(index(argv[2]) <0)
				return help();
		} else {
			return help();
		}
	}

	printf("Success!\n");

	return 0;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");

	return command(argc, argv);
}

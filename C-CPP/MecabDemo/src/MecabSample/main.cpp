
#include <CLucene.h>
#include <tchar.h>
#include <mecab.h>
#include <fstream>
#include <iostream>
#include <sstream>
#if _MSC_VER == 1700
#include <thread>
#include <chrono>
#endif
#include <assert.h>
#include "mecab_parser.h"

#define WINDOWS_EVENT_IN_FILE_PATH "../../bin_debug/we_in.txt"


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
	if(NULL == fp)
		return -1;

	int len = 0;
	while( !feof(fp) ) {
		len += fread(out_buf+len, sizeof(char), out_buf_size, fp);
	}
	fclose(fp);

	out_buf[len] = 0;

	WCHAR wszBuf[40960] = {0};
	MultiByteToWideChar(CP_UTF8, 0, out_buf, len, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);

	return len;
}

int foo() {
	const int buf_size = 40960;
	char out_buf[buf_size];
	test::MecabParser parser;
	
	int len = parser.Parse(MbToUtf8("&나타냄 나타냄 안녕 하하하 abcd 이름이 이름이* 사람이 나타남 볼 볼* 이런 실행 이런*").c_str(), out_buf, buf_size);

	if(len > 0)
		printf("%s\n", out_buf);

	WCHAR wszBuf[40960] = {0};
	MultiByteToWideChar(CP_UTF8, 0, out_buf, len, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringW(L"\n");

	int a = 0;

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

	test::MecabParser parser;

	FILE* fp = fopen(out_file_path, "w");
	if(fp) {
		int size = parser.Parse(buf, out_buf, buf_size);
		if( size > 0 ) {
			fwrite(out_buf, sizeof(char), size, fp);
		}

		size = parser.ParseDetail(buf, out_buf, buf_size);
		if( size > 0 )
			fwrite(out_buf, sizeof(char), size, fp);

		//fwrite(strUnicode, sizeof(wchar_t), nLen, fp);
	} else {
		printf("File open failed, WRITE MODE\n");
	}
	fclose(fp);

	return 0;
}

void printInputMsg() {
	printf("Enter Keyword OR q ==> ");
}

int index( char* in_file_path ) {
	// 입력 파일의 데이터를 줄단위로 색인하고 검색한다.
	lucene::store::RAMDirectory ram;
    //lucene::analysis::SimpleAnalyzer a;
	lucene::analysis::SKBAnalyzer a;
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
	test::MecabParser parser;	// 한글 형태소 분석 객체
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

		wprintf(_T("%s\n"), out_buf);

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

		wstring query_wstr = Utf8ToUnicode(MbToUtf8(query_str));	// 콘솔로 입력받기위한 인코딩 부분
																	// Dataface에서는 의미없음.

		Query* query = query_parser.parse(query_wstr.c_str());
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

#if _MSC_VER == 1700
void thread_foo_func(int thread_no) {
	const int buf_size = 40960;
	char buf[buf_size] = {0,};
    char out_buf[buf_size];
	int read_len = read_file(WINDOWS_EVENT_IN_FILE_PATH, buf, buf_size);
	
	test::MecabParser parser;	

	for(int i=0; i<10000; ++i) {
		int len = parser.Parse(buf, out_buf, buf_size);

		if(len <= 0) {
			printf("N");
			continue;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if(thread_no == 0)
			printf(".");
		else if(thread_no == 1)
			printf("*");
		else if(thread_no == 2) 
			printf("@");
		else
			printf("$");
	}
}
#endif

#if _MSC_VER == 1700
void thread_foo() {
	std::thread t(&thread_foo_func, 0);
	std::thread t1(&thread_foo_func, 1);
	std::thread t2(&thread_foo_func, 2);
	std::thread t3(&thread_foo_func, 3);

	t.join();
	t1.join();
	t2.join();
	t3.join();
}
#endif

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");

	int len = 0;

#if _MSC_VER == 1700
	thread_foo();
#endif

	//foo();
	for(int i=0; i<1000; i++)
	len = file("../../bin_debug/we_in.txt", "../../bin_debug/we_out.txt");

	printf("success\n");

	getchar();
	return 0;

	

	len = index("../../samples/index_in.txt");

	foo();
	
	return 0;
}

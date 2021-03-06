#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <cstring>
#include <climits>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <deque>
#include <algorithm>

#ifdef _MSC_VER 
#include <conio.h>
#endif

// C++11 Standard.?
#include <sstream>
#include <utility>
#include <type_traits> /// for is_class, ...

// C++17 Standard.
#include <variant>

#define ARRAY_QUEUE std::deque // chk?
#define VECTOR std::vector


///
/// GLOBAL_DEBUG
///

namespace wiz {

	#define ENTER '\n' // cf) ENTER

	class _Out {
	private:
		std::string LOG_FILE_NAME = "clautext_log.txt";
		
		long policy = 0; // default 0 - only console, 1 - only file, 2 - file and console.
 	public:
		_Out& operator = (const _Out& other) {
			policy = other.policy;
			return *this;
		}

		template<class T>
		_Out& operator << (const T& data) 
		{
			if (0 == policy || 2 == policy) {
				std::cout << data;
			}
			if (1 == policy || 2 == policy) {
				std::ofstream outFile;
				outFile.open(LOG_FILE_NAME, std::ios::app);
				outFile << data;
				outFile.close();
			}

			return *this;
		}

		void clear_file()
		{
			if (1 == policy || 2 == policy) {
				std::ofstream outFile;
				outFile.open(LOG_FILE_NAME);
				outFile.close();
			}
		}
	public:
		_Out(std::string log_file_name = "clautext_log.txt", long policy = 0) 
			: LOG_FILE_NAME(log_file_name), policy(policy) {
			//
		}

		void SetFileName(const std::string& fileName) {
			LOG_FILE_NAME = fileName;
		}
		void SetPolicy(long policy) {
			this->policy = policy;
		}
	};
	inline _Out Out;
	inline bool USE_REMOVE_IN_DATATYPE = false;
	inline bool USE_EMPTY_VECTOR_IN_LOAD_DATA_TYPES = false;



	// 0~n-2 : sorted, 
	// 0~n-1 : not sorted(maybe?)
	template <class T, class COMPARE>
	void miniInsertSort(std::vector<T>& vec)
	{
		// 0.
		if (vec.size() <= 1) {
			return;
		}

		int idx = vec.size() - 1;
		T last = vec.back();

		// 1. find position;
		for (int i = vec.size() - 2; i >= 0; --i) {
			if (COMPARE()(last, vec[i])) {
				idx = i;
			}
		}
		// 2. shift and assign
		for (int i = vec.size() - 1; i > idx; --i) {
			vec[i] = std::move(vec[i - 1]);
		}
		vec[idx] = std::move(last);
	}

	inline long long GetIdx(long long x) {
		return (x >> 33) & 0x000000007FFFFFFF;
	}
	inline long long GetLength(long long x) {
		return (x & 0x00000001FFFFFFF0) >> 4;
	}
	inline long long GetType(long long x) { //to enum or enum class?
		return (x & 0xE) >> 1;
	}

	class LineInfo {
	public:
		explicit LineInfo() { }
		explicit LineInfo(long long line, long long distance)
			:line(line), distance(distance)
		{
			//
		}
		~LineInfo() {
			//
		}
	public:
		long long line = -1;
		long long distance = -2;

	public:
		LineInfo& operator=(const LineInfo& other) {
			if (other.line == -1) {
				//
			}
			else {
				line = other.line;
				distance = other.distance;
			}
			return *this;
		}
	};


	LineInfo GetLineInfo(long long idx, long long* lines, long long lines_len, long long& start);

	class DataType {
	public:
		int before_pos = -1;
	private:
		std::string str_value;
		mutable long long int_value;
		mutable long double float_value;
		mutable int type = 0;
		mutable bool change = false;
	public:
		LineInfo lineInfo;
	public:
		DataType() { int_value = 0; float_value = 0; }
		DataType(const char* cstr);
		DataType(const char* cstr, size_t len);
		DataType(const std::string& str);

		DataType(const char* cstr, size_t len, const LineInfo& lineOpt);
		DataType(const std::string& str, const LineInfo& lineOpt);

		DataType(std::string&& str);
		virtual ~DataType() {
			//
		}
	public:
		void SetInt(long long val);
		void SetFloat(long double val);
		bool IsInt() const {
			return type == 3;
		}
		bool IsFloat() const {
			return type == 5;
		}
	public:
		int GetType()const;
		std::string GetTypeString()const;
	public:
		bool empty()const {
			return str_value.empty();
		}
	public:
		bool operator==(const DataType& type) const;
		bool operator==(const char* cstr) const;
		bool operator==(const std::string& str) const;
		bool operator==(std::string_view str) const;
		bool operator!=(const DataType& type) const;
		bool operator!=(const char* cstr) const;
		bool operator!=(const std::string& str) const;

		DataType operator+(const DataType& type)const;
		DataType operator+(const char* cstr) const;
		DataType operator+(const std::string& str) const;

		DataType& operator+=(const DataType& type);
		DataType& operator+=(const char* cstr);
		DataType& operator+=(const std::string& str);

	public:
		std::string ToString() const {
			return str_value;
		}
		long long ToInt() const {
			if (change) {
				int_value = std::stoll(str_value);
				change = false;
			}
			return int_value;
		}
		long double ToFloat() const {
			if (change) {
				float_value = std::stold(str_value);
				change = false;
			}
			return float_value;
		}
		friend std::ostream& operator<<(std::ostream& stream, const DataType& x)
		{
			stream << x.ToString();
			return stream;
		}
	};

	bool operator==(const char* cstr, const DataType& type);
	bool operator==(const std::string& str, const DataType& type);
	bool operator==(std::string_view str, const DataType& type);


	bool operator!=(const char* cstr, const DataType& type);
	bool operator!=(const std::string& str, const DataType& type);

	DataType operator+(const char* cstr, const DataType& type);
	DataType operator+(const std::string& str, const DataType& type);
}

using WIZ_STRING_TYPE = wiz::DataType;

namespace wiz {
	inline std::string ToString(WIZ_STRING_TYPE&& x) {
		return x.ToString();
	}
	inline std::string ToString(const WIZ_STRING_TYPE& x) {
		return x.ToString();
		/*
		if (x.index() == 0) {
			return std::get<0>(x);
		}
		else {
			wiz::Token2 temp = std::get<1>(x);

			return std::string(temp.str, temp.len);
		}
		*/
	}

	inline std::string ToString(WIZ_STRING_TYPE& x) {
		return x.ToString();
		/*
		if (x.index() == 0) {
			return std::get<0>(x);
		}
		else {
			wiz::Token2 temp = std::get<1>(x);

			if (UseConvertToken2ToString) {
				x = std::string(temp.str, temp.len);
				return std::get<0>(x);
			}
			else {
				return std::string(temp.str, temp.len);
			}
		}
		*/
	}
	/*
	inline bool operator==(const WIZ_STRING_TYPE& x, const WIZ_STRING_TYPE& y)
	{
		return ToString(x) == ToString(y);
	}
	inline bool operator!=(const WIZ_STRING_TYPE& x, const WIZ_STRING_TYPE& y)
	{
		return ToString(x) != ToString(y);
	}
	inline bool operator<(const WIZ_STRING_TYPE& x, const WIZ_STRING_TYPE& y)
	{
		return ToString(x) < ToString(y);
	}
	*/

	inline int Equal(const std::vector<char>& option, const char ch)
	{
		for (int i = 0; i < option.size(); ++i) {
			if (ch == option[i]) {
				return i;
			}
		}
		return -1;
	}
	inline int Equal(const std::vector<std::string>& option, const char ch)
	{
		for (int i = 0; i < option.size(); ++i) {
			if (option[i].size() == 1 && ch == option[i][0]) {
				return i;
			}
		}
		return -1;
	}
	inline int Equal2(const char option, const char ch)
	{
		if (ch == option) {
			return 0;
		}

		return -1;
	}
}

namespace wiz{
    #define COL_BASED
    #define ROW_BASED

	#define WIZ_IN const
	#define IN
	#define OUT
	#define INOUT

	#define AND &&
	#define OR ||
	#define NOT !
	#define EQUAL ==

	/*
    template <class T>
    inline T max( const T& t1, const T& t2 )
    {
        if( t2 < t1 ) { return t1; }
        return t2;
    }

    template <class T>
    inline T min( const T& t1, const T& t2 )
    {
        if( t1 < t2 ) { return t1; }
        return t2;
    }
	*/
    ///

    template <class T>
    class STD_SWAP
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            std::swap( t1, t2 ); /// if c++11, maybe std::move...or make std::move
        }
    };
    template <class T>
    class NORMAL_SWAP
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            T temp = t1;
            t1 = t2;
            t2 = temp;
        }
    };
    template <class T>
    class HAS_SWAP_METHOD
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            t1.swap( t2 );
        }
    };
    template <class T>
    class WIZ_SWAP
    {
    public:
        void operator()( T& t1, T& t2 )
        {
            std::swap( t1, t2 );
        }
    };
	template < class T, class SWAP=STD_SWAP<T> >
	inline void Swap( T& t1, T& t2 )
	{
	    SWAP()( t1, t2 );
	}

    /// TO DO
    /// std::move, NO_MOVE
    /// ( T& t1, T& t2 )
    /// t1 = t2; // NO_MOVE
    /// t1 = std::move( t2 ); // std::move

	template <class T>
	class ASC {
	public:
		bool operator() (const T& t1, const T& t2) const  { return t1 < t2; }
	};
	template <class T>
	class DSC // DESC
	{
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 > t2; }
	};
	template <class T>
	class EQ { // EE -> EQ!
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 == t2; }
	};
    /// TO DO
    /// ASC_EE, DSC_EE, NOT_EE, EE_SAME_VALUE, NOT_EE_SAME_VALUE , chk red-black tree!!
    template <class T>
	class ASC_EE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 <= t2; }
	};
	template <class T>
	class DSC_EE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 >= t2; }
	};
	template <class T>
	class NOT_EE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 != t2; }
	};
    template <class T>
	class EE_SAME_VALUE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1.isSameValue( t2 ); }
	};
    template <class T>
	class NOT_EE_SAME_VALUE { /// chk..
	public:
		bool operator() (const T& t1, const T& t2) const { return !t1.isSameValue( t2 ); }
	};


    template <class T> /// T is pointer type..
	class PASC {
	public:
		bool operator() (const T t1, const T t2) const  { return *t1 < *t2; }
	};
    template <class T> /// T is pointer type..
    class PDSC // PDESC
    {
    public:
        bool operator() ( const T t1, const T t2 ) const
        {
            return *t1 > *t2;
        }
    };
    template <class T> /// T is pointer type..
	class PEE {
	public:
		bool operator() (const T t1, const T t2) const { return *t1 == *t2; }
	};


	template <typename T> /// T <- char, int, long, long long...
	std::string toStr(const T x, const int base = 10);

	template <class T, class COMP = ASC<T>, class COMP2 = ASC<int>, class EE = EQ<T> > /// 쒖꽌 諛붽씀湲 - 2015.07.18
	class WrapForInfinity
	{
		enum Op{ MIF = 0, GR = 1, IF = 2 };
		COMP comp;
		COMP2 comp2;
		EE ee;
		Op op; // Option
	private:
		explicit WrapForInfinity(const T& val, const Op& op) : val(val), op(op) { }
	public:
		explicit WrapForInfinity(const T& val = T()) : val(val), op(GR) { }
	public:
		T val;
		bool operator>(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
		    return wfi < (*this);
		}
		bool operator<(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
			if (GR == this->op&& GR == wfi.op)
			{
				return comp(this->val, wfi.val);
			}
			else
			{
				return comp2( this->op, wfi.op ); /// chk...
			}
		}
		bool operator==(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
		    if( wfi.op == IF && this->op == IF ) { return true; }
		    if( wfi.op == MIF && this->op == MIF ) { return true; }
			return (ee(wfi.val, this->val) && wfi.op == this->op);
		}
		bool operator!=( const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
		    return !( *this == wfi );
		}
		bool operator<=(const WrapForInfinity<T,COMP,COMP2,EE>& wfi)const
		{
			return *this < wfi ||
				*this == wfi;
		}
		bool operator>=(const WrapForInfinity <T,COMP, COMP2, EE>& wfi)const
		{
			return wfi <= *this;
		}
		WrapForInfinity<T> operator+(const WrapForInfinity<T>& other)const
		{
			if (other.op == this->op) {
				return WrapForInfinity<T>(this->val + other.val, this->op);
			}
			else if (this->op == MIF && other.op == GR) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == IF && other.op == GR) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == GR && other.op == IF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == GR && other.op == MIF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == MIF && other.op == MIF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == IF && other.op == IF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else
			{
				throw std::string(" + Error in wrapforinfiinity");
			}
		}
		WrapForInfinity<T> operator-(const WrapForInfinity<T>& other)const
		{
			if (other.op == this->op && this->op == GR) {
				return WrapForInfinity<T>(this->val - other.val, this->op);
			}
			else if (this->op == MIF && other.op == GR) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == GR && other.op == MIF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == IF && other.op == GR) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else if (this->op == GR && other.op == IF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == MIF && other.op == IF) {
				return WrapForInfinity<T>::GetMinusInfinity();
			}
			else if (this->op == IF && other.op == MIF) {
				return WrapForInfinity<T>::GetInfinity();
			}
			else
			{
				throw std::string(" - Error in wrapforinfiinity");
			}
		}
		std::string toString()const
		{
			if (this->op == MIF) return "minus infinity";
			else if (this->op == IF) return "plus infinity";
			else return wiz::toStr(this->val);
		}

		friend
			std::ostream& operator<<(std::ostream& stream, const WrapForInfinity<T,COMP,COMP2,EE>& wfi)
		{
		    if( wfi.op == MIF ) stream << "minus infinity";
            else if(wfi.op == IF ) stream << "plus infinity";
            else stream << wfi.val;

            return stream;
        }
		static WrapForInfinity<T, COMP, COMP2,EE > GetInfinity()
		{
			return WrapForInfinity<T, COMP, COMP2, EE >(T(), IF);
		}
		static WrapForInfinity<T, COMP, COMP2, EE > GetMinusInfinity()
		{
			return WrapForInfinity<T, COMP, COMP2, EE >(T(), MIF);
		}
	};

	template <typename T> /// x is 10吏꾩닔..
	inline T pos_1(const T x, const int base=10) // 1먮━ 媛怨꾩궛
	{
		if( x >= 0 ) { return x % base; }// x - ( x / 10 ) * 10; }
		else{ return (x / base) * base - x; }
		// -( x - ( (x/10) * 10 ) )
	}
	template <typename T> /// T <- char, int, long, long long...
	std::string toStr(const T x, const int base) /// chk!!
	{
		if( base < 2 || base > 16 ) { return "base is not valid"; }
		T i = x;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 nullptr, 1 minus
		std::string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE+1] = '\0'; ///臾몄옄쒖떆..

		for (k = INT_SIZE; k >= 1; k--){
			T val = pos_1<T>(i, base); /// 0 ~ base-1
			/// number to ['0'~'9'] or ['A'~'F']
			if( val < 10 ) { temp[k] = val + '0'; }
			else { temp[k] = val-10 + 'A'; }

			i /= base;

			if (0 == i){ // レ옄.
				k--;
				break;
			}
		}

		if (isMinus){
			temp[k] = '-';
			tempString = std::string(temp + k);//
		}
		else{
			tempString = std::string(temp + k + 1); //
		}
		delete[] temp;

		return tempString;
	}

    /// chk.... need more thinking..., ToDo...
	template <typename T> /// T <- char, int, long, long long...
	std::string toStr2(const T x, const int base, const int str_space) /// chk!!
	{
	    if( base < 2 || base > 16 ) { return "base is not valid"; }
		T i = x;
		T k2 = 0;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 nullptr, 1 minus
		for(int i=0; i < INT_SIZE+2; ++i ) { temp[i] = '0'; }//
		std::string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE+1] = '\0'; ///臾몄옄쒖떆..

		for (k = INT_SIZE; k >= 1; k--){
            T val = pos_1<T>(i, base); /// 0 ~ base-1
            /// number to ['0'~'9'] or ['A'~'F']
            if( val < 10 ) { temp[k] = val + '0'; }
            else { temp[k] = val-10 + 'A'; }

			i /= base;

			if (0 == i){ // レ옄.
				k--;
				break;
			}
		}

		if (isMinus){
			temp[k] = '-';
			tempString = std::string(temp + k);//
		}
		else{
            if( INT_SIZE+1 - (k+1) +1 < str_space+1 )
            {
                k2 = str_space+1 - ( INT_SIZE+1 - (k+1) + 1 );
            }
            else
            {
                k2 = 0;
            }
			tempString = std::string(temp + k + 1 - k2 ); //
		}
		delete[] temp;

		return tempString;
	}

	inline std::string str(const int x)
	{
		return toStr<int>(x);
	}

	template <typename T> 
	inline std::string _toString(const T x)
	{
		std::stringstream strs;
		strs << x;
		return strs.str();
	}
	template <>
	inline std::string _toString(const long double x)
	{
		std::stringstream strs;
		strs << std::fixed << x; // 나타낼수있는자리수 - 1 ?
		std::string temp = strs.str();
		std::size_t idx = temp.find('.');
		if (idx == temp.size()-1) {
			temp.push_back('0');
		}
		else if (idx == std::string::npos) {
			temp.push_back('.');
			temp.push_back('0');
		}
		return temp;
	}

	template <>
	inline std::string _toString(const long long x)
	{
		return toStr(x);
	}
	template <>
	inline std::string _toString(const int x)
	{
		return str(x);
	}
    template <>
	inline std::string _toString(const bool x)
	{
		if (x) { return "true"; }
		return "false";
	}

    // following remove
    inline bool isWhitespace( const char ch )
    {
		switch (ch)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\v':
		case '\f':
			return true;
			break;
		}
		return false;
	}

    template <typename T> // T must be unsigned type!
    inline T Unsigned_Maximum()
    {
        return -1;
    }

    template <typename T> // T must be signed type!
    T Signed_Maximum()
    {
        size_t byteSize = sizeof(T);
		T val = 0;

        val = 0x7F;
		for (size_t i = 1; i < byteSize; i++)
		{
			val = val << 8; // left shift 1 byte

			val = val | 0xFF;
		}
		return val;
    }
    // C++11
    template <typename T>
    inline T Maximum()
    {
        if( std::is_signed<T>::value )
        {
            return Signed_Maximum<T>();
        }
        if( std::is_unsigned<T>::value )
        {
            return Unsigned_Maximum<T>();
        }
        throw std::string( "unsupport type : double, class, and etc..." );
    }
	inline int getFirstIndex(const std::string& str, const char ch)
	{
		for (int i = 0; i < str.size(); ++i)
		{
			if (ch == str[i]) { return i; }
		}
		return -1;
	}
	class wizObject
	{
	private:
	public:
		wizObject() { }
		wizObject(const wizObject& object) { }

		virtual ~wizObject() { }

		virtual std::string toString()const = 0;
		//virtual void initial( const wizObject& object )=0;
		virtual wizObject& operator=(const wizObject& object) { return *this; }
		virtual wizObject* clone()const = 0;
	};

	template <typename T>
	T Rand() // T : maybe int, long, long long,.., ÀüÃ¼ ¹üÀ§ Rand...
	{
		size_t byteSize = sizeof(T);
		T val = 0;

		for (size_t i = 0; i < byteSize; i++)
		{
			val = val << 8; // left shift 1 byte

			val = val | (rand() % 256);
		}
		return val;
	}
	template <typename T>
	inline T Rand2() /// T : must be signed! and no return minus value.
	{
		T val = Rand<T>();
		return val & Signed_Maximum<T>();
	}

	long long checkDelimiter(const char* start, const char* last, const std::vector<std::string>& delimiter);

	std::vector<std::string> tokenize(std::string sv, char ch);
}



#define NOMINMAX // use std::min, std::max


//
#ifdef __linux__
#include <termios.h>
#include <stdio.h>

static struct termios old, _new;

/* Initialize new terminal i/o settings */
inline void initTermios(int echo)
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	_new = old; /* make new settings same as old settings */
	_new.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo) {
		_new.c_lflag |= ECHO; /* set echo mode */
	}
	else {
		_new.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &_new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
inline void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
inline char linux_getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
inline char linux_getch(void)
{
	return linux_getch_(0);
}
#endif

inline int _GETCH()
{
#ifdef _MSC_VER 
	return _getch();
#else
	#ifdef __linux__
		return linux_getch();
	#else
		return std::cin.get();
	#endif
#endif
}

inline int GETCH()
{
	int x = _GETCH();

#ifdef _MSC_VER 
	if (x == '\0' || x == 0xE0) { // cf) same? in linux??
		x = _GETCH();
	}
#endif
	return x;
}
#ifdef _MSC_VER

#include <Windows.h>

inline void FFLUSH()
{
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');
}

inline void gotoxy(short x, short y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

inline void setcolor(int color, int bgcolor)
{
	color = color & 0xF;
	bgcolor = bgcolor & 0xF;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		(bgcolor << 4) | color);
}
#endif




#endif

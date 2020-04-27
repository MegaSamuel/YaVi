#include <limits.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <sys/stat.h>

#ifdef QT_CORE_LIB
#include <QtCore>
#include <QString>
#include <QDebug>
#endif

#include "goods.h"

using namespace cobu;

//------------------------------------------------------------------------------

// Устраняет необходимость использования <boost/algorithm/string.hpp>
template<typename SequenceSequenceT, typename CharT>
SequenceSequenceT&  split( SequenceSequenceT& result, const std::string& input, CharT delim )
{
    std::stringstream ss( input );
    std::string item;
    while( std::getline( ss, item, delim ) ) {
        result.push_back( item );
    }
    return result;
}

// Устраняет необходимость использования <boost/algorithm/string/trim.hpp>

// Удаляет пробелы в начале строки
static inline void ltrim( std::string& s ) {
    s.erase( s.begin(), std::find_if( s.begin(), s.end(), []( int ch ) {
        return !std::isspace( ch );
    } ) );
}

// Удаляет пробелы в конце строки
static inline void rtrim( std::string& s ) {
    s.erase( std::find_if( s.rbegin(), s.rend(), []( int ch ) {
        return !std::isspace( ch );
    } ).base(), s.end() );
}

// Удаляет пробелы в начале и в конце строки
static inline void trim( std::string& s ) {
    ltrim( s );
    rtrim( s );
}

inline bool  filIsRegular( const std::string&  filename )
{
    struct stat 	buffer;

    if( 0 != stat( filename.c_str(), &buffer ) )
        return false;
    return ( 0 != S_ISREG( buffer.st_mode ) );
}

class cobu::TGoodsFieldPrivate
{
    class TGoodsDataWord
    {
    public:
        TGoodsDataWord( unsigned a_uBeginWord, unsigned a_uOffset, unsigned a_uBits, bool a_bSecondVector = false )
            : m_uBeginWord( a_uBeginWord )
            , m_uOffset( a_uOffset )
            , m_uBits( a_uBits )
            , m_bSecondVector( a_bSecondVector )
        {}

        inline unsigned    beginWord() const noexcept { return m_uBeginWord; }
        inline unsigned    offset() const noexcept { return m_uOffset; }
        inline unsigned    bits() const noexcept { return m_uBits; }

        // true, если слово относится ко второму вектору параметра (только для параметров, хранящихся в двух векторах)
        inline bool        secondVector() const noexcept { return m_bSecondVector; }

    private:
        unsigned    m_uBeginWord;
        unsigned    m_uOffset;
        unsigned    m_uBits;
        bool        m_bSecondVector;
    };

    friend class TGoodsField;
private:
    // статические таблицы, используемые для проверки соответствий
    // типы записей, к которым применим тип bool
    static std::vector<TGoodsField::EType> 				s_tTypesForBool;
    // типы записей, к которым применим тип int
    static std::vector<TGoodsField::EType> 				s_tTypesForInt;
    // типы записей, к которым применим тип unsigned
    static std::vector<TGoodsField::EType> 				s_tTypesForUInt;
    // типы записей, к которым применим тип text
    static std::vector<TGoodsField::EType> 				s_tTypesForText;
    // типы записей, к которым применим тип double
    static std::vector<TGoodsField::EType> 				s_tTypesForDouble;
    // типы записей, к которым применимы min и max
    static std::vector<TGoodsField::EType> 				s_tTypesForBound;

    TGoodsField			*ins__; 	// pointer to corresponding protocol field instance

    std::string 			m_zId;
    std::string 			m_zBrief;
    TGoodsField::EType 	m_eType;
    TGoodsField::ETextEncoding   m_eEncoding;

    unsigned 				m_uRoute;
    size_t 					m_uOrderNumber;

    unsigned 	m_uDataSize; 	// размер одного элемента данных (слово - 16 бит) в битах

    unsigned 	m_uBits;
    int 	 	m_nSign; 	// order of sign bit, 0 if alt-code, -1 if unsigned, -2 if RTM1495v2 matrix for sign
    // TODO по хорошему, для увеличения точности, вместо одного коэффициента надо ввести два, как в конфиге
    double 		m_fULOB;	// единица младшего разряда

    double 		m_fMin;
    double 		m_fMax;
    unsigned 	m_uPrecision;
    bool 		m_bHidden;
    bool        m_bLogic;
    bool        m_bDualVector;

    std::vector<TGoodsDataWord>  m_vDataWords;
    std::string                     m_zSecondVectorId;

    std::string                 m_zUnit;
    std::vector<std::string>    m_vValues;
    std::vector<std::string>    m_vExpandValues;

    inline void 	clear() noexcept
    {
        m_uDataSize = 16;
        m_zId.clear();
        m_zBrief.clear();
        m_zUnit.clear();
        m_vValues.clear();
        m_vExpandValues.clear();
        m_bHidden = false;
        m_bLogic = false;
        m_bDualVector = false;
        m_uRoute = 0;
    }

    inline TGoodsFieldPrivate( TGoodsField  *ins ) :
        ins__( ins ),
        m_uDataSize( 16 )
    {
        clear();
        (void)ins__;
    }
};

// TODO sequence of static initializers
std::vector<TGoodsField::EType> 	TGoodsFieldPrivate::s_tTypesForBool =
{
    TGoodsField::Bool, TGoodsField::Integer, TGoodsField::Unsigned, TGoodsField::Hex, TGoodsField::Combo
};
std::vector<TGoodsField::EType> 	TGoodsFieldPrivate::s_tTypesForInt =
{
    TGoodsField::Bool, TGoodsField::Integer, TGoodsField::Unsigned, TGoodsField::Hex, TGoodsField::Combo
};
std::vector<TGoodsField::EType> 	TGoodsFieldPrivate::s_tTypesForUInt =
{
    TGoodsField::Bool, TGoodsField::Integer, TGoodsField::Unsigned, TGoodsField::Hex, TGoodsField::Combo
};
std::vector<TGoodsField::EType> 	TGoodsFieldPrivate::s_tTypesForText =
{
    TGoodsField::Combo, TGoodsField::Text
};
std::vector<TGoodsField::EType> 	TGoodsFieldPrivate::s_tTypesForDouble =
{
    TGoodsField::Real, TGoodsField::Double, TGoodsField::Float
};
std::vector<TGoodsField::EType> 	TGoodsFieldPrivate::s_tTypesForBound =
{
    TGoodsField::Integer, TGoodsField::Unsigned, TGoodsField::Real, TGoodsField::Double, TGoodsField::Float
};

static bool __yaml_IsScalar( const YAML::Node&  node )
{
	return ( node.IsDefined() ? ( node.IsScalar() ? true : false ) : false );
}

static bool __yaml_IsSequence( const YAML::Node&  node )
{
	return ( node.IsDefined() ? ( node.IsSequence() ? true : false ) : false );
}

static bool __yaml_IsMap( const YAML::Node&  node )
{
	return ( node.IsDefined() ? ( node.IsMap() ? true : false ) : false );
}

static const std::string __yaml_GetString( const YAML::Node&  node, const std::string&  name, const std::string  def = "" )
{
	if( __yaml_IsScalar( node[ name ] ) )
		return node[ name ].as<std::string>();
	return def;
}

static uint64_t  __filled_value( unsigned  bits )
{
    if( 64 == bits )
        return 0xFFFFFFFFFFFFFFFFULL;
    return ( ( 1ULL << bits ) - 1ULL );
}

unsigned TGoodsField::yaml_GetUnsigned( const YAML::Node&  node, const std::string&  name, unsigned  def )
{
	if( __yaml_IsScalar( node[ name ] ) )
    {
        try {
            return node[ name ].as<unsigned>();
        } catch(...) {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Bad Protocol Yaml unsigned conversion for " << name << "\n";
#endif
            return def;
        }
    }

	return def;
}

double TGoodsField::yaml_GetDouble( const YAML::Node&  node, const std::string&  name, double  def )
{
	if( __yaml_IsScalar( node[ name ] ) )
    {
        try {
            return node[ name ].as<double>();
        } catch (...) {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Bad Protocol Yaml double conversion for " << name << "\n";
#endif
            return def;
        }
    }
	return def;
}

unsigned TGoodsField::yaml_GetAltUnsigned( const YAML::Node&  node, const std::string&  main, const std::string&  alt, unsigned  def )
{
	if( __yaml_IsScalar( node[ main ] ) )
    {
        try {
            return node[ main ].as<unsigned>();
        } catch( ... )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Bad Protocol Yaml unsigned conversion for main " << main << "\n";
#endif
        }
    }
	if( __yaml_IsScalar( node[ alt ] ) )
    {
        try {
            return node[ alt ].as<unsigned>();
        } catch( ... )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Bad Protocol Yaml unsigned conversion for alt " << alt << "\n";
#endif
        }
    }
	return def;
}

//------------------------------------------------------------------------------

bool TGoodsField::parse_yaml( const YAML::Node&  node )
{
    // таблица соответствия имени типа при чтении из файла -> сам тип записи протокола
    std::map<std::string,TGoodsField::EType> 	svTypes =
    {
        { "bool", 	TGoodsField::Bool },
        { "integer",	TGoodsField::Integer },
        { "unsigned", 	TGoodsField::Unsigned },
        { "hex", 	TGoodsField::Hex },
        { "combo", 	TGoodsField::Combo },
        { "real", 	TGoodsField::Real },
        { "double", 	TGoodsField::Double },
        { "float", 	TGoodsField::Float },
        { "text", 	TGoodsField::Text },
        // compatibility
        { "STRING", 		TGoodsField::Text },
        { "INT32", 		TGoodsField::Integer },
        { "DOUBLE", 		TGoodsField::Real },
        { "DOUBLESPINBOX", 	TGoodsField::Real },
        { "SPINBOX", 		TGoodsField::Integer },
        { "HEXSPINBOX", 	TGoodsField::Hex },
        { "COMBOBOX", 		TGoodsField::Combo },
        { "VALUESLIDER", 	TGoodsField::Combo },
        { "TEXT", 		TGoodsField::Text },
    };
    // таблица соответствия имени кодировки текстового поля из файла -> сама кодировка
    std::map<std::string,TGoodsField::ETextEncoding> 	svTextEncodings =
    {
        { "Ascii",          TGoodsField::EncodingAscii },
        { "Windows1251",    TGoodsField::EncodingWindows1251 },
        { "Windows1252",    TGoodsField::EncodingWindows1252 },
        { "Koi7",           TGoodsField::EncodingKoi7 },
    };
    // check for require
	static 	std::vector<std::string> 	satPrtclRequired = 
	{
		"id", "name", "type"
	};
	for( auto it : satPrtclRequired )
	{
		if( ! __yaml_IsScalar( node[ it ] ) )
			return false;
	}

	// paranoia for try-catch
	try
	{
		priv__->m_zId = node[ "id" ].as<std::string>();
		priv__->m_zBrief = node[ "name" ].as<std::string>();
		std::string 	zType = node[ "type" ].as<std::string>();
		priv__->m_eType = svTypes[ zType ];

		if( Undefined == priv__->m_eType )
		{
			std::cerr << priv__->m_zId << " Cannot set field type, read " << zType << "\n";
			return false;
		}
	} catch( const YAML::Exception&  e )
	{
		std::cerr << "Error exception " << __FILE__ << " : " << __LINE__ << " what() " << e.what() << "\n";
		return false;
	}

    // check for hidden
    if( __yaml_IsScalar( node[ "hidden" ] ) )
        priv__->m_bHidden = node[ "hidden" ].as<bool>();

    // check for logic
    if( __yaml_IsScalar( node[ "logic" ] ) )
        priv__->m_bLogic = node[ "logic" ].as<bool>();

    // check for second vector
    if( __yaml_IsScalar( node[ "secondVector" ] ) )
    {
        priv__->m_zSecondVectorId = node[ "secondVector" ].as<std::string>();
        priv__->m_bDualVector = true;

        if( priv__->m_zSecondVectorId.empty() )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Read empty second vector id\n";
#endif
        }
    }

    // читаем route если он есть
    if( __yaml_IsScalar( node[ "route" ] ) )
        priv__->m_uRoute = node[ "route" ].as<unsigned>();

	// TODO not the object
    YAML::Node  	node_data;

	if( __yaml_IsSequence( node[ "dataWords" ] ) )
		node_data = node[ "dataWords" ];
	else if( __yaml_IsSequence( node[ "data" ] ) )
		node_data = node[ "data" ];
	else
    {
#ifdef __DEBUG__
        std::cerr << priv__->m_zId << " Cannot read dataWords or data fields\n";
#endif
        return false;
    }
    if( ! node_data.size() )
    {
#ifdef __DEBUG__
        std::cerr << priv__->m_zId << " Read empty dataWords or data fields\n";
#endif
        return false;
    }

    priv__->m_uBits = 0;
    for( unsigned i = 0; i < node_data.size(); i++ )
    {
        unsigned    uBeginWord = yaml_GetAltUnsigned( node_data[i], "dataWord", "word", 0 );
        unsigned    uOffset    = yaml_GetAltUnsigned( node_data[i], "bias", "offset", 0 );
        unsigned    uBits      = yaml_GetAltUnsigned( node_data[i], "numberOfDigits", "bits", 0 );
        // ругаемся при отстутствии числа бит
        if( ( ! __yaml_IsScalar( node_data[i][ "numberOfDigits" ] ) ) && ( ! __yaml_IsScalar( node_data[i][ "bits" ] ) ) )
            std::cerr << priv__->m_zId << " No 'numberOfDigits' and 'bits' param, set default value " << uBits << "\n";

        // проверяем, не относится ли слово к другому вектору
        unsigned    uSecondVector = false;
        if( __yaml_IsScalar( node_data[i][ "secondVector" ] ) )
        {
            uSecondVector = node_data[i][ "secondVector" ].as<bool>();
        }

        priv__->m_vDataWords.emplace_back( uBeginWord, uOffset, uBits, uSecondVector );
        priv__->m_uBits += uBits;
    }

    // ограничение: для всех типов, кроме 'Text' максимальная ширина поля - 64 бита.
    if( Text != priv__->m_eType )
        priv__->m_uBits = std::min( priv__->m_uBits, static_cast<unsigned>( 64 ) );

	// знак
    priv__->m_nSign = -1;
    if( ( Integer == priv__->m_eType ) || ( Real == priv__->m_eType ) )
	{
            priv__->m_nSign = 0;

            std::string 	sign = __yaml_GetString( node, "sign" );
            if( ! sign.empty() )
            {
                if( 'u' == std::tolower( sign.front() ) )
                    priv__->m_nSign = -1;
                else if( "matrix" == sign )
                    priv__->m_nSign = -2;
                else if( "rtmv2" == sign )
                    priv__->m_nSign = -2;
                else
                {
                    try
                    {
                        priv__->m_nSign = stoi( sign );
                    } catch( ... )
                    {
                        priv__->m_nSign = 0;
                    }
                }
            }
	}
    if( ( Unsigned == priv__->m_eType ) || ( Hex == priv__->m_eType ) || ( Bool == priv__->m_eType ) )
	    priv__->m_nSign = -1;

	// unit or values
    priv__->m_zUnit = __yaml_GetString( node, "unit" );

    if( Combo == priv__->m_eType )
	{
		// values list
		if( __yaml_IsScalar( node[ "values" ] ) )
		{
			const std::string 	val = node[ "values" ].as<std::string>();
            split( priv__->m_vValues, val, '\n' );
		}
		else  	// fill for numbers
		{
            unsigned  	uMax = static_cast<unsigned>( ( 1ULL << priv__->m_uBits ) - 1ULL );

            uMax = yaml_GetUnsigned( node, "max", static_cast<unsigned>( __filled_value( priv__->m_uBits ) ) );

            priv__->m_vValues.reserve( uMax + 1 );
            unsigned 	counter = 0;
            std::generate_n( std::back_inserter( priv__->m_vValues ), uMax+1, [&] { return std::to_string( counter++ ); } );
		}
	}
    if( Text == priv__->m_eType )
    {
        priv__->m_fMin = 0;
        priv__->m_fMax = std::floor( priv__->m_uBits / 8 );

        if( __yaml_IsScalar( node[ "values" ] ) )
            priv__->m_vValues.push_back( node[ "values" ].as<std::string>() );

        // Encoding
        try
        {
            if( node[ "encoding" ] )
            {
                std::string 	zEncoding = node[ "encoding" ].as<std::string>();
                priv__->m_eEncoding = svTextEncodings[ zEncoding ];
            }
        } catch( const YAML::Exception&  e )
        {
            std::cerr << "Error exception " << __FILE__ << " : " << __LINE__ << " what() " << e.what() << "\n";
            return false;
        }
    }

    uint64_t    max_value = 0;
    if( -1 == priv__->m_nSign )
        max_value = __filled_value( priv__->m_uBits );
    else if( -2 == priv__->m_nSign )  // Знак в матрице состояния Arinc
        max_value = __filled_value( priv__->m_uBits - 2 );
    else
        max_value = __filled_value( priv__->m_uBits - 1 );

    // единица младшего разряда
    priv__->m_fULOB = 1.;
    if( Real == priv__->m_eType )
    {
	    if( __yaml_IsScalar( node[ "orderBit" ] ) && __yaml_IsScalar( node[ "valueOfOrderBit" ] ) )
	    {
		    unsigned 	order_bit = yaml_GetUnsigned( node, "orderBit" );
		    double 		order_bit_value = yaml_GetDouble( node, "valueOfOrderBit" );

		    priv__->m_fULOB = order_bit_value / static_cast<double>( 1ULL << order_bit );
	    }
	    else if( __yaml_IsScalar( node[ "valueOfLowOrderBit" ] ) )
		    priv__->m_fULOB = yaml_GetDouble( node, "valueOfLowOrderBit" );

        max_value = static_cast<uint64_t>(priv__->m_fULOB * max_value);
    }

    if( -1 == priv__->m_nSign )
    {
        priv__->m_fMin = 0;
        priv__->m_fMax = max_value;
    }
    else if( ( 0 == priv__->m_nSign ) || ( -2 == priv__->m_nSign ) ) // Дополнительный код
    {
        priv__->m_fMin = -1.0 * max_value - 1.0;
        priv__->m_fMax = max_value;
    }
    else    // Прямой код
    {
        priv__->m_fMin = -1.0 * max_value;
        priv__->m_fMax = max_value;
    }

    priv__->m_fMin = yaml_GetDouble( node, "min", priv__->m_fMin );
    priv__->m_fMax = yaml_GetDouble( node, "max", priv__->m_fMax );
    priv__->m_uPrecision = yaml_GetUnsigned( node, "precision", 2 );

    // if no min or max params for items
    if( ( std::find( priv__->s_tTypesForBound.begin(), priv__->s_tTypesForBound.end(), priv__->m_eType ) != priv__->s_tTypesForBound.end() ) && ( ! priv__->m_bHidden ) )
    {
#ifdef ProtocolParamDebug
        if( ! __yaml_IsScalar( node[ "min" ] ) )
		    std::cerr << priv__->m_zId << " No 'min' param, set default value " << priv__->m_fMin << "\n"; 
	    if( ! __yaml_IsScalar( node[ "max" ] ) )
		    std::cerr << priv__->m_zId << " No 'max' param, set default value " << priv__->m_fMax << "\n"; 
#endif
    }
    if( Hex == priv__->m_eType )
    {
	    if( 2 > priv__->m_uBits )
		    std::cerr << priv__->m_zId << " hex type has only " << priv__->m_uBits << " bits\n";
    }

	return true;
}

TGoodsField::TGoodsField()
{
    priv__ = std::unique_ptr<cobu::TGoodsFieldPrivate>( new cobu::TGoodsFieldPrivate( this ) );
	clear();
}

TGoodsField::TGoodsField( const YAML::Node&  node )
{
    priv__ = std::unique_ptr<cobu::TGoodsFieldPrivate>( new cobu::TGoodsFieldPrivate( this ) );
    clear();
    if( ! parse_yaml( node ) )
		clear();
}

void TGoodsField::clear() noexcept
{
    priv__->clear();
}

TGoodsField::~TGoodsField()
{
	clear();
}

bool TGoodsField::empty() const noexcept
{
    return priv__->m_zId.empty();
}

TGoodsField::EType 	TGoodsField::type() const noexcept
{
    return priv__->m_eType;
}

TGoodsField::ETextEncoding 	TGoodsField::encoding() const noexcept
{
    return priv__->m_eEncoding;
}

const std::string &TGoodsField::id() const noexcept
{
    return priv__->m_zId;
}

const std::string &TGoodsField::name() const noexcept
{
    return priv__->m_zBrief;
}

const std::string &TGoodsField::unit() const noexcept
{
    return priv__->m_zUnit;
}

// округление числа до нужного количества знаков после запятой
static double  __round_to( double val, int  precision )
{
	double div = 1.0;
	if( 0 <= precision )
	{
		while( precision-- )
			div *= 10.0;
	}
	else
	{
		while( precision++ )
			div /= 10.0;
	}
	return floor( val * div + 0.5 ) / div;
}

double TGoodsField::minimum() const noexcept
{
    return __round_to( priv__->m_fMin, static_cast<int>(priv__->m_uPrecision) );
}

double TGoodsField::maximum() const noexcept
{
    return __round_to( priv__->m_fMax, static_cast<int>(priv__->m_uPrecision) );
}

double 	TGoodsField::medium() const noexcept
{
    if( Hex == priv__->m_eType )
        return static_cast<double>( ( 1 << ( priv__->m_uBits/2 ) ) );

    //uint64_t 		uCount = static_cast<uint64_t>( std::round( ( priv__->m_fMax - priv__->m_fMin )/priv__->m_fULOB ) );
    int64_t 		nCountUp = static_cast<int64_t>( std::round( priv__->m_fMax/priv__->m_fULOB ) );
    int64_t 		nCountDown = static_cast<int64_t>( std::round( priv__->m_fMin/priv__->m_fULOB ) );
    //uint64_t 		uMediumCount = uCount/2.0;
    //double 		fMedium = priv__->m_fMin + uMediumCount*priv__->m_fULOB;
    double 		fMedium = ( nCountUp/2 )*priv__->m_fULOB + ( nCountDown/2 )*priv__->m_fULOB;

    if( ( Unsigned == priv__->m_eType ) || ( Integer == priv__->m_eType ) )
        return std::round( fMedium );

    //std::cerr << "count " << uCount << " medium count " << uMediumCount << " medium " << fMedium << " ulob " << priv__->m_fULOB << " val " <<  __round_to( fMedium, priv__->m_uPrecision ) << "\n";    
    return __round_to( fMedium, static_cast<int>(priv__->m_uPrecision) );
}

unsigned TGoodsField::precision() const noexcept
{
    return priv__->m_uPrecision;
}

bool TGoodsField::hidden() const noexcept
{
    return priv__->m_bHidden;
}

bool TGoodsField::logic() const noexcept
{
    return priv__->m_bLogic;
}

bool TGoodsField::dualVector() const noexcept
{
    return priv__->m_bDualVector;
}

const std::string& TGoodsField::secondVectorName() const noexcept
{
    return priv__->m_zSecondVectorId;
}


unsigned  TGoodsField::bits() const noexcept
{
	return priv__->m_uBits;
}

const std::vector<std::string>& TGoodsField::values() const noexcept
{
    return priv__->m_vValues;
}

const std::vector<std::string>& TGoodsField::expand_values() const noexcept
{
    unsigned long 	cnt = 0, cnt_total;
    std::string 	str;

    // вектор с исключениями
    std::vector<std::string> vctExclStr =
    {
        "«»", "\"\""
    };

    // очищаем вектор, иначе получим дублирующие записи
    priv__->m_vExpandValues.clear();

    // количество записей в combobox
    cnt_total = priv__->m_vValues.size();

    for( auto&  it : priv__->m_vValues )
    {
        bool bDirect = false;

        if( it.empty() )
        {
            bDirect = true;
        }
        else
        {
            size_t index;

            str = it.c_str();

            // удаляем все пробелы из строки
            while( ( index = str.find(' ') ) != std::string::npos )
                str.erase( index, 1 );

            // прогоняем получившуюся строку по массиву исключений
            bDirect = ( std::find( vctExclStr.begin(), vctExclStr.end(), str ) != vctExclStr.end() );
        }
#if 1   // XXX this is must be comment for show the items number always
        if( bDirect )
        {
            // значения у поля нет - выводим как есть
            priv__->m_vExpandValues.push_back( it.c_str() );
        }
        else
#endif
        {
            // число выводимых нулей
            int zeroes = static_cast<int>( log10( cnt_total-1 ? cnt_total-1 : 1 ) ) - static_cast<int>( log10( cnt ? cnt : 1 ) );

            // заполняем строку с учетом числа выводимых нулей
            str.clear();
            while( zeroes )
            {
                str.append( "0" );
                zeroes--;
            }
            str.append( std::to_string( cnt ) );
            str.append( " - " );
            str.append( it.c_str() );

            priv__->m_vExpandValues.push_back( str );
        }

        // счетчик
        cnt++;
    }

    return priv__->m_vExpandValues;
}

//------------------------------------------------------------------------------

static bool __protocol_check_size( size_t  word, size_t  size, std::string  id = "" )
{
    if( word >= ( size / 2 ) )
#ifdef __DEBUG__
        std::cerr << id << " Bad check word and size, word " << word << " and size " << size << "\n";
#else
        (void)id; 	// unused if not debug
#endif
    return ( word < ( size / 2 ) );
}

uint64_t TGoodsField::get_unsigned_value( const uint16_t  *data_0, size_t  size_0,
                                             const uint16_t  *data_1, size_t  size_1 ) const noexcept
{
    if( ( ! data_0 ) || ( dualVector() && ( ! data_1 ) ) )
    {
#ifdef __DEBUG__
        std::cerr << "Zero data pointer for field " << id() << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
        return 0;
    }

    unsigned    bits_total = 0;
    uint64_t    val = 0;

    for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
    {
        unsigned        word   = it->beginWord();
        unsigned        bits   = it->bits();
        unsigned        offset = it->offset();
        const uint16_t *data = nullptr;
        size_t          size = 0;
        if( ! it->secondVector() )
        {
            data = data_0;
            size = size_0;
        }
        else
        {
            data = data_1;
            size = size_1;
        }

        // Слово интерфейса может занимать больше одного слова в векторах данных.
        // Если сдвиг больше 15 бит, то сразу переходим в следующее слово вектора данных.
        while( 15 < offset )
        {
            word++;
            offset -= 16;
        }
        while( 0 != bits )
        {
            if( ! __protocol_check_size( word, size, id() ) )
                break;

            unsigned    bits_in_word = std::min( bits, 16U - offset );
            unsigned    mask = __filled_value( bits_in_word );
            uint64_t    val_in_word = ( data[ word ] >> offset ) & mask;

            val = val | ( static_cast<uint64_t>( val_in_word ) << bits_total );
            bits       -= bits_in_word;
            bits_total += bits_in_word;

            // Переходим в следующее слово вектора и обнуляем offset на случай, если интерфейсное слово
            // занимает больше одного слова в векторах
            word++;
            offset = 0;
        }
    }

    return  val;
}

int64_t  TGoodsField::get_int_value( const uint16_t  *data_0, size_t  size_0,
                                        const uint16_t  *data_1, size_t  size_1 ) const noexcept
{
    uint64_t  	uval = get_unsigned_value( data_0, size_0, data_1, size_1 );
    int64_t 	val = 0;

    // дополнительный код
    if( ! priv__->m_nSign )
    {
        // смотрим старший бит
        if( uval & ( 1ULL << ( priv__->m_uBits-1ULL ) ) )
            val = -( ( ( 1ULL << priv__->m_uBits ) - 1ULL ) - uval + 1ULL ); 	// отрицательное число как дополнение до 1
        else
            val = static_cast<int64_t>(uval);
    }
    else if( -1 == priv__->m_nSign ) 	// unsigned
        val = static_cast<int64_t>(uval);
    else if( -2 == priv__->m_nSign ) 	// RTM v2
    {
        // почти как дополнительный код, но предпоследний байт не смотрим
        // смотрим два старших бита
        switch( ( uval >> ( priv__->m_uBits-2ULL ) ) & 0x3ULL )
        {
        case 0 : // нормальное положительное число
            val = static_cast<int64_t>(uval); break;
        case 1 : // отказ параметра или контрольное значение - обнуляем биты маски и выдаем как положительное число
            val = static_cast<int64_t>( uval & ~( 3ULL << ( priv__->m_uBits-2ULL ) ) ); break;
        case 2 : // информация недостоверна  - обнуляем биты маски и выдаем как положительное число
            val = static_cast<int64_t>( uval & ~( 3ULL << ( priv__->m_uBits-2ULL ) ) ); break;
        case 3 : // нормальное отрицательное число
            val = -( ( ( 1ULL << priv__->m_uBits ) - 1ULL ) - uval + 1ULL ); break;	// отрицательное число как дополнение до 1
        }
    }
    else // прямой код
    {
        // знаковый разряд
        if( uval & ( 1ULL << priv__->m_nSign ) )
            val = -( uval & ~( 1ULL << priv__->m_nSign ) );
        else
            val = static_cast<int64_t>(uval);
    }

    return val;
}

double  TGoodsField::get_real_value( const uint16_t  *data_0, size_t  size_0,
                                        const uint16_t  *data_1, size_t  size_1 ) const noexcept
{
    // беззнака
    if( -1 == priv__->m_nSign )
    {
        uint64_t 	uval = get_unsigned_value( data_0, size_0, data_1, size_1 );
        return uval * priv__->m_fULOB;
    }
    else
    {
        int64_t 	sval = get_int_value( data_0, size_0, data_1, size_1 );
        return sval * priv__->m_fULOB;
    }
}

//------------------------------------------------------------------------------

void TGoodsField::set_unsigned_value( uint64_t  value, uint16_t  *data_0, size_t  size_0,
                                                          uint16_t  *data_1, size_t  size_1 ) const noexcept
{
    if( ( ! data_0 ) || ( dualVector() && ( ! data_1 ) ) )
    {
#ifdef __DEBUG__
        std::cerr << "Zero data pointer for field " << id() << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
        return;
    }

    unsigned 	bits_total = priv__->m_uBits;

    // ограничение на максимально возможное значение по битам
    uint64_t 	max_value = __filled_value( bits_total );

    if( value > max_value )
    {
        value = max_value;
    }

    for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
    {
        unsigned    word   = it->beginWord();    // слово в векторах данных
        unsigned    bits   = it->bits();
        unsigned    offset = it->offset();
        uint16_t   *data = nullptr;
        size_t      size = 0;
        if( ! it->secondVector() )
        {
            data = data_0;
            size = size_0;
        }
        else
        {
            data = data_1;
            size = size_1;
        }

        // Слово интерфейса может занимать больше одного слова в векторах данных.
        // Если сдвиг больше 15 бит, то сразу переходим в следующее слово вектора данных.
        while( 15 < offset )
        {
            word++;
            offset -= 16;
        }
        while( 0 != bits )
        {
            if( ! __protocol_check_size( word, size, id() ) )
                break;

            unsigned    bits_in_word = std::min( bits, 16U - offset );
            unsigned    mask_in_word = ( ( 1U << bits_in_word ) - 1U ) << offset;
            unsigned    val_in_word = ( ( value & 0xFFFF ) << offset ) & mask_in_word;

            data[ word ] = ( data[ word ] & ~mask_in_word ) | static_cast<uint16_t>( val_in_word );
            // Переходим в следующее слово вектора и обнуляем offset на случай, если интерфейсное слово
            // занимает больше одного слова в векторах
            word++;
            offset = 0;
            value >>= bits_in_word;
            bits -= bits_in_word;
        }
    }
}

void TGoodsField::set_int_value( int64_t  value, uint16_t  *data_0, size_t  size_0,
                                                    uint16_t  *data_1, size_t  size_1 ) const noexcept
{
    uint64_t 	uval;
    // положительные числа ограничиваем в дополнительном коде, чтобы они не перелезли в знаковый разряд
    int64_t 	max_value = ( ( 1LL << ( priv__->m_uBits-1 ) ) - 1LL );

    // дополнительный код
    if( ( 0 == priv__->m_nSign ) || ( -2 == priv__->m_nSign ) )
    {
        if( -2 == priv__->m_nSign )
            max_value = ( ( 1LL << ( priv__->m_uBits-2 ) ) - 1LL );

        if( value > max_value )
            value = max_value;

        // по минимальному значению не ограничиваем,
        // потому что в дополнительном коде отрицательные числа просто дополняются единицами в старших разрядах

        // побитово переносим данные
        memcpy( &uval, &value, sizeof(int64_t) );
        // и берем по маске, иначе обрежется при установке unsigned
        uval &= ( ( 1ULL << priv__->m_uBits ) - 1ULL );

        set_unsigned_value( uval, data_0, size_0, data_1, size_1 );
    }
    else
    {
        // генерируем прямой код
        // берем по модулю
        uval = static_cast<uint64_t>( ( value >= 0 ) ? value : -value );
        // накладываем маску
        uval &= ( ( 1ULL << ( priv__->m_uBits-1 ) ) - 1ULL );
        // добавляем знаковый разряд
        if( value < 0 )
            uval |= ( 1ULL << priv__->m_nSign );

        set_unsigned_value( uval, data_0, size_0, data_1, size_1 );
    }
}

void TGoodsField::set_real_value( double  value, uint16_t  *data_0, size_t  size_0,
                                                    uint16_t  *data_1, size_t  size_1 ) const noexcept
{
    // без знака
    if( -1 ==  priv__->m_nSign )
    {
        uint64_t 	uval = static_cast<uint64_t>( round( value / priv__->m_fULOB ) );
        set_unsigned_value( uval, data_0, size_0, data_1, size_1 );
    }
    else
    {
        int64_t 	val = static_cast<int64_t>( round( value / priv__->m_fULOB ) );
        set_int_value( val, data_0, size_0, data_1, size_1 );
    }
}

void TGoodsField::set_mask( bool  masked, uint16_t  *data_0, size_t size_0,
                                             uint16_t  *data_1, size_t size_1 ) const noexcept
{
    for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
    {
        unsigned    word   = it->beginWord();    // слово в векторах данных
        unsigned    bits   = it->bits();
        unsigned    offset = it->offset();
        uint16_t   *data = nullptr;
        size_t      size = 0;
        if( ! it->secondVector() )
        {
            data = data_0;
            size = size_0;
        }
        else
        {
            data = data_1;
            size = size_1;
        }

        // Слово интерфейса может занимать больше одного слова в векторах данных.
        // Если сдвиг больше 15 бит, то сразу переходим в следующее слово вектора данных.
        while( 15 < offset )
        {
            word++;
            offset -= 16;
        }
        while( 0 != bits )
        {
            if( ! __protocol_check_size( word, size, id() ) )
                break;

            unsigned    bits_in_word = std::min( bits, 16U - offset );
            unsigned    mask_in_word = ( ( 1U << bits_in_word ) - 1U ) << offset;
            if( masked )
                data[ word ] = data[ word ] | mask_in_word;
            else
                data[ word ] = data[ word ] & ~mask_in_word;
            bits -= bits_in_word;
            word++;
            offset = 0;
        }
    }
}

bool TGoodsField::masked( uint16_t  *data_0, size_t size_0,
                             uint16_t  *data_1, size_t size_1 ) const noexcept
{
    for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
    {
        unsigned    word   = it->beginWord();    // слово в векторах данных
        unsigned    bits   = it->bits();
        unsigned    offset = it->offset();
        uint16_t   *data = nullptr;
        size_t      size = 0;
        if( ! it->secondVector() )
        {
            data = data_0;
            size = size_0;
        }
        else
        {
            data = data_1;
            size = size_1;
        }

        // Слово интерфейса может занимать больше одного слова в векторах данных.
        // Если сдвиг больше 15 бит, то сразу переходим в следующее слово вектора данных.
        while( 15 < offset )
        {
            word++;
            offset -= 16;
        }
        while( 0 != bits )
        {
            if( ! __protocol_check_size( word, size, id() ) )
                return false;

            unsigned    bits_in_word = std::min( bits, 16U - offset );
            unsigned    mask_in_word = ( ( 1U << bits_in_word ) - 1U ) << offset;
            if( ( static_cast<unsigned>( data[ word ] ) & mask_in_word ) != mask_in_word )
                return  false;
            bits -= bits_in_word;
            word++;
            offset = 0;
        }
    }

    return  true;
}

void 	TGoodsField::set_order( size_t  number ) noexcept
{
    priv__->m_uOrderNumber = number;
}

size_t 	TGoodsField::order() const noexcept
{
    return priv__->m_uOrderNumber;
}

void 	TGoodsField::set_route( unsigned  route ) noexcept
{
    priv__->m_uRoute = route;
}

unsigned 	TGoodsField::route() const noexcept
{
    return priv__->m_uRoute;
}

//------------------------------------------------------------------------------

bool TGoodsField::get_bool(const uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForBool.begin(), priv__->s_tTypesForBool.end(), priv__->m_eType ) == priv__->s_tTypesForBool.end() )
		return false;

	uint64_t 	uval = get_unsigned_value( data, size );
	
	return ( 0 != uval );
}

int TGoodsField::get_int(const uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForInt.begin(), priv__->s_tTypesForInt.end(), priv__->m_eType ) == priv__->s_tTypesForInt.end() )
		return 0;

	int64_t 	val = get_int_value( data, size );
	
    return static_cast<int>(val) * static_cast<int>( priv__->m_fULOB );
}

unsigned TGoodsField::get_uint(const uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForUInt.begin(), priv__->s_tTypesForUInt.end(), priv__->m_eType ) == priv__->s_tTypesForUInt.end() )
		return 0;

	uint64_t 	uval = get_unsigned_value( data, size );
	
    return static_cast<unsigned>(uval) * static_cast<unsigned>( priv__->m_fULOB );
}

uint64_t TGoodsField::get_ulong(const uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForUInt.begin(), priv__->s_tTypesForUInt.end(), priv__->m_eType ) == priv__->s_tTypesForUInt.end() )
		return 0;

	uint64_t 	uval = get_unsigned_value( data, size );
	
    return uval * static_cast<uint64_t>( priv__->m_fULOB );
}

std::string TGoodsField::get_text(const uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForText.begin(), priv__->s_tTypesForText.end(), priv__->m_eType ) == priv__->s_tTypesForText.end() )
		return "";

    if( Combo == priv__->m_eType )
	{
		uint64_t 	uval = get_unsigned_value( data, size );
		
		try
		{
            return priv__->m_vValues.at( static_cast<unsigned>(uval) );
		} catch( std::out_of_range )
		{
			return "";
		}
	}

    // Проверяем, что смещение во всех словах 0 или 8. Вариант произвольного смещения в слове исключаем.
    for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
    {
        if( 0 != ( it->offset() % 8 ) )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Bad Protocol. Yaml text field bias is not a multiple of 8 for " << priv__->m_zId << "\n";
#endif
            return "";
        }
        if( 0 != ( it->bits() % 8 ) )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << " Bad Protocol. Yaml text field numberOfDigits is not a multiple of 8 for " << priv__->m_zId << "\n";
#endif
            return "";
        }
    }

    std::string  val;
    for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
    {
        unsigned    word   = it->beginWord();
        unsigned    bits   = it->bits();
        unsigned    offset = it->offset();
        // Слово интерфейса может занимать больше одного слова в векторах данных.
        // Если сдвиг больше 15 бит, то сразу переходим в следующее слово вектора данных.
        while( 15 < offset )
        {
            word++;
            offset -= 16;
        }
        while( 0 != bits )
        {
            if( ! __protocol_check_size( word, size, id() ) )
                break;

            unsigned    bits_in_word = std::min( bits, 16U - offset );
            char        ch = ( data[ word ] >> offset ) & 0x00FF;
            val.push_back( ch );
            bits -= 8;

            if( 16 == bits_in_word )
            {
                ch = ( data[ word ] >> 8 ) & 0x00FF;
                val.push_back( ch );
                bits -= 8;
            }

            // Переходим в следующее слово вектора и обнуляем offset на случай, если интерфейсное слово
            // занимает больше одного слова в векторах
            word++;
            offset = 0;
        }
    }

    return  val;
}

double TGoodsField::get_double( const uint16_t  *data_0, size_t size_0,
                                   const uint16_t  *data_1, size_t size_1 ) const noexcept
{
    if( std::find( priv__->s_tTypesForDouble.begin(), priv__->s_tTypesForDouble.end(), priv__->m_eType ) == priv__->s_tTypesForDouble.end() )
        return 0;

    if( Real == priv__->m_eType )
        return get_real_value( data_0, size_0, data_1, size_1 );

    if( Double == priv__->m_eType )
    {
        double 	fVal;
        uint64_t raw_val = get_unsigned_value( data_0, size_0, data_1, size_1 );
        memcpy( &fVal, &raw_val, sizeof( double ) );

        return fVal;
    }

    if( Float == priv__->m_eType )
    {
        float 	fVal;
        uint64_t raw_val = get_unsigned_value( data_0, size_0, data_1, size_1 );
        memcpy( &fVal, &raw_val, sizeof( float ) );

        return static_cast<double>( fVal );
    }

    return 0;
}

//------------------------------------------------------------------------------

void TGoodsField::set_bool(bool  value, uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForBool.begin(), priv__->s_tTypesForBool.end(), priv__->m_eType ) == priv__->s_tTypesForBool.end() )
		return;

	uint64_t 	uval = ( value ? 1 : 0 );

	set_unsigned_value( uval, data, size );
}

void TGoodsField::set_int(int  value, uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForInt.begin(), priv__->s_tTypesForInt.end(), priv__->m_eType ) == priv__->s_tTypesForInt.end() )
		return;

	int64_t 	val = value / static_cast<int>( priv__->m_fULOB );

	set_int_value( val, data, size );
}

void TGoodsField::set_uint(unsigned  value, uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForUInt.begin(), priv__->s_tTypesForUInt.end(), priv__->m_eType ) == priv__->s_tTypesForUInt.end() )
		return;

	uint64_t 	val = value / static_cast<uint64_t>( priv__->m_fULOB );

	set_unsigned_value( val, data, size );
}

void TGoodsField::set_ulong(uint64_t  value, uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForUInt.begin(), priv__->s_tTypesForUInt.end(), priv__->m_eType ) == priv__->s_tTypesForUInt.end() )
		return;

	uint64_t 	val = value / static_cast<uint64_t>( priv__->m_fULOB );

	set_unsigned_value( val, data, size );
}

void TGoodsField::set_text(const std::string  value, uint16_t  *data, size_t size ) const noexcept
{
    if( std::find( priv__->s_tTypesForText.begin(), priv__->s_tTypesForText.end(), priv__->m_eType ) == priv__->s_tTypesForText.end() )
		return;

    if( Combo == priv__->m_eType )
	{
        auto it = std::find( priv__->m_vValues.begin(), priv__->m_vValues.end(), value );

        if( it != priv__->m_vValues.end() )
		{
            int 	val = std::distance( it, priv__->m_vValues.begin() ) - 1;
            set_unsigned_value( static_cast<uint64_t>(val), data, size );
		}
	}
	else
	{
        // Проверяем, что смещение во всех словах 0 или 8. Вариант произвольного смещения в слове исключаем.
        for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
        {
            if( 0 != ( it->offset() % 8 ) )
            {
#ifdef __DEBUG__
                std::cerr << priv__->m_zId << " Bad Protocol. Yaml text field bias is not a multiple of 8 for " << priv__->m_zId << "\n";
#endif
                return;
            }
            if( 0 != ( it->bits() % 8 ) )
            {
#ifdef __DEBUG__
                std::cerr << priv__->m_zId << " Bad Protocol. Yaml text field numberOfDigits is not a multiple of 8 for " << priv__->m_zId << "\n";
#endif
                return;
            }
        }

        const char*     str_data = value.c_str();
        size_t          idx = 0;
        for( auto it = priv__->m_vDataWords.cbegin(); it != priv__->m_vDataWords.cend(); it++ )
        {
            unsigned    word   = it->beginWord();    // слово в векторах данных
            unsigned    bits   = it->bits();
            unsigned    offset = it->offset();
            // Слово интерфейса может занимать больше одного слова в векторах данных.
            // Если сдвиг больше 15 бит, то сразу переходим в следующее слово вектора данных.
            while( 15 < offset )
            {
                word++;
                offset -= 16;
            }
            while( ( 0 != bits ) && ( '\0' != str_data[ idx ] ) )
            {
                if( ! __protocol_check_size( word, size, id() ) )
                    break;

                unsigned    bits_in_word = std::min( bits, 16U - offset );
                unsigned    mask_in_word = ( ( 1U << bits_in_word ) - 1U ) << offset;
                unsigned    val_in_word = ( static_cast<unsigned>( str_data[ idx++ ] ) & 0xFF ) << offset;
                bits -= 8;

                if( ( 16 == bits_in_word ) && ( 0 != str_data[ idx ] ) )
                {
                    val_in_word = val_in_word | ( ( static_cast<unsigned>( str_data[ idx++ ] ) & 0xFF ) << 8 );
                    mask_in_word = 0xFFFF;
                    bits -= 8;
                }

                data[ word ] = ( data[ word ] & ~mask_in_word ) | static_cast<uint16_t>( val_in_word );

                word++;
                offset = 0;
            }
        }
	}
}

void TGoodsField::set_double( double  value, uint16_t  *data_0, size_t size_0,
                                                uint16_t  *data_1, size_t size_1 ) const noexcept
{
    if( std::find( priv__->s_tTypesForDouble.begin(), priv__->s_tTypesForDouble.end(), priv__->m_eType ) == priv__->s_tTypesForDouble.end() )
        return;

    if( Real == priv__->m_eType )
    {
        set_real_value( value, data_0, size_0, data_1, size_1 );
    }
    else if( Double == priv__->m_eType )
    {
        uint64_t    uval = 0;
        memcpy( &uval, &value, sizeof( double ) );
        set_unsigned_value( uval, data_0, size_0, data_1, size_1 );
    }
    else if( Float == priv__->m_eType )
    {
        uint64_t    uval = 0;
        memcpy( &uval, &value, sizeof( float ) );
        set_unsigned_value( uval, data_0, size_0, data_1, size_1 );
    }
}

//------------------------------------------------------------------------------

class cobu::TGoodsTablePrivate
{
    friend class TGoodsTable;
private:
    TGoodsTable 	*ins__;

    std::string 	m_zId;
    std::unordered_map<std::string, PTGoodsField> 	m_vFields; 	// массив записей по id - для поиска
    std::vector<PTGoodsField> 						m_vOrder; 	// массив записей по порядку их чтения

    unsigned 		m_uRoute;

    inline void clear()
    {
        m_zId.clear();
        m_vFields.clear();
        m_vOrder.clear();
        m_uRoute = 0;
    }

    inline TGoodsTablePrivate( TGoodsTable  *ins ) :
        ins__( ins )
    {
        clear();
        (void)ins__;
    }
};

void 	TGoodsTable::set_route( unsigned  route ) noexcept
{
    priv__->m_uRoute = route;
    for( auto field : priv__->m_vOrder )
        field->set_route( route );
}

unsigned 	TGoodsTable::route() const noexcept
{
    return priv__->m_uRoute;
}

bool TGoodsTable::add_field( const PTGoodsField  a_ptField )
{
    if( ! a_ptField->empty() )
    {
        // устанавливаем текущий порядковый номер поля в таблице
        a_ptField->set_order( priv__->m_vOrder.size() );
        // устанавливаем маршрут из таблицы если он был
        if( priv__->m_uRoute )
            a_ptField->set_route( priv__->m_uRoute );

        priv__->m_vFields[ a_ptField->id() ] = a_ptField;
        priv__->m_vOrder.push_back( a_ptField );
    }
    return ( ! a_ptField->empty() );
}

bool TGoodsTable::parse_yaml( const YAML::Node&  node )
{
    // check for buffer name
    if( ! __yaml_IsScalar( node[ "buffer" ] ) )
    {
#ifdef __DEBUG__
        std::cerr << "Cannot read buffer name for table in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
        clear();
        return false;
    }

    priv__->m_zId = node[ "buffer" ].as<std::string>();

    // check for require items
    if( ! __yaml_IsSequence( node[ "items" ] ) )
    {
#ifdef __DEBUG__
        std::cerr << priv__->m_zId << " Cannot read require items for table in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
        clear();
        return false;
    }

    priv__->m_vFields.clear();
    priv__->m_vFields.reserve( node[ "items" ].size() );

    // читаем route если он есть
    if( __yaml_IsScalar( node[ "route" ] ) )
        priv__->m_uRoute = node[ "route" ].as<unsigned>();

    // проходимся по записям и создаем их
    for( auto  it : node[ "items" ] )
        add_field( std::make_shared<TGoodsField>( it ) );

    if( ! priv__->m_vOrder.size() )
    {
#ifdef __DEBUG__
        std::cerr << priv__->m_zId << " Empty of fields sequence in table in " << __FILE__ << " : " << __LINE__ << "\n";
        clear();
#endif
        return false;
    }

	return true;
}

TGoodsTable::TGoodsTable()
{
    priv__ = std::unique_ptr<TGoodsTablePrivate>( new TGoodsTablePrivate( this ) );
    clear();
}

TGoodsTable::TGoodsTable( const YAML::Node&  node )
{
    priv__ = std::unique_ptr<TGoodsTablePrivate>( new TGoodsTablePrivate( this ) );
    clear();
    if( ! parse_yaml( node ) )
        clear();
}

TGoodsTable::TGoodsTable( const std::string&  config )
{
    priv__ = std::unique_ptr<TGoodsTablePrivate>( new TGoodsTablePrivate( this ) );
    clear();
    YAML::Node 	node = YAML::Load( config );
    if( ! parse_yaml( node ) )
        clear();
}

TGoodsTable::~TGoodsTable()
{
	clear();
}

void TGoodsTable::clear() noexcept
{
    priv__->clear();
}

bool TGoodsTable::empty() const noexcept
{
    return priv__->m_zId.empty();
}

const std::string &TGoodsTable::id() const noexcept
{
    return priv__->m_zId;
}

size_t TGoodsTable::size() const noexcept
{
    return priv__->m_vOrder.size();
}

bool 	TGoodsTable::has_field( const std::string&  id ) const
{
    std::string 	strim_id = id;
    trim( strim_id );

    if( ! priv__->m_vFields[ strim_id ] )
        return false;
    return ( ! priv__->m_vFields[ strim_id ]->empty() );
}

TGoodsField& TGoodsTable::operator[]( const std::string& id )
{
    std::string 	strim_id = id;
    trim( strim_id );

    if( nullptr == priv__->m_vFields[ strim_id ].get() )
        priv__->m_vFields[ strim_id ] = std::make_shared<TGoodsField>();
    return *( priv__->m_vFields[ strim_id ] );
}

std::vector<PTGoodsField>::iterator 	TGoodsTable::begin()
{
    return priv__->m_vOrder.begin();
}

std::vector<PTGoodsField>::iterator 	TGoodsTable::end()
{
    return priv__->m_vOrder.end();
}

void 	TGoodsTable::united( uint16_t *result, const uint16_t *data, const uint16_t *manual, const uint16_t *mask, size_t  size )
{
    for( size_t i = 0; i < size; i++ )
    {
        uint16_t 	val = data[ i ];
        val &= ~mask[ i ];
        val |= ( manual[ i ] & mask[ i ] );
        result[ i ] = val;
    }
}

//------------------------------------------------------------------------------

class cobu::TGoodsInterfaceEntryPrivate
{
    friend class TGoodsInterfaceEntry;
private:
    TGoodsInterfaceEntry             *ins__;         // pointer to corresponding protocol interface entry instance

    std::string                         m_zSrc;         // Поле src (имя вектора)
    double                              m_fFreq;        // Частота передачи вектора
    TGoodsInterfaceEntry::EDirect    m_eDirect;      // Направление передачи вектора
    // специфичные для МКИО поля
    unsigned                            m_uSubaddr;     // Подадрес МКИО
    // специфичные для Arinc поля
    unsigned                            m_uWord;        // Адрес слова Arinc
    double                              m_fBaud;        // Поле baud для Arinc
    int                                 m_nAddr;        // Линия передачи слова Arinc

    TGoodsInterfaceEntry::ERoute     m_eRoute;       // Тип интерфейса: Milstd, Arinc или Oneoff

    inline void     clear() noexcept
    {
        m_zSrc.clear();
        m_fFreq = 0;
        m_uSubaddr = 0;
        m_uWord = 0;
        m_fBaud = 0;
        m_nAddr = -1;
    }

    inline TGoodsInterfaceEntryPrivate( TGoodsInterfaceEntry  *ins, TGoodsInterfaceEntry::ERoute route = TGoodsInterfaceEntry::NotDefined )
        : ins__( ins )
        , m_eRoute( route )
    {
        clear();
        (void)ins__;
    }
};

TGoodsInterfaceEntry::TGoodsInterfaceEntry()
{
    priv__ = std::unique_ptr<TGoodsInterfaceEntryPrivate>( new TGoodsInterfaceEntryPrivate( this ) );
    clear();
}

TGoodsInterfaceEntry::TGoodsInterfaceEntry( const YAML::Node&  node, TGoodsInterfaceEntry::ERoute route,
                                                  int default_arinc_in_addr, int default_arinc_out_addr )
{
    priv__ = std::unique_ptr<TGoodsInterfaceEntryPrivate>( new TGoodsInterfaceEntryPrivate( this, route ) );
    clear();
    if( ! parse_yaml( node, default_arinc_in_addr, default_arinc_out_addr ) )
        clear();
}

TGoodsInterfaceEntry::~TGoodsInterfaceEntry()
{
    clear();
}

void TGoodsInterfaceEntry::clear() noexcept
{
    priv__->clear();
}

bool TGoodsInterfaceEntry::empty() const noexcept
{
    return priv__->m_zSrc.empty();
}

std::string TGoodsInterfaceEntry::src() const noexcept
{
    return priv__->m_zSrc;
}

TGoodsInterfaceEntry::EDirect    TGoodsInterfaceEntry::direct() const noexcept
{
    return priv__->m_eDirect;
}

double      TGoodsInterfaceEntry::freq() const noexcept
{
    return priv__->m_fFreq;
}

unsigned    TGoodsInterfaceEntry::subaddr() const noexcept
{
    return priv__->m_uSubaddr;
}

unsigned    TGoodsInterfaceEntry::word() const noexcept
{
    return priv__->m_uWord;
}

double      TGoodsInterfaceEntry::baud() const noexcept
{
    return priv__->m_fBaud;
}

int         TGoodsInterfaceEntry::addr() const noexcept
{
    return priv__->m_nAddr;
}

bool TGoodsInterfaceEntry::parse_yaml( const YAML::Node&  node, int default_arinc_in_addr, int default_arinc_out_addr )
{
    // check for require
    static  std::vector<std::string>    satPrtclRequiredCommon =
    {
        "src", "direct"
    };
    for( auto it : satPrtclRequiredCommon )
    {
        if( ! __yaml_IsScalar( node[ it ] ) )
        {
#ifdef __DEBUG__
            std::cerr << "Cannot read " << it << " field for interface entry in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
            return false;
        }
    }

    try {
        priv__->m_zSrc = __yaml_GetString( node, "src" );
        std::string zDirect = __yaml_GetString( node, "direct" );
        if( ProtocolInterfaceDirectIn == zDirect )
            priv__->m_eDirect = In;
        else if( ProtocolInterfaceDirectOut == zDirect )
            priv__->m_eDirect = Out;
        else
        {
#ifdef __DEBUG__
            std::cerr << "Cannot read direct field for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
            return false;
        }
    }
    catch( const YAML::Exception& )
    {
        std::cout << "Exception of parse yaml is not expected at " << __FILE__ << ":" << __LINE__ << "\n";
        return false;
    }

    switch( priv__->m_eRoute )
    {
    case NotDefined:
        return false;
    case Milstd:
        if( __yaml_IsMap( node ) )
        {
            static  std::vector<std::string>    satPrtclRequiredMilstd =
            {
                "subaddr", "freq"
            };
            for( auto it : satPrtclRequiredMilstd )
            {
                if( ! __yaml_IsScalar( node[ it ] ) )
                {
#ifdef __DEBUG__
                    std::cerr << "Cannot read " << it << " field in mkio section for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }
            }
            try {
                priv__->m_uSubaddr = node[ "subaddr" ].as<unsigned>();
                // Проверяем, что подадрес находится в допустимых пределах
                if( 0 == priv__->m_uSubaddr || ProtocolInterfaceMilstdMaxSubaddr <= priv__->m_uSubaddr )
                {
#ifdef __DEBUG__
                    std::cerr << "Invalid MIL-STD subaddress found for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }
                priv__->m_fFreq = node[ "freq" ].as<double>();
            }
            catch( const YAML::Exception& )
            {
                std::cout << "Exception of parse yaml in mkio section is not expected in " << __FILE__ << ":" << __LINE__ << "\n";
                return false;
            }
        }
        break;
    case Arinc:
        if( __yaml_IsMap( node ) )
        {
            static  std::vector<std::string>    satPrtclRequiredArinc =
            {
                "word", "freq", "baud"
            };
            for( auto it : satPrtclRequiredArinc )
            {
                if( ! __yaml_IsScalar( node[ it ] ) )
                {
#ifdef __DEBUG__
                    std::cerr << "Cannot read " << it << " field in arinc section for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }
            }

            try {
                priv__->m_uWord = node[ "word" ].as<unsigned>();
                if( 0 == priv__->m_uWord || ProtocolInterfaceArincMaxWord < priv__->m_uWord )
                {
#ifdef __DEBUG__
                    std::cerr << "word field contains incorrect value for " << priv__->m_zSrc << " at " << __FILE__ << " : " << __LINE__ << "\n";
#endif
                    return false;
                }
                priv__->m_fFreq = node[ "freq" ].as<double>();
                priv__->m_fBaud = node[ "baud" ].as<double>();
            }
            catch( const YAML::Exception& )
            {
                std::cout << "Exception of parse yaml in arinc section is not expected at " << __FILE__ << ":" << __LINE__ << "\n";
                return false;
            }

            // номер линии ARINC
            if( In == priv__->m_eDirect )   // Входной вектор
            {
                if( __yaml_IsScalar( node[ "out_addr" ] ) )
                {
#ifdef __DEBUG__
                    std::cerr << "out_addr is set for IN vector in arinc section for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }

                if( __yaml_IsScalar( node[ "in_addr" ] ) )
                    priv__->m_nAddr = node[ "in_addr" ].as<int>();
                else
                    priv__->m_nAddr = default_arinc_in_addr;

                if( 0 > priv__->m_nAddr )
                {
#ifdef __DEBUG__
                    std::cerr << "Cannot read ARINC in_addr field for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }
            }
            else    // Выходной вектор
            {
                if( __yaml_IsScalar( node[ "in_addr" ] ) )
                {
#ifdef __DEBUG__
                    std::cerr << "in_addr is set for OUT vector in arinc section for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }

                if( __yaml_IsScalar( node[ "out_addr" ] ) )
                    priv__->m_nAddr = node[ "out_addr" ].as<int>();
                else
                    priv__->m_nAddr = default_arinc_out_addr;

                if( 0 > priv__->m_nAddr )
                {
#ifdef __DEBUG__
                    std::cerr << "Cannot read ARINC out_addr field for " << priv__->m_zSrc << " at " << __FILE__ << ":" << __LINE__ << "\n";
#endif
                    return false;
                }
            }
        }
        break;
    case Oneoff:
        return false;
    case Ethernet :
        return false;
    case Model :
        return false;
    case Stick :
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------

class cobu::TGoodsInterfacePrivate
{
    friend class TGoodsInterface;
private:
    TGoodsInterface  *ins__;

    std::string         m_zId;                  // Тип (наименование) интерфейса
    unsigned            m_uMkioAddr;            // Адрес МКИО (ноль, если протокол не МКИО)
    int                 m_nCommonArincInAddr;   // Общий для всех векторов входной адрес ARINC
    int                 m_nCommonArincOutAddr;  // Общий для всех векторов выходной адрес ARINC
    std::unordered_map<std::string, PTGoodsInterfaceEntry>	m_vEntries;		// массив записей по id - для поиска
    std::vector<PTGoodsInterfaceEntry>						m_vOrder;		// массив записей по порядку их чтения

    inline void clear()
    {
        m_zId.clear();
        m_uMkioAddr = 0;
        m_nCommonArincInAddr = -1;
        m_nCommonArincOutAddr = -1;
        m_vEntries.clear();
        m_vOrder.clear();
    }

    inline TGoodsInterfacePrivate( TGoodsInterface  *ins ) :
        ins__( ins )
    {
        clear();
        (void)ins__;
    }
};

TGoodsInterface::TGoodsInterface( const std::string&  id,  const YAML::Node&  node )
{
    priv__ = std::unique_ptr<TGoodsInterfacePrivate>( new TGoodsInterfacePrivate( this ) );
    clear();
    priv__->m_zId = id;
    if( ! parse_yaml( node ) )
        clear();
}

TGoodsInterface::TGoodsInterface( const std::string&  id,  const std::string&  config )
{
    priv__ = std::unique_ptr<TGoodsInterfacePrivate>( new TGoodsInterfacePrivate( this ) );
    clear();
    priv__->m_zId = id;
    YAML::Node 	node = YAML::Load( config );
    if( ! parse_yaml( node ) )
        clear();
}

TGoodsInterface::~TGoodsInterface()
{
    clear();
}

void TGoodsInterface::clear() noexcept
{
    priv__->clear();
}

bool TGoodsInterface::empty() const noexcept
{
    return ( 0 == size() );
}

size_t TGoodsInterface::size() const noexcept
{
    return priv__->m_vEntries.size();
}

const std::string& TGoodsInterface::id() const noexcept
{
    return priv__->m_zId;
}

bool    TGoodsInterface::has_entry( const std::string&  id ) const
{
    std::string     strim_id = id;
    trim( strim_id );

    if( ! priv__->m_vEntries[ strim_id ] )
        return false;
    return ( ! priv__->m_vEntries[ strim_id ]->empty() );
}

TGoodsInterfaceEntry&    TGoodsInterface::operator[]( const std::string&  id )
{
    std::string 	strim_id = id;
    trim( strim_id );

    if( nullptr == priv__->m_vEntries[ strim_id ].get() )
        priv__->m_vEntries[ strim_id ] = std::make_shared<TGoodsInterfaceEntry>();
    return *( priv__->m_vEntries[ strim_id ] );
}

unsigned    TGoodsInterface::mkio_addr() const noexcept
{
    if( ProtocolMkioSectionName == priv__->m_zId )
        return priv__->m_uMkioAddr;
    else
        return 0;
}

int         TGoodsInterface::arinc_common_in_addr() const noexcept
{
    if( ProtocolArincSectionName == priv__->m_zId )
        return priv__->m_nCommonArincInAddr;
    else
        return -1;
}

int         TGoodsInterface::arinc_common_out_addr() const noexcept
{
    if( ProtocolArincSectionName == priv__->m_zId )
        return priv__->m_nCommonArincOutAddr;
    else
        return -1;
}

std::vector<PTGoodsInterfaceEntry>::iterator     TGoodsInterface::begin()
{
    return priv__->m_vOrder.begin();
}

std::vector<PTGoodsInterfaceEntry>::iterator     TGoodsInterface::end()
{
    return priv__->m_vOrder.end();
}

bool TGoodsInterface::add_entry( const PTGoodsInterfaceEntry  a_ptEntry )
{
    if( ! a_ptEntry->empty() )
    {
        priv__->m_vEntries[ a_ptEntry->src() ] = a_ptEntry;
        priv__->m_vOrder.push_back( a_ptEntry );
    }
    return ( ! a_ptEntry->empty() );
}

bool TGoodsInterface::parse_yaml( const YAML::Node&  node )
{
    // Check for presence of buffer child node
    if( !__yaml_IsSequence( node[ "buffer"] ) )
    {
#ifdef __DEBUG__
        std::cerr << priv__->m_zId << " section does not have correct buffer subsection\n";
#endif
        return false;
    }

    // Если секция описывает конфигурацию МКИО
    if( ProtocolMkioSectionName == id() )
    {
        if( !__yaml_IsScalar( node[ "addr" ] ) )
        {
#ifdef __DEBUG__
            std::cerr << "Cannot read addr from mkio section\n";
#endif
            return false;
        }

        priv__->m_uMkioAddr = node[ "addr" ].as<unsigned>();
        if( ProtocolInterfaceMilstdMaxAddr < priv__->m_uMkioAddr )
        {
#ifdef __DEBUG__
                std::cerr << "Milstd address is out of range at " << __FILE__ << " : " << __LINE__ << "\n";
#endif
        }

        for( auto it : node[ "buffer" ] )
        {
            if( __yaml_IsMap( it ) )
            {
                add_entry( std::make_shared<TGoodsInterfaceEntry>( it, TGoodsInterfaceEntry::Milstd ) );
            }
            else
            {
#ifdef __DEBUG__
                std::cerr << "Cannot read buffer entry from mkio section in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
            }
        }
    }

    // Если секция описывает конфигурацию ARINC
    if( ProtocolArincSectionName == id() )
    {
        // Если секция содержит общий для всех векторов in_addr
        if( __yaml_IsScalar( node[ "in_addr" ] ) )
            priv__->m_nCommonArincInAddr = node[ "in_addr" ].as<int>();

        // Если секция содержит общий для всех векторов out_addr
        if( __yaml_IsScalar( node[ "out_addr" ] ) )
            priv__->m_nCommonArincOutAddr = node[ "out_addr" ].as<int>();

        for( auto it : node[ "buffer" ] )
        {
            if( __yaml_IsMap( it ) )
            {
                add_entry( std::make_shared<TGoodsInterfaceEntry>( it,
                                                                      TGoodsInterfaceEntry::Arinc,
                                                                      priv__->m_nCommonArincInAddr,
                                                                      priv__->m_nCommonArincOutAddr ) );
            }
            else
            {
#ifdef __DEBUG__
                std::cerr << "Cannot read buffer entry from arinc section in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------

class cobu::TGoodsPrivate
{
    friend class TGoods;
private:
    TGoods 	*ins__;

    std::vector<PTGoodsTable> 				m_vOrder;
    std::map<std::string, TGoodsTableArray> 	m_avDirect;
    std::map<std::string, PTGoodsTable> 		m_avTables;
    std::map<std::string, PTGoodsInterface>	m_avInterfaces;

    std::string 	m_zId;
    std::string 	m_zName;
    std::string 	m_zVersion;
    std::string 	m_zDescription;
    std::string 	m_zDesignation;
    unsigned		m_uNetwork;
    std::vector<std::string> 		m_vDocuments; 	// перечень имен документов
    std::vector<std::string> 		m_vSuffixes; 	// перечень суффиксов для добавления к имени при установке АРМа
    // перечень состояний системы (секция states)
    std::vector<std::string> 		m_vStates;
    // перечень векторов с их размерами
    std::vector<std::pair<std::string,unsigned>> 	m_vVectors;

    YAML::Node 		config__;
    bool 		m_bNoModel;

    inline 	void clear()
    {
        m_vOrder.clear();
        m_avDirect.clear();
        m_avTables.clear();
        m_vStates.clear();
        m_vVectors.clear();

        m_zId.clear();
        m_zName.clear();
        m_zVersion.clear();
        m_zDescription.clear();
        m_zDesignation.clear();
        m_uNetwork = 0;
        m_vDocuments.clear();
        m_vSuffixes.clear();

        m_bNoModel = false;
    }

    inline TGoodsPrivate( TGoods  *ins ) :
        ins__( ins )
    {
        clear();
        (void)ins__;
    }
};

bool TGoods::parse_yaml( const YAML::Node&  config )
{
    priv__->config__ = config;

    if( __yaml_IsMap( config[ ProtocolHeaderSectionName ] ) )
    {
	    priv__->m_zId = __yaml_GetString( config[ ProtocolHeaderSectionName ], "id" );
	    priv__->m_zName = __yaml_GetString( config[ ProtocolHeaderSectionName ], "name" );
	    priv__->m_zVersion = __yaml_GetString( config[ ProtocolHeaderSectionName ], "version" );
	    priv__->m_zDescription = __yaml_GetString( config[ ProtocolHeaderSectionName ], "description" );
	    priv__->m_zDesignation = __yaml_GetString( config[ ProtocolHeaderSectionName ], "designation" );

        if( __yaml_IsScalar( config[ ProtocolHeaderSectionName ][ "network" ] ) )
        {
            priv__->m_uNetwork = config[ ProtocolHeaderSectionName ][ "network" ].as<unsigned>();
        }

	    // documents list
	    if( __yaml_IsScalar( config[ ProtocolHeaderSectionName ][ "documents" ] ) )
	    {
		    const std::string 	val = config[ ProtocolHeaderSectionName ][ "documents" ].as<std::string>();
            split( priv__->m_vDocuments, val, '\n' );
	    }
	    // suffixes list
	    if( __yaml_IsScalar( config[ ProtocolHeaderSectionName ][ "suffixes" ] ) )
	    {
		    const std::string 	val = config[ ProtocolHeaderSectionName ][ "suffixes" ].as<std::string>();
            split( priv__->m_vSuffixes, val, '\n' );
	    }
    }	

    // секции которые будем читать в вектора
    std::vector<std::string>  azSections =
    {
        ProtocolInSectionName, ProtocolOutSectionName, ProtocolErrSectionName, ProtocolModelSectionName
    };

    if( priv__->m_bNoModel )
    {
	    azSections.pop_back();
    }

    // читаем секции в вектора
    for( auto  str : azSections )
    {
        if( __yaml_IsSequence( config[ str ] ) )
        {
            for( auto it : config[ str ] )
            {
                PTGoodsTable 	ptr = std::make_shared<TGoodsTable>( it );
                if( add_table( ptr ) )
                    priv__->m_avDirect[ str ].push_back( ptr );
                else
                {
#ifdef __DEBUG__
                    std::cerr << priv__->m_zId << " Failure to read table in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
                }
            }
        }
    }

	// states list
	if( __yaml_IsScalar( config[ ProtocolStatesSectionName ] ) )
	{
		const std::string 	val = config[ ProtocolStatesSectionName ].as<std::string>();
        split( priv__->m_vStates, val, '\n' );
	}

    // vectors list
    if( __yaml_IsSequence( config[ ProtocolVectorsSectionName ] ) )
    {
        for( auto v : config[ ProtocolVectorsSectionName ] )
        {
            if( __yaml_IsScalar( v[ "id" ] ) && __yaml_IsScalar( v[ "size" ] ) )
            {
                std::string 	id = v[ "id" ].as<std::string>();
                unsigned	 	s = v[ "size" ].as<unsigned>();

                priv__->m_vVectors.emplace_back( id, s );
            }
        }
    }

    // МКИО
    if( __yaml_IsMap( config[ ProtocolMkioSectionName ] ) )
    {
        PTGoodsInterface     ptr = std::make_shared<TGoodsInterface>( ProtocolMkioSectionName, config[ ProtocolMkioSectionName ] );
        if( ! add_interface( ptr ) )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << ": Failure to parse mkio section in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
        }
    }

    // ARINC
    if( __yaml_IsMap( config[ ProtocolArincSectionName ] ) )
    {
        PTGoodsInterface     ptr = std::make_shared<TGoodsInterface>( ProtocolArincSectionName, config[ ProtocolArincSectionName ] );
        if( ! add_interface( ptr ) )
        {
#ifdef __DEBUG__
            std::cerr << priv__->m_zId << ": Failure to parse arinc section in " << __FILE__ << " : " << __LINE__ << "\n";
#endif
        }
    }

	return true;
}

bool 	TGoods::parse( const std::string&  filename )
{
    if( ! filIsRegular( filename ) )
    {
        std::cout << __FILE__ << ":" << __LINE__ << " File " << filename << " is not exists, or is not a regular file, failure\n";
        return false;
    }

    YAML::Node 	config;
    try
    {
        config = YAML::LoadFile( filename );
    } catch ( const YAML::Exception&  e ) {
        std::cout << __FILE__ << ":" << __LINE__ << " Exception of LoadFile function for " << filename << " : " << e.what() << "\n";
        return false;
    }

    try
    {
        return parse_yaml( config );
    }
    catch( ... )
    {
        std::cout << "Exception of parse yaml is not expected for " << filename << " at " << __FILE__ << ":" << __LINE__ << "\n";
        return false;
    }
}

TGoods::TGoods()
{
    priv__ = std::unique_ptr<TGoodsPrivate>( new TGoodsPrivate( this ) );
    clear();
}

TGoods::TGoods( const YAML::Node&  config )
{
    priv__ = std::unique_ptr<TGoodsPrivate>( new TGoodsPrivate( this ) );
    clear();
    if( ! parse_yaml( config ) )
		clear();
}

TGoods::TGoods( const std::string&  filename, bool  bNoModel )
{
    priv__ = std::unique_ptr<TGoodsPrivate>( new TGoodsPrivate( this ) );
    clear();
    priv__->m_bNoModel = bNoModel;

    if( ! parse( filename ) )
        clear();
}

TGoods::~TGoods()
{
    clear();
}

void TGoods::clear() noexcept
{
    priv__->clear();
}

bool TGoods::empty() const noexcept
{
    return priv__->m_zId.empty();
}

size_t 	TGoods::size() const noexcept
{
    return priv__->m_vOrder.size();
}

std::string TGoods::id() const noexcept
{
    return priv__->m_zId;
}

std::string TGoods::name() const noexcept
{
    return priv__->m_zName;
}

std::string TGoods::version() const noexcept
{
    return priv__->m_zVersion;
}

std::string TGoods::description() const noexcept
{
    return priv__->m_zDescription;
}

std::string TGoods::designation() const noexcept
{
    return priv__->m_zDesignation;
}

unsigned TGoods::network() const noexcept
{
    return priv__->m_uNetwork;
}

const std::vector<std::string>& TGoods::documents() const noexcept
{
    return priv__->m_vDocuments;
}

const std::vector<std::string>& TGoods::suffixes() const noexcept
{
    return priv__->m_vSuffixes;
}

const std::vector<std::string>& TGoods::states() const noexcept
{
    return priv__->m_vStates;
}

const std::vector<std::pair<std::string,unsigned>>& TGoods::vectors() const noexcept
{
    return priv__->m_vVectors;
}

size_t 			TGoods::vector_size( const std::string&  id ) const
{
    auto it = std::find_if( priv__->m_vVectors.begin(), priv__->m_vVectors.end(),
                            [ = ]( std::pair<std::string,unsigned> val ) {
        return ( val.first == id ); } );
    if( it == priv__->m_vVectors.end() )
        return 0;
    return it->second;
;}

std::vector<PTGoodsTable>::iterator 	TGoods::begin() noexcept
{
    return priv__->m_vOrder.begin();
}

std::vector<PTGoodsTable>::iterator 	TGoods::end() noexcept
{
    return priv__->m_vOrder.end();
}

TGoodsTable& 	TGoods::operator[]( const std::string&  id )
{
    if( nullptr == priv__->m_avTables[ id ].get() )
        priv__->m_avTables[ id ] = std::make_shared<TGoodsTable>();
    return *( priv__->m_avTables[ id ] );
}

PTGoodsTable 	TGoods::table_ptr( const std::string&  id )
{
    return priv__->m_avTables[ id ];
}

TGoodsTableArray& 	TGoods::at( const std::string&  section )
{
	/*
	 *if( nullptr == m_avDirect[ section ].get() )
     *        m_avTables[ id ] = std::make_shared<TGoodsTableArray>();
	 */
    return priv__->m_avDirect[ section ];
}

PTGoodsInterface TGoods::interface_ptr( const std::string&  id )
{
    return priv__->m_avInterfaces[ id ];
}

void 	TGoods::set_id( const std::string&  id ) noexcept
{
    priv__->m_zId = id;
}

bool 	TGoods::add_table( const PTGoodsTable  a_ptTable )
{
    if( ! a_ptTable->empty() )
    {
        // проверяем наличие этой таблице в общем списке - по поиску
        if( priv__->m_avTables[ a_ptTable->id() ] )
        {
            auto 	ptr = priv__->m_avTables[ a_ptTable->id() ];
            for( auto f : *a_ptTable )
                ptr->add_field( f );
            //< поскольку ptr это был указатель, а сам объект таблицы у нас один, то эти изменения прошли и в Order и в Direct
        }
        else 	// если это новая таблица - просто добавляем ее
        {
            priv__->m_vOrder.push_back( a_ptTable );
            priv__->m_avTables[ a_ptTable->id() ] = a_ptTable;
        }
    }

    return ( ! a_ptTable->empty() );
}

bool 	TGoods::add_table( const YAML::Node&  node )
{
    return add_table( std::make_shared<TGoodsTable>( node ) );
}

bool 	TGoods::add_table( const std::string&  config )
{
    return add_table( std::make_shared<TGoodsTable>( config ) );
}

bool 	TGoods::add_vector( const std::string&  vect, size_t  def_size, bool  overwrite )
{
    // ищем вектор если он есть
    auto 	vect_it = std::find_if( priv__->m_vVectors.begin(), priv__->m_vVectors.end(),
                                    [ = ]( const std::pair<std::string,unsigned>&  val ) {
        return ( val.first == vect );
    } );

    // если его нет - добавляем
    if( vect_it == priv__->m_vVectors.end() )
    {
        priv__->m_vVectors.emplace_back( vect, def_size );
        return true;
    }
    else if( overwrite ) 	// если нет и установлен флаг - переписываем размер
    {
        vect_it->second = static_cast<unsigned>( def_size );
        return true;
    }
    return false;
}

bool 	TGoods::auto_add_vectors_from( const std::string&  section,  size_t  def_size )
{
    if( ! priv__->config__.size() )
        return false;

    if( ! __yaml_IsMap( priv__->config__[ section ] ) )
       return false;

    if( ! __yaml_IsSequence( priv__->config__[ section ][ "buffer" ] ) )
        return false;

    for( auto it : priv__->config__[ section ][ "buffer" ] )
    {
        const std::string 	zVect =  __yaml_GetString( it, "src" );
        if( zVect.empty() )
            continue;

        if( add_vector( zVect, def_size ) )
        {
#ifdef __DEBUG__
//            std::cerr << priv__->m_zId << " Auto add vector " << zVect << " size " << def_size << " from section " << section << "\n";
#endif
        }
    }

    return true;
}

void 	TGoods::auto_add_vectors()
{
    auto_add_vectors_from( ProtocolMkioSectionName, 64 );
    auto_add_vectors_from( ProtocolArincSectionName, 4 );
    auto_add_vectors_from( ProtocolOneoffSectionName, 128 );
}

void 	TGoods::route_vector( const std::string&  vect, unsigned  route )
{
    // ищем таблицу
    PTGoodsTable 	tbl = table_ptr( vect );

    if( tbl )
        tbl->set_route( route );
}

void 	TGoods::auto_route_from( const std::string&  section, unsigned  route )
{
    if( ! priv__->config__.size() )
        return;

    if( ! __yaml_IsMap( priv__->config__[ section ] ) )
       return;

    if( ! __yaml_IsSequence( priv__->config__[ section ][ "buffer" ] ) )
        return;

    for( auto it : priv__->config__[ section ][ "buffer" ] )
    {
        const std::string 	zVect =  __yaml_GetString( it, "src" );
        if( zVect.empty() )
            continue;

        route_vector( zVect, route );
    }
}

void 	TGoods::auto_route_vectors()
{
    auto_route_from( ProtocolMkioSectionName, 1 );
    auto_route_from( ProtocolArincSectionName, 2 );
    auto_route_from( ProtocolOneoffSectionName, 3 );
}

bool 	TGoods::add_interface( const PTGoodsInterface  a_ptInterface )
{
    if( ! a_ptInterface->empty() )
    {
        // проверяем наличие этого интерфейса в общем списке - по поиску
        if( ! priv__->m_avInterfaces[ a_ptInterface->id() ] )
        {
            // добавляем описание интерфейса
            priv__->m_avInterfaces[ a_ptInterface->id() ] = a_ptInterface;
            return true;
        }
    }

    return false;
}

bool 	TGoods::add_interface( const std::string&  id, const YAML::Node&  node )
{
    return add_interface( std::make_shared<TGoodsInterface>( id, node ) );
}

bool 	TGoods::add_interface( const std::string&  id, const std::string&  config )
{
    return add_interface( std::make_shared<TGoodsInterface>( id, config ) );
}

//------------------------------------------------------------------------------

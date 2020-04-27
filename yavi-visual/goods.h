#ifndef GOODS_H
#define GOODS_H

#include <stdio.h>

#include <string>
#include <yaml-cpp/yaml.h>

namespace cobu
{

//------------------------------------------------------------------------------

class TGoodsFieldPrivate;

//! класс записи протокола - одно поле
class TGoodsField
{
    friend class TGoodsFieldPrivate;

	public :
        //! типы полей в протоколе
		enum EType 
		{
			Undefined,
			Bool,
			Integer,
			Unsigned,
			Hex,
			Combo,
			Real,
			Double,
			Float,
			Text
		};

        //! поддерживаемые кодировки для полей типа Text
        enum ETextEncoding
        {
            EncodingDefault,    // Кодировка, используемая Qt по умолчанию
            EncodingAscii,
            EncodingWindows1251,
            EncodingWindows1252,
            EncodingKoi7,
        };

        // constructors
        explicit TGoodsField( const YAML::Node&  node ); 				//!< constructor
        TGoodsField(); 													//!< default constructor
        TGoodsField( const TGoodsField& other ) = delete; 			//!< copy constructor
        TGoodsField& operator=( const TGoodsField& other ) = delete; 	//!< copy operator
        TGoodsField( TGoodsField&& other ) = delete; 					//!< move constructor
        TGoodsField& operator=( TGoodsField&& other ) = default; 		//!< TODO move operator
        ~TGoodsField(); 													//!< destructor

        //! очистить запись
        void 			clear() noexcept;
        // забрать параметры записи
        bool 			empty() const noexcept;			//!< запись пустая?
        EType 			type() const noexcept;			//!< тип записи
        ETextEncoding   encoding() const noexcept;		//!< тип кодировки для текстового поля
        const std::string& 	id()  const noexcept;		//!< идентификатор записи
        const std::string& 	name() const noexcept;		//!< имя (описание)
        const std::string& 	unit() const noexcept;		//!< единица измерения
        double 			minimum() const noexcept;		//!< минимальное значение
        double 			maximum() const noexcept;		//!< максимальное значение
        double 			medium() const noexcept; 		//!< среднее значение, которое можно установить
        unsigned 		precision() const noexcept;		//!< число знаков после запятой
        bool 			hidden() const noexcept; 		//!< запись спрятана
        unsigned 		bits() const noexcept; 			//!< полное число бит в поле
        bool            logic() const noexcept; 		//!< участие в логики
        bool                dualVector() const noexcept;        //!< параметр использует два вектора?
        const std::string& 	secondVectorName() const noexcept;  //!< имя второго вектора (пустая строка, если его нет)

        //! вектор строк с возможными значениями (Combo)
        const std::vector<std::string>& 	values() const noexcept;
        //! вектор строк с возможными значениями (Combo)
        const std::vector<std::string>& 	expand_values() const noexcept;

        // маскирование записей - установка всех бит записи в 1
        //! маскировать/размаскировать запись - все биты в 1/все биты в 0 (data_1, size_1 для значений, использующих два вектора)
//        void 		set_mask( bool  masked, uint16_t  *data, size_t  size ) const noexcept;
        void 		set_mask( bool  masked, uint16_t  *data_0,           size_t  size_0,
                                            uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;
        //! запись маскирована - все биты установлены в 1 ? (data_1, size_1 для значений, использующих два вектора)
//        bool 		masked( uint16_t  *data, size_t  size ) const noexcept;
        bool 		masked( uint16_t  *data_0,           size_t  size_0,
                            uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;

        // порядковый номер записи в таблице (для различных технологических целей)
        //! установить порядковый номер записи в таблице
        void 		set_order( size_t  number ) noexcept;
        //! порядковый номер записи в таблице
        size_t 		order() const noexcept;

        // маршрут (линия) для передачи поля
        //! установить маршрут передачи как число
        void 		set_route( unsigned  path ) noexcept;
        //! маршрут передачи поля (как число)
        unsigned 	route() const noexcept;

        // взять значения записи протокола разных типов
        //! взять значение как bool
        bool 		get_bool( const uint16_t  *data, size_t  size ) const noexcept;
        //! взять значение как целое знаковое число
        int	 		get_int( const uint16_t  *data, size_t  size ) const noexcept;
        //! взять значение как целое число без знака
        unsigned 	get_uint( const uint16_t  *data, size_t  size ) const noexcept;
        //! взять значение как длинное целое число без знака
        uint64_t 	get_ulong( const uint16_t  *data, size_t  size ) const noexcept;
        //! взять значение как строку текста
        std::string get_text( const uint16_t  *data, size_t  size ) const noexcept;
//        //! взять значение как double
//        double 		get_double( const uint16_t  *data, size_t  size ) const noexcept;
        //! взять значение как double (data_1, size_1 для значений, использующих два вектора)
        double 		get_double( const uint16_t  *data_0,           size_t  size_0,
                                const uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;

        // установить значения записи протокола разных типов
        //! установить значение как bool
        void 		set_bool( bool  value, uint16_t  *data, size_t  size ) const noexcept;
        //! установить значение как целое знаковое число
        void 		set_int( int  value, uint16_t  *data, size_t  size ) const noexcept;
        //! установить значение как целое число без знака
        void 		set_uint( unsigned  value, uint16_t  *data, size_t  size ) const noexcept;
        //! установить значение как длинное целое число без знака
        void 		set_ulong( uint64_t  value, uint16_t  *data, size_t  size ) const noexcept;
        //! установить значение как строку текста
        void 		set_text( const std::string  value, uint16_t  *data, size_t  size ) const noexcept;
//        //! установить значение как double
//        void 		set_double( double  value, uint16_t  *data, size_t  size ) const noexcept;
        //! установить значение как double (data_1, size_1 для значений, использующих два вектора)
        void 		set_double( double  value, uint16_t  *data_0,           size_t  size_0,
                                               uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;

    protected :
        // базовые функции чтения полных значений из протокола
//        uint64_t 	get_unsigned_value( const uint16_t  *data, size_t  size ) const noexcept;
//        int64_t		get_int_value( const uint16_t  *data, size_t  size ) const noexcept;
//        double		get_real_value( const uint16_t  *data, size_t  size ) const noexcept;

        // базовые функции чтения полных значений из протокола (data_1, size_1 для значений, использующих два вектора)
        uint64_t 	get_unsigned_value( const uint16_t  *data_0,           size_t  size_0,
                                        const uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;
        int64_t		get_int_value( const uint16_t  *data_0,           size_t  size_0,
                                   const uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;
        double		get_real_value( const uint16_t  *data_0,           size_t  size_0,
                                    const uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;

        // считать файл типа yaml, вернуть значение, все ли хорошо
		bool 		parse_yaml( const YAML::Node&  node );

        // базовые функции записи полных значений в протокол
//        void 		set_unsigned_value( uint64_t  value, uint16_t  *data, size_t  size ) const noexcept;
//        void 		set_int_value( int64_t  value, uint16_t  *data, size_t  size ) const noexcept;
//        void 		set_real_value( double  value, uint16_t  *data, size_t  size ) const noexcept;

        // базовые функции записи полных значений в протокол (data_1, size_1 для значений, использующих два вектора)
        void 		set_unsigned_value( uint64_t  value, uint16_t  *data_0,           size_t  size_0,
                                                         uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;
        void 		set_int_value( int64_t  value, uint16_t  *data_0,           size_t  size_0,
                                                   uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;
        void 		set_real_value( double  value, uint16_t  *data_0,           size_t  size_0,
                                                   uint16_t  *data_1 = nullptr, size_t  size_1 = 0 ) const noexcept;

    private :
        std::unique_ptr<TGoodsFieldPrivate> 	priv__;

        unsigned 	yaml_GetUnsigned( const YAML::Node&  node, const std::string&  name, unsigned  def = 0 );
        double 		yaml_GetDouble( const YAML::Node&  node, const std::string&  name, double  def = 0 );
        unsigned 	yaml_GetAltUnsigned( const YAML::Node&  node, const std::string&  main, const std::string&  alt, unsigned  def );

};

//! тип - указатель на запись протокола
typedef std::shared_ptr<TGoodsField> 	PTGoodsField;

//------------------------------------------------------------------------------

class TGoodsTablePrivate;

//! класс - таблица протокола - вектор данных
class TGoodsTable
{
    friend class TGoodsTablePrivate;

	public :

        // constructors
        TGoodsTable(); 	// TODO for delete default constructor
        explicit TGoodsTable( const YAML::Node&  node );
        explicit TGoodsTable( const std::string&  config );
        TGoodsTable( const TGoodsTable& other ) = delete; 		//!< copy constructor
        TGoodsTable& operator=( const TGoodsTable& other ) = delete; 	//!< copy operator
        ~TGoodsTable();

        //! очистить таблицу
		void 		clear() noexcept;

        // возврат параметров таблицы
        //! таблица пустая?
		bool 		empty() const noexcept;
        //! количество записей в таблице - число полей протокола
		size_t 		size() const noexcept;
        //! текстовый идентификатор таблицы - имя вектора
        const std::string& 	id() const noexcept;

        //! проверка наличия поля в таблице
        bool 		has_field( const std::string&  id ) const;
        //! взять запись протокола по ее имени (по ее id)
        TGoodsField& operator[]( const std::string&  id );

        // итераторы для обхода таблицы как массива, в порядке записей в файле
        //! итератор начала таблицы - в соответствии с порядком ее описания в файле
        std::vector<PTGoodsField>::iterator 	begin();
        //! итератор конца таблицы - в соответствии с порядком ее описания в файле
        std::vector<PTGoodsField>::iterator 	end();
        //! добавить запись в таблицу по указателю
        bool 		add_field( const PTGoodsField 	a_ptField );

        // маршрут (линия) для передачи вектора данных
        //! установить маршрут передачи как число
        void 		set_route( unsigned  path ) noexcept;
        //! маршрут передачи вектора (как число)
        unsigned 	route() const noexcept;

    public :
        //! функция объединения массивов
        //! в mask указываются биты, которые надо взять из manual и заместить ими биты в data, положить результат в result
        static void 	united( uint16_t  *result, const uint16_t  *data, const uint16_t  *manual, const uint16_t  *mask, size_t  size );

	protected :
        // считать таблицу из yaml
		bool 		parse_yaml( const YAML::Node&  node );

	private :
        std::unique_ptr<TGoodsTablePrivate> 	priv__;
};

//! тип - указатель на таблицу протокола
typedef std::shared_ptr<TGoodsTable> 	PTGoodsTable;

//! тип - массив таблиц протокола - вектора из конкретной секции
typedef std::vector<PTGoodsTable> 		TGoodsTableArray;

//------------------------------------------------------------------------------

class TGoodsInterfaceEntryPrivate;

//! класс записи вектора в секции конфигурации интерфейса - одно поле
class TGoodsInterfaceEntry
{
    friend class TGoodsInterfaceEntryPrivate;

public:
#define ProtocolInterfaceDirectIn           "in"    //!< значение поля direct для входящего вектора
#define ProtocolInterfaceDirectOut          "out"   //!< значение поля direct для исходящего вектора
#define ProtocolInterfaceMilstdMaxSubaddr   30      //!< максимально допустимый подадрес МКИО
#define ProtocolInterfaceArincMaxWord       255     //!< максимально допустимый адрес ARINC

    //! направление передачи вектора
    enum EDirect
    {
        In,
        Out
    };

    //! возможные типы интерфейсов
    enum ERoute
    {
        NotDefined,
        Milstd,
        Arinc,
        Oneoff,
        Ethernet,
        Model,
        Stick
    };

    // constructors
    TGoodsInterfaceEntry( const YAML::Node&  node, ERoute route, int default_arinc_in_addr = -1,
                             int default_arinc_out_addr = -1 );                             //!< constructor
    TGoodsInterfaceEntry();                                                              //!< default constructor
    TGoodsInterfaceEntry( const TGoodsInterfaceEntry& other ) = delete;               //!< copy constructor
    TGoodsInterfaceEntry& operator=( const TGoodsInterfaceEntry& other ) = delete;    //!< copy operator
    TGoodsInterfaceEntry( TGoodsInterfaceEntry&& other ) = delete;                    //!< move constructor
    TGoodsInterfaceEntry& operator=( TGoodsInterfaceEntry&& other ) = default;        //!< TODO move operator
    ~TGoodsInterfaceEntry();

    //! очистить запись
    void        clear() noexcept;
    bool        empty() const noexcept;			//!< запись пустая?

    std::string src() const noexcept;           //!< имя вектора
    EDirect     direct() const noexcept;        //!< направление передачи вектора
    double      freq() const noexcept;          //!< частота обмена, если она была задана, в противном случае возращает 0.

    // MILSTD
    unsigned    subaddr() const noexcept;       //!< подадрес МКИО или 0, если запись описывает не МКИО

    // ARINC
    unsigned    word() const noexcept;          //!< адрес ARINC или 0, если запись описывает не ARINC
    double      baud() const noexcept;          //!< скорость передачи по ARINC или 0, если запись описывает не ARINC

    //! поле in_addr или out_addr для ARINC (в зависимости от направления передачи вектора). Возвращает -1, если запись описывает не ARINC.
    int         addr() const noexcept;

protected:
    // считать файл типа yaml, вернуть значение, все ли хорошо
    bool 		parse_yaml( const YAML::Node&  node, int default_arinc_in_addr, int default_arinc_out_addr );

private :
    std::unique_ptr<TGoodsInterfaceEntryPrivate> priv__;
};

//! тип - указатель на запись таблицы интерфейса
typedef std::shared_ptr<TGoodsInterfaceEntry> 	PTGoodsInterfaceEntry;

//------------------------------------------------------------------------------

class TGoodsInterfacePrivate;

//! класс - таблица описания конфигурации интерфейсов
class TGoodsInterface
{
    friend class TGoodsInterfacePrivate;

    public :
#define ProtocolInterfaceMilstdMaxAddr          31      //!< максимально допустимый адрес МКИО

        // constructors
        TGoodsInterface() = delete;
        TGoodsInterface( const std::string&  id,  const YAML::Node&  node );
        TGoodsInterface( const std::string&  id,  const std::string&  config );
        TGoodsInterface( const TGoodsInterface& other ) = delete;               //!< copy constructor
        TGoodsInterface& operator=( const TGoodsInterface& other ) = delete;    //!< copy operator
        ~TGoodsInterface();

        //! очистить таблицу
        void 		clear() noexcept;

        // возврат параметров интерфейса
        //! интерфейс не содержит записей (векторов)?
        bool 		empty() const noexcept;
        //! количество записей (векторов) в конфиге
        size_t 		size() const noexcept;
        //! текстовый идентификатор интерфейса (протокола обмена)
        const std::string& 	id() const noexcept;

        //! проверка наличия записи с вектором в секции описания интерфейса (передаётся ли вектор по интерфейсу?)
        bool 		has_entry( const std::string&  id ) const;
        //! взять запись протокола по ее имени (по ее id)
        TGoodsInterfaceEntry& operator[]( const std::string&  id );
        //! адрес МКИО имитатора, если объект описывает mkio-секцию конфига. Для других секций возвращает 0
        unsigned    mkio_addr() const noexcept;
        //! общий адрес ARINC для входящих векторов, прописанный в конфиге. Возвращает -1, если значение не задано или объект описывает не ARINC-интерфейс
        int         arinc_common_in_addr() const noexcept;
        //! общий адрес ARINC для исходящих векторов, прописанный в конфиге. Возвращает -1, если значение не задано или объект описывает не ARINC-интерфейс
        int         arinc_common_out_addr() const noexcept;

        // итераторы для обхода секции описания протокола как массива, в порядке записей в файле
        //! итератор начала секции - в соответствии с порядком ее описания в файле
        std::vector<PTGoodsInterfaceEntry>::iterator begin();
        //! итератор конца секции - в соответствии с порядком ее описания в файле
        std::vector<PTGoodsInterfaceEntry>::iterator end();
        //! добавить запись по указателю
        bool        add_entry( const PTGoodsInterfaceEntry 	a_ptEntry );

    protected :
        // считать таблицу из yaml
        bool 		parse_yaml( const YAML::Node&  node );

    private :
        std::unique_ptr<TGoodsInterfacePrivate> 	priv__;
};

//! тип - указатель на таблицу конфигурации интерфейса
typedef std::shared_ptr<TGoodsInterface> 	PTGoodsInterface;

//! тип - массив таблиц конфигурации интерфейсов протокола
typedef std::vector<PTGoodsInterface> 		TGoodsInterfaceArray;

//------------------------------------------------------------------------------

class TGoodsPrivate;

//! класс - протокол взаимодействия, считанный с файла
class TGoods
{
    friend class TGoodsPrivate;

	public :
    // имена основных секций файла конфигурации
#define ProtocolHeaderSectionName 	"header" 	//!< имя секции заголовка с основной информацией
#define ProtocolInSectionName 		"in" 		//!< имя секции входных данных
#define ProtocolOutSectionName 		"out"		//!< имя секции выходных данных
#define ProtocolErrSectionName 		"error" 	//!< имя секции с ошибками (выходные данные)
#define ProtocolModelSectionName 	"model"		//!< имя секции с входными данными от модели
#define ProtocolStatesSectionName 	"states" 	//!< имя секции с описанием состояний
#define ProtocolVectorsSectionName 	"vectors" 	//!< имя секции с описанием векторов (буферов)
#define ProtocolMkioSectionName 	"mkio"
#define ProtocolArincSectionName 	"arinc"
#define ProtocolOneoffSectionName 	"oneoff"

        // constructors
        TGoods();
        explicit TGoods( const std::string&  filename, bool  bNoModel = false ); 	//!< создание из файла (сейчас yaml)
        explicit TGoods( const YAML::Node&  config );		//!< создание из ноды yaml
        TGoods( const TGoods& other ) = delete; 			//!< copy constructor
        TGoods& operator=( const TGoods& other ) = delete; 	//!< copy operator
        ~TGoods();

        //! разбор файла протокола
        bool 		parse( const std::string&  filename );
        //! разбор протокола, считанного предварительно в yaml
        bool 		parse_yaml( const YAML::Node&  config );

        void 		clear() noexcept;
        bool 		empty() const noexcept;
        size_t 		size() const noexcept;

        // возврат значений из секции header
        std::string 	id() const noexcept;
        std::string 	name() const noexcept;
        std::string 	version() const noexcept;
        std::string 	description() const noexcept;
        std::string 	designation() const noexcept;
        unsigned        network() const noexcept;
        const std::vector<std::string>& documents() const noexcept;
        const std::vector<std::string>& suffixes() const noexcept;
        // возврат значений из секции states
        const std::vector<std::string>& states() const noexcept;
        // возврат значений из секции vectors
        const std::vector<std::pair<std::string,unsigned>>& vectors() const noexcept;

        // размер вектора по имени id в байтах - из секции vectors
        size_t 			vector_size( const std::string&  id ) const;

        // итераторы для последовательного доступа к векторам - в порядке их описания в файле
        std::vector<PTGoodsTable>::iterator 	begin() noexcept;
        std::vector<PTGoodsTable>::iterator 	end() noexcept;

        // доступ к таблице протокола (вектору) по имени (id)
        TGoodsTable& 	operator[]( const std::string&  id );
        PTGoodsTable 	table_ptr( const std::string&  id );
        // доступ к массиву таблиц протокола конкретной секции - по имени секции
        // далее возвращается массив, который можно обходить последовательно - вектор
        TGoodsTableArray& 	at( const std::string&  section );

        // доступ к описанию интерфейса по его названию
        PTGoodsInterface interface_ptr( const std::string&  id );

        // установка id
        void 		set_id( const std::string&  id ) noexcept;
        // добавление таблицы в протокол по указателю
        bool 		add_table( const PTGoodsTable  a_ptTable );
        // ручное добавление таблицы в протокол, по ноде или прямо по yaml в текстовом виде
        bool 		add_table( const YAML::Node&  node );
        bool 		add_table( const std::string&  config );
        // ручное добавление вектора в перечень векторов
        bool 		add_vector( const std::string&  vect, size_t  def_size, bool  overwrite = false );

        // автоматическое создание недостающих элементов в секции vectors
        void 		auto_add_vectors();
        bool 		auto_add_vectors_from( const std::string&  section,  size_t  def_size );

        void 		route_vector( const std::string&  vect, unsigned  route );
        void 		auto_route_from( const std::string&  section, unsigned  route );
        void 		auto_route_vectors();

        // добавление конфигурации интерфейса в протокол по указателю
        bool 		add_interface( const PTGoodsInterface  a_ptInterface );
        // ручное добавление конфигурации интерфейса в протокол, по ноде или прямо по yaml в текстовом виде
        bool 		add_interface( const std::string&  id, const YAML::Node&  node );
        bool 		add_interface( const std::string&  id, const std::string&  config );

    private :
        std::unique_ptr<TGoodsPrivate> 	priv__;
};

//! тип - указатель на протокол, описанный в файле
typedef std::shared_ptr<TGoods> 	PTGoods;

//------------------------------------------------------------------------------

} // namespace cobu

#endif // GOODS_H

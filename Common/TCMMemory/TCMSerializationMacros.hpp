#pragma once

namespace TCM
{
	namespace Engine
	{
		class TCMBase;
	}
}

/**
 * \brief EXPAND() is used to bypass Visual Studio sending __VA_ARGS__ as one simple argument\n
 *		  Example, if __VA_ARGS__ == 1, 2, 3, Visual would send DO_SOMETHING( __VA_ARGS__ ) with one DO_SOMETHING( (1, 2, 3) ) parameter instead of three DO_SOMETHING( 1, 2, 3 ) parameters
 */
#define EXPAND( x ) x

/**
 * \brief Macro used to get the SerializedVarClass of a class member
 * \param var The variable to be serialized
 */
#define TCM_SERIALIZED_VAR_EDITABLE( var )	new SerializedVarClass<decltype(##var)>(var, #var, TCM::SerializedInterface::EEditorType::EDITABLE)
#define TCM_SERIALIZED_VAR_READONLY( var )	new SerializedVarClass<decltype(##var)>(var, #var, TCM::SerializedInterface::EEditorType::READONLY)
#define TCM_SERIALIZED_VAR_HIDDEN( var )	new SerializedVarClass<decltype(##var)>(var, #var, TCM::SerializedInterface::EEditorType::HIDDEN)

/**
 * \brief Get the type name of a variable
 * \param var The var to get the type
 */
#define TCM_GET_TYPE_NAME( var )	typeid( decltype(##var) ).name()

/**
 * \brief Creates a pair name / type name
 * \param var The variable used to create the pair
 */
#define TCM_GET_PAIR_NAME_TYPE( var )	std::pair<const std::string, std::string>( #var, MetaSerializedFactory::Define<decltype(##var)>( TCM_GET_TYPE_NAME( var ) ) )

#pragma region Macro PRE EXECUTION FOR EACH

#define TCM_PRE_EXEC_MAX_PARAMETER	20

#define TCM_PRE_EXEC_FE_1( toBeExecuted, var )			toBeExecuted( var )
#define TCM_PRE_EXEC_FE_2( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_1( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_3( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_2( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_4( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_3( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_5( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_4( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_6( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_5( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_7( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_6( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_8( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_7( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_9( toBeExecuted, var, ... )		toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_8( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_10( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_9( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_11( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_10( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_12( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_11( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_13( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_12( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_14( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_13( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_15( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_14( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_16( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_15( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_17( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_16( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_18( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_17( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_19( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_18( toBeExecuted, __VA_ARGS__ ) )
#define TCM_PRE_EXEC_FE_20( toBeExecuted, var, ... )	toBeExecuted( var ), EXPAND( TCM_PRE_EXEC_FE_19( toBeExecuted, __VA_ARGS__ ) )

/**
 * \brief Returns the function to be executed depending on the the number of parameters
 * \param NAME The function to be executed
 * \note  This is necessary because macros can't be recursive
 */
#define TCM_GET_PRE_EXECUTE_FOR_EACH_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, NAME, ... )	NAME

/**
 * \brief Executes a macro on a list of parameters and return the list of parameters after the function/macro
 * \param toBeExecuted The function/macro to be executed
 */
#define TCM_PRE_EXECUTE_FOR_EACH( toBeExecuted, ... )	EXPAND( TCM_GET_PRE_EXECUTE_FOR_EACH_N( __VA_ARGS__, TCM_PRE_EXEC_FE_20, TCM_PRE_EXEC_FE_19, TCM_PRE_EXEC_FE_18, TCM_PRE_EXEC_FE_17, TCM_PRE_EXEC_FE_16, TCM_PRE_EXEC_FE_15, TCM_PRE_EXEC_FE_14, TCM_PRE_EXEC_FE_13, TCM_PRE_EXEC_FE_12, TCM_PRE_EXEC_FE_11, TCM_PRE_EXEC_FE_10, TCM_PRE_EXEC_FE_9, TCM_PRE_EXEC_FE_8, TCM_PRE_EXEC_FE_7, TCM_PRE_EXEC_FE_6, TCM_PRE_EXEC_FE_5, TCM_PRE_EXEC_FE_4, TCM_PRE_EXEC_FE_3, TCM_PRE_EXEC_FE_2, TCM_PRE_EXEC_FE_1 )( toBeExecuted, __VA_ARGS__ ) )

#pragma endregion

#define TCM_SERIALIZE( ... )																													\
	public:																																		\
		virtual void _Serialize_Editable() override																								\
		{																																		\
			Super::_Serialize_Editable();																										\
			m_sizeInBytes = sizeof(*this);																										\
			_SerializeForEach( std::make_tuple( TCM_PRE_EXECUTE_FOR_EACH( TCM_SERIALIZED_VAR_EDITABLE, __VA_ARGS__ ) ) );						\
		};																																		\
		virtual void _SetSerializedAttributes_Editable( const std::unordered_map<std::string, SerializedInterface *>* serializedMap ) override	\
		{																																		\
			Super::_SetSerializedAttributes_Editable( serializedMap );																					\
			_UnSerializeForEach( std::make_tuple( TCM_PRE_EXECUTE_FOR_EACH( TCM_SERIALIZED_VAR_EDITABLE, __VA_ARGS__ ) ), serializedMap );		\
		};																																		\
		static std::map<const std::string, std::string>* _CreateMapClassMembers_Editable()														\
		{																																		\
			std::map<const std::string, std::string>*	mapClassMember = Super::_CreateMapClassMembers_Editable();										\
			std::initializer_list< std::pair<const std::string, std::string> > initList = { TCM_PRE_EXECUTE_FOR_EACH( TCM_GET_PAIR_NAME_TYPE, __VA_ARGS__ ) };		\
			mapClassMember->insert( initList );																									\
			return mapClassMember;																												\
		};

#define TCM_SERIALIZE_READONLY( ... )																													\
	public:																																		\
		virtual void _Serialize_Readonly() override																								\
		{																																		\
			Super::_Serialize_Readonly();																										\
			m_sizeInBytes = sizeof(*this);																										\
			_SerializeForEach( std::make_tuple( TCM_PRE_EXECUTE_FOR_EACH( TCM_SERIALIZED_VAR_READONLY, __VA_ARGS__ ) ) );						\
		};																																		\
		virtual void _SetSerializedAttributes_Readonly( const std::unordered_map<std::string, SerializedInterface *>* serializedMap ) override	\
		{																																		\
			Super::_SetSerializedAttributes_Readonly( serializedMap );																					\
			_UnSerializeForEach( std::make_tuple( TCM_PRE_EXECUTE_FOR_EACH( TCM_SERIALIZED_VAR_READONLY, __VA_ARGS__ ) ), serializedMap );		\
		};																																		\
		static std::map<const std::string, std::string>* _CreateMapClassMembers_Readonly()														\
		{																																		\
			std::map<const std::string, std::string>*	mapClassMember = Super::_CreateMapClassMembers_Readonly();										\
			std::initializer_list< std::pair<const std::string, std::string> > initList = { TCM_PRE_EXECUTE_FOR_EACH( TCM_GET_PAIR_NAME_TYPE, __VA_ARGS__ ) };		\
			mapClassMember->insert( initList );																									\
			return mapClassMember;																												\
		};

#define TCM_SERIALIZE_HIDDEN( ... )																													\
	public:																																		\
		virtual void _Serialize_Hidden() override																								\
		{																																		\
			Super::_Serialize_Hidden();																										\
			m_sizeInBytes = sizeof(*this);																										\
			_SerializeForEach( std::make_tuple( TCM_PRE_EXECUTE_FOR_EACH( TCM_SERIALIZED_VAR_HIDDEN, __VA_ARGS__ ) ) );						\
		};																																		\
		virtual void _SetSerializedAttributes_Hidden( const std::unordered_map<std::string, SerializedInterface *>* serializedMap ) override	\
		{																																		\
			Super::_SetSerializedAttributes_Hidden( serializedMap );																					\
			_UnSerializeForEach( std::make_tuple( TCM_PRE_EXECUTE_FOR_EACH( TCM_SERIALIZED_VAR_HIDDEN, __VA_ARGS__ ) ), serializedMap );		\
		};																																		\
		static std::map<const std::string, std::string>* _CreateMapClassMembers_Hidden()														\
		{																																		\
			std::map<const std::string, std::string>*	mapClassMember = Super::_CreateMapClassMembers_Hidden();										\
			std::initializer_list< std::pair<const std::string, std::string> > initList = { TCM_PRE_EXECUTE_FOR_EACH( TCM_GET_PAIR_NAME_TYPE, __VA_ARGS__ ) };		\
			mapClassMember->insert( initList );																									\
			return mapClassMember;																												\
		};

/**
 * \brief Macro used for every TCM Abstract class
 * \param BaseClass The TCM class the Derived Class inherits from
 */
#define TCM_ABSTRACT_CLASS( BaseClass )		\
	private:								\
		typedef	BaseClass Super;

/**
 * \brief Macro used for every TCM Concrete class
 * \param DerivedClass The current TCM class
 * \param BaseClass The TCM class the Derived Class inherits from
 */
#define TCM_CONCRETE_CLASS( DerivedClass, BaseClass )	\
	private:											\
		typedef	BaseClass Super;						\
	public:												\
		virtual TCMBase* _ClassConstructor() const override			\
		{															\
			return _GetSerializedConstructor<DerivedClass>();		\
		}															\
		static TCMBase* _FactoryConstructor()						\
		{															\
			return _GetSerializedConstructor<DerivedClass>();		\
		}															\
		virtual void _ClassDestructor( TCMBase* derived ) const override		\
		{															\
			_GetSerializedDestructor<DerivedClass>( dynamic_cast<DerivedClass *>( derived ) );		\
		}															\
		static void _FactoryDestructor( TCMBase* derived )			\
		{															\
			_GetSerializedDestructor<DerivedClass>( dynamic_cast<DerivedClass *>( derived ) );		\
		}															\
		virtual std::string _GetMetaFactoryName() const override	\
		{															\
			return #DerivedClass;									\
		}															\
		static bool _bHasBeenMetaFactoried_##DerivedClass;			\
		static std::map<const std::string, std::string>* _CreateMapClassMembers()													\
		{																															\
			std::map<const std::string, std::string>*	mapClassMember = new std::map<const std::string, std::string>();			\
			std::map<const std::string, std::string>*	mapClassMemberEditable = DerivedClass::_CreateMapClassMembers_Editable();	\
			mapClassMember->insert( mapClassMemberEditable->begin(), mapClassMemberEditable->end() );									\
			delete mapClassMemberEditable;																							\
			std::map<const std::string, std::string>*	mapClassMemberReadonly = DerivedClass::_CreateMapClassMembers_Readonly();	\
			mapClassMember->insert( mapClassMemberReadonly->begin(), mapClassMemberReadonly->end() );									\
			delete mapClassMemberReadonly;																							\
			std::map<const std::string, std::string>*	mapClassMemberHidden = DerivedClass::_CreateMapClassMembers_Hidden();		\
			mapClassMember->insert( mapClassMemberHidden->begin(), mapClassMemberHidden->end() );										\
			delete mapClassMemberHidden;																							\
			return mapClassMember;																									\
		}																															\
		virtual const std::map<const std::string, std::string>*& _GetMapClassMembers() const override	\
		{																								\
			return _mapClassMembers_##DerivedClass;														\
		}																								\
		static const std::map<const std::string, std::string>*& _GetStaticMapClassMembers()				\
		{																								\
			return _mapClassMembers_##DerivedClass;														\
		}																								\
		static const std::map<const std::string, std::string>* _mapClassMembers_##DerivedClass;

#define TCM_DEFINE( DerivedClass ) \
	static_assert( std::is_base_of<TCM::Engine::TCMBase, DerivedClass>::value, "TCM_DEFINE() should be used only for class inheriting from TCMBase !" ); \
	static_assert( !std::is_abstract<DerivedClass>::value, "TCM_DEFINE() should be used only for non-abstract class !" ); \
	bool DerivedClass::_bHasBeenMetaFactoried_##DerivedClass = MetaFactory::Define<DerivedClass>(#DerivedClass); \
	const std::map<const std::string, std::string>* DerivedClass::_mapClassMembers_##DerivedClass = DerivedClass::_CreateMapClassMembers();

////
// Next macros permit to automatically select TCM_CONCRETE_CLASS or TCM_ABSTRACT_CLASS using the number of arguments sent to TCM_CLASS
// If we send only BaseClass, we'll have __VA_ARGS__ -> BaseClass. In that case, GET_TCMCLASS( BaseClass, TCM_CONCRETE_CLASS, TCM_ABSTRACT_CLASS, ... ) will use TCM_ASBTRACT_CLASS
// If we send DerivedClass too, we'll have __VA_ARGS__ -> DerivedClass, BaseClass. In that case, GET_TCMCLASS( DerivedClass, BaseClass, TCM_CONCRETE_CLASS, TCM_ABSTRACT_CLASS ) will use TCM_CONCRETE_CLASS
// In the two cases of GET_TCMCLASS(), the function used is the third argument.
////

#define GET_TCMCLASS( _1, _2, NAME, ... ) NAME

/**
 * \brief Macro used for every TCM classes
 * \param DerivedClass|BaseClass for every concrete and instantiable class
 * \param BaseClass for every astract class
 * \note Call TCM_DEFINE( DerivedClass ) in the .cpp if class is non-abstract
 */
#define TCM_CLASS( ... ) EXPAND( GET_TCMCLASS( __VA_ARGS__, TCM_CONCRETE_CLASS, TCM_ABSTRACT_CLASS )( __VA_ARGS__ ) )

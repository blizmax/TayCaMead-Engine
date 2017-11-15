#pragma once

#include <string>
#include "TCMLogger/Logger.h"

namespace TCM
{
	/**
	 * \brief Serialized Interface of every serialized variable, class based or not
	 */
	struct SerializedInterface abstract
	{
		/**
		 * \brief Tells wether the Serialized Variable is visible or modifiable by the editor
		 */
		enum EEditorType : uint8_t
		{
			EDITABLE,
			READONLY,
			HIDDEN
		};

		explicit SerializedInterface( std::string varName, const EEditorType editorType )
			: m_varName( move( varName ) )
			, m_editorType( editorType )
		{
		}

		virtual ~SerializedInterface() = default;

		/**
		 * \brief Get the name of the variable being stored
		 * \return The name of the variable
		 */
		std::string GetVarName() const
		{
			return m_varName;
		}

		/**
		 * \brief Set the name of the variable being stored
		 * \param name _IN_ The new name
		 */
		void SetVarName( const std::string& name )
		{
			m_varName.assign( name );
		}

		/**
		 * \brief Get the editor type of the variable being stored
		 * \return The editor type of the variable
		 */
		EEditorType GetEditorType() const
		{
			return m_editorType;
		}

		/**
		 * \brief Set the editor type of the variable being stored
		 * \param editorType _IN_I The new editor type
		 */
		void SetEditorType( const EEditorType editorType )
		{
			m_editorType = editorType;
		}

		/**
		 * \brief Get the current type name
		 * \return A string of the contained type name
		 */
		virtual std::string GetTypeName() const = 0;

		/**
		 * \brief Get the current type name (without pointer)
		 * \return A string of the contained type name
		 */
		virtual std::string GetPointerlessTypeName() const = 0;

		/**
		 * \brief Write the data of this struct into the file
		 * \param fileStream _IN_ The file in which is going to be written the data
		 */
		virtual bool WriteToFile( std::ostream& fileStream ) const = 0;

		/**
		 * \brief Read the data to be set in this struct from the file
		 * \param fileIO _IN_ The file in which is going to be read the data
		 */
		virtual bool ReadFromFile( std::istream& fileIO ) = 0;

		/**
		 * \brief Returns wether the contained type is a pointer
		 * \return True if is pointer, false otherwise
		 */
		virtual bool IsPointer() const = 0;

		/**
		 * \brief Duplicates this class using the class's copy constructor
		 * \return A duplicate of this class
		 */
		virtual SerializedInterface* Duplicate() const = 0;

		/**
		 * \brief Get the value stored in this SerializedInterface
		 * \tparam T The type of the value to get
		 * \return The value stored in the SerializedInterface
		 */
		template<typename T>
		T Get() const;

		/**
		 * \brief Get the value stored in this SerializedInterface, removing pointer
		 * \tparam T The type of the value to get
		 * \return The value stored in the SerializedInterface, without pointers
		 */
		template<typename T>
		typename std::enable_if<!std::is_pointer<T>::value, T>::type GetPointerless() const;

		/**
		 * \brief Set a value to this SerializedInterface
		 * \tparam T The type of the value to set
		 * \param value The value to be set
		 */
		template<typename T>
		void Set( const T value );

		/**
		 * \brief Set a value to this SerializedInterface and modifies the referenced value
		 * \tparam T The type of the value to be set
		 * \param value The value to be set
		 */
		template<typename T>
		void ModifyVar( const T value );

		/**
		 * \brief Set a value to this SerializedInterface and modifies the referenced value if contained type is a pointer
		 * \tparam T The type of the value to be set
		 * \param value The value to be set
		 */
		template<typename T>
		void ModifyVarEditorPointer( const T value );

		/**
		 * \brief Set a value to this SerializedInterface and modifies the referenced value if contained type is not a pointer
		 * \tparam T The type of the value to be set
		 * \param value The value to be set
		 */
		template<typename T>
		void ModifyVarEditorNoPointer( const T value );

		/**
		 * \brief The name of the variable being kept
		 */
		std::string m_varName;

		/**
		 * \brief The editor type of the variable being kept
		 */
		EEditorType m_editorType;
	};

	/**
	 * \brief Serialized Variable to keep variable name and value serialized
	 * \tparam T The type to be kept
	 */
	template<typename T>
	struct SerializedVar : SerializedInterface
	{
#pragma region Basic constructors
		template<typename U = T>
		explicit SerializedVar( const std::string& name, typename std::enable_if<!std::is_pointer<U>::value>::type* = nullptr )
			: SerializedInterface( name, EDITABLE )
		{
		}

		template<typename U = T>
		explicit SerializedVar( const std::string& name, typename std::enable_if<std::is_pointer<U>::value>::type* = nullptr )
			: SerializedInterface( name, EDITABLE )
			, m_varValue( nullptr )
		{
		}

		/**
		 * \brief Basic constructor for non-pointer types
		 * \tparam U Is T, used for type_traits
		 * \param varValue _IN_ Reference to the variable to be stored
		 * \param varName _IN_ Name of the variable to be stored
		 */
		template<typename U = T>
		explicit SerializedVar( T& varValue, const std::string& varName, const EEditorType editorType, typename std::enable_if<!std::is_pointer<U>::value>::type* = nullptr )
			: SerializedInterface( varName, editorType )
			, m_varValue( varValue )
		{
		}

		/**
		 * \brief Basic constructor for pointer types
		 * \tparam U Is T, used for type_traits
		 * \param varValue _IN_ Reference to the pointer to be stored
		 * \param varName _IN_ Name of the pointer to be stored
		 * \note Value is copied, struct has no rights on the base pointer
		 */
		template<typename U = T>
		explicit SerializedVar( T& varValue, const std::string& varName, const EEditorType editorType, typename std::enable_if<std::is_pointer<U>::value>::type* = nullptr )
			: SerializedInterface( varName, editorType )
			, m_varValue( ( varValue == nullptr ) ? nullptr : new typename std::remove_pointer<T>::type( *varValue ) )
		{
		}
#pragma endregion

#pragma region Copy constructors
		/**
		 * \brief Copy constructor
		 * \param other _IN_ The SerializedVar to be the base of the copy
		 */
		explicit SerializedVar( const SerializedVar<T>& other )
			: SerializedInterface( other.m_varName, other.m_editorType )
		{
			CopyConstructor<T>( other );
		}

		/**
		 * \brief Specific function called using the Copy Constructor, for non-pointer types
		 * \tparam U Is T, used for type_traits
		 * \param other _IN_ The SerializedVar to be the base of the copy
		 */
		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, void>::type CopyConstructor( const SerializedVar<T>& other )
		{
			m_varValue = other.m_varValue;
		}

		/**
		 * \brief Specific function called using the Copy Constructor, for pointer types
		 * \tparam U Is T, used for type_traits
		 * \param other _IN_ The SerializedVar to be the base of the copy
		 * \note Value is copied, struct has no rights on the other pointer
		 */
		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, void>::type CopyConstructor( const SerializedVar<T>& other )
		{
			m_varValue = new typename std::remove_pointer<T>::type( *other.m_varValue );
		}
#pragma endregion

#pragma region Destructors
		virtual ~SerializedVar()
		{
			DeleteSerialized<T>();
		};

		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, void>::type DeleteSerialized()
		{
			// Do nothing
		}

		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, void>::type DeleteSerialized()
		{
			if ( m_varValue )
				delete m_varValue;
			m_varValue = nullptr;
		}
#pragma endregion

#pragma region File I/O

#pragma region File writing
		/**
		* \brief Write the data of this struct into the file
		* \param fileStream _IN_ The file in which is going to be written the data
		*/
		virtual bool WriteToFile( std::ostream& fileStream ) const override
		{
			// Write the editor type
			fileStream.write( reinterpret_cast<const char *>(&m_editorType), sizeof( EEditorType) );

			return _WriteDebugSwitchString<T>( fileStream );
			// Write data
			//return _WriteToFileTemplate<T>( fileIO );
		}

#pragma region Debug String Writing
		template<typename U = T, typename std::enable_if<std::is_same<U, std::string>::value>::type* = nullptr>
		bool _WriteDebugSwitchString( std::ostream& fileIO ) const
		{
			return _WriteDebugString<U>( fileIO );
		}

		template<typename U = T, typename std::enable_if<std::is_same<U, std::string *>::value>::type* = nullptr>
		bool _WriteDebugSwitchString( std::ostream& fileIO ) const
		{
			return _WriteDebugString<U>( fileIO );
		}

		template<typename U = T, typename std::enable_if<!std::is_same<U, std::string>::value>::type* = nullptr, typename std::enable_if<!std::is_same<U, std::string*>::value>::type* = nullptr>
		bool _WriteDebugSwitchString( std::ostream& fileIO ) const
		{
			return _WriteToFileTemplate<U>( fileIO );
		}

		template<typename U = T, typename std::enable_if<std::is_same<U, std::string>::value>::type* = nullptr>
		bool _WriteDebugString( std::ostream& fileIO ) const
		{
			size_t varLen = m_varValue.length();
			// Write the length of the name
			fileIO.write( reinterpret_cast<const char *>(&varLen), sizeof( size_t) );
			// Write the name of length -> len
			fileIO.write( reinterpret_cast<const char *>(m_varValue.c_str()), varLen );
			return true;
		}

		template<typename U = T, typename std::enable_if<std::is_same<U, std::string*>::value>::type* = nullptr>
		bool _WriteDebugString( std::ostream& fileIO ) const
		{
			size_t varLen = m_varValue->length();
			// Write the length of the name
			fileIO.write( reinterpret_cast<const char *>(&varLen), sizeof( size_t) );
			// Write the name of length -> len
			fileIO.write( reinterpret_cast<const char *>(m_varValue->c_str()), varLen );
			return true;
		}
#pragma endregion

		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, bool>::type _WriteToFileTemplate( std::ostream& fileIO ) const
		{
			fileIO.write( reinterpret_cast<const char *>(m_varValue), sizeof( typename std::remove_pointer<U>::type) );
			return true;
		}

		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, bool>::type _WriteToFileTemplate( std::ostream& fileIO ) const
		{
			fileIO.write( reinterpret_cast<const char *>(&m_varValue), sizeof( U) );
			return true;
		}
#pragma endregion

#pragma region File reading
		/**
		* \brief Read the data to be set in this struct from the file
		* \param fileIO _IN_ The file in which is going to be read the data
		*/
		virtual bool ReadFromFile( std::istream& fileIO ) override
		{
			// Read the editor type
			EEditorType editorType;
			fileIO.read( reinterpret_cast<char *>(&editorType), sizeof( EEditorType) );
			m_editorType = editorType;

			return _ReadDebugSwitchString<T>( fileIO );
			//return _ReadFromFileTemplate<T>( fileIO );
		}

#pragma region Debug String Writing
		template<typename U = T, typename std::enable_if<std::is_same<U, std::string>::value>::type* = nullptr>
		bool _ReadDebugSwitchString( std::istream& fileIO )
		{
			return _ReadDebugString<U>( fileIO );
		}

		template<typename U = T, typename std::enable_if<std::is_same<U, std::string *>::value>::type* = nullptr>
		bool _ReadDebugSwitchString( std::istream& fileIO )
		{
			return _ReadDebugString<U>( fileIO );
		}

		template<typename U = T, typename std::enable_if<!std::is_same<U, std::string>::value>::type* = nullptr, typename std::enable_if<!std::is_same<U, std::string*>::value>::type* = nullptr>
		bool _ReadDebugSwitchString( std::istream& fileIO )
		{
			return _ReadFromFileTemplate<U>( fileIO );
		}

		template<typename U = T, typename std::enable_if<std::is_same<U, std::string>::value>::type* = nullptr>
		bool _ReadDebugString( std::istream& fileIO )
		{
			// Read len of srting
			size_t len = 0;
			fileIO.read( reinterpret_cast<char *>(&len), sizeof( size_t) );

			// Read c_str() of string
			char* buff = new char[len + 1];
			memset( buff, 0, len + 1 );
			fileIO.read( reinterpret_cast<char *>(buff), len );

			std::string tmp( buff );
			delete[] buff;

			Set<T>( tmp );

			return true;
		}

		template<typename U = T, typename std::enable_if<std::is_same<U, std::string*>::value>::type* = nullptr>
		bool _ReadDebugString( std::istream& fileIO )
		{
			// Read len of srting
			size_t len = 0;
			fileIO.read( reinterpret_cast<char *>(&len), sizeof( size_t) );

			// Read c_str() of string
			char* buff = new char[len + 1];
			memset( buff, 0, len + 1 );
			fileIO.read( reinterpret_cast<char *>(buff), len );

			std::string tmp( buff );
			delete[] buff;

			Set<T>( tmp );

			return true;
		}
#pragma endregion

		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, bool>::type _ReadFromFileTemplate( std::istream& fileIO )
		{
			typename std::remove_pointer<U>::type valueRead;
			fileIO.read( reinterpret_cast<char *>(&valueRead), sizeof( typename std::remove_pointer<U>::type) );
			Set( valueRead );
			return true;
		}

		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, bool>::type _ReadFromFileTemplate( std::istream& fileIO )
		{
			U valueRead;
			fileIO.read( reinterpret_cast<char *>(&valueRead), sizeof( U) );
			Set( valueRead );
			return true;
		}
#pragma endregion

#pragma endregion

		/**
		 * \brief Returns wether the contained type is a pointer
		 * \return True if is pointer, false otherwise
		 */
		virtual bool IsPointer() const override
		{
			return std::is_pointer<T>::value;
		}

		/**
		 * \brief Get the current type name (without pointer)
		 * \return A string of the contained type name
		 */
		std::string GetPointerlessTypeName() const override
		{
			return typeid( std::remove_pointer<T>::type ).name();
		}

		/**
		 * \brief Get the current type name
		 * \return A string of the contained type name
		 */
		std::string GetTypeName() const override
		{
			return typeid( T).name();
		}

		/**
		 * \brief Duplicates this class using the class's copy constructor
		 * \return A duplicate of this class
		 */
		SerializedInterface* Duplicate() const override
		{
			return new SerializedVar<T>( *this );
		}

		/**
		 * \brief Get the value stored in the class
		 * \return The value stored
		 */
		T Get() const
		{
			return m_varValue;
		}

		/**
		 * \brief Get the value stored in the class
		 * \return The value stored
		 */
		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, typename std::remove_pointer<T>::type>::type GetPointerless() const
		{
			return m_varValue;
		}

		/**
		 * \brief Get the value stored in the class
		 * \return The value stored
		 */
		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, typename std::remove_pointer<T>::type>::type GetPointerless() const
		{
			return *m_varValue;
		}

		/**
		 * \brief Set the value stored in the class for non-pointer types
		 * \tparam U Is T, used for type_traits
		 * \param value _IN_ The value to be set
		 */
		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, void>::type Set( const U value )
		{
			m_varValue = value;
		}

		/**
		 * \brief Set the value stored in the class for pointer types
		 * \tparam U Is T, used for type_traits
		 * \param value _IN_ The value to be set
		 * \note Calls operator=() on T
		 */
		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, void>::type Set( const U value )
		{
			if ( m_varValue == nullptr )
				m_varValue = new typename std::remove_pointer<T>::type( *value );
			else
				*m_varValue = *value;
		}

		/**
		 * \brief Set the value stored in the class for pointer types
		 * \tparam U Is T, used for type_traits
		 * \param value _IN_ The value to be set
		 * \note Calls operator=() on T
		 */
		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, void>::type Set( const typename std::remove_pointer<U>::type value )
		{
			if ( m_varValue == nullptr )
				m_varValue = new typename std::remove_pointer<T>::type( value );
			else
				*m_varValue = value;
		}

		/**
		 * \brief Set a value to this SerializedInterface and modifies the referenced value
		 * \param value _IN_ The value to be set
		 */
		void ModifyVar( const T value );

		/**
		 * \brief Set a value to this SerializedInterface and modifies the referenced value if contained type is a pointer
		 * \tparam U Is T, used for type_traits
		 * \param value _IN_ The value to be set
		 */
		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, void>::type ModifyVarEditor( const typename std::remove_pointer<U>::type value );

		/**
		 * \brief Set a value to this SerializedInterface and modifies the referenced value if contained type is not a pointer
		 * \tparam U Is T, used for type_traits
		 * \param value _IN_ The value to be set
		 */
		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, void>::type ModifyVarEditor( const U value );

		/**
		 * \brief The modified value contained by the SerializedVar
		 * \note Does not modifies any game data
		 */
		T m_varValue;
	};

	/**
	 * \brief Serialized Variable Class to keep variable name, value and reference serialized
	 * \tparam T The type to be kept
	 */
	template<typename T>
	struct SerializedVarClass : SerializedVar<T>
	{
		SerializedVarClass( T& varRef, const std::string& varName, const SerializedInterface::EEditorType editorType )
			: SerializedVar<T>( varRef, varName, editorType )
			, m_varRef( varRef )
		{
		}

		virtual ~SerializedVarClass()
		{
		}

		/**
		 * \brief Apply the stored variable to the stored reference, modifying real value
		 * \tparam U Is T, used for type_traits
		 */
		template<typename U = T>
		typename std::enable_if<!std::is_pointer<U>::value, void>::type ApplyVar()
		{
			m_varRef = SerializedVar<T>::m_varValue;
		}

		/**
		 * \brief Apply the stored variable to the stored reference, modifying real value
		 * \tparam U Is T, used for type_traits
		 */
		template<typename U = T>
		typename std::enable_if<std::is_pointer<U>::value, void>::type ApplyVar()
		{
			if ( m_varRef == nullptr )
			{
				m_varRef = new typename std::remove_pointer<T>::type;
			}
			*m_varRef = *SerializedVar<T>::m_varValue;
		}

		/**
		 * \brief Apply the stored variable to the stored reference, modifying real value
		 * \tparam U Is T, used for type_traits
		 */
		template<typename U = T, typename std::enable_if<std::is_copy_constructible<typename std::remove_pointer<U>::value>::value, int>::type = 0>
		typename std::enable_if<std::is_pointer<U>::value, void>::type ApplyVar()
		{
			if ( m_varRef == nullptr )
				m_varRef = new typename std::remove_pointer<T>::type( *SerializedVar<T>::m_varValue );
			else
				*m_varRef = *SerializedVar<T>::m_varValue;
		}

		/**
		 * \brief Apply the stored variable to the stored reference, modifying real value
		 * \tparam U Is T, used for type_traits
		 */
		template<typename U = T, typename std::enable_if<!std::is_copy_constructible<typename std::remove_pointer<U>::value>::value, int>::type = 0>
		typename std::enable_if<std::is_pointer<U>::value, void>::type ApplyVar()
		{
			TCMFATALERROR( "YOU SHALL NOT PASS" );
		}

		T& m_varRef;
	};

	template<typename T>
	T SerializedInterface::Get() const
	{
		const SerializedVar<T>* casted = dynamic_cast<const SerializedVar<T>*>(this);
		if ( !casted )
		TCMFATALERROR( "Serialized var is null !" );
		return casted->Get();
	}

	template<typename T>
	typename std::enable_if<!std::is_pointer<T>::value, T>::type SerializedInterface::GetPointerless() const
	{
		if ( IsPointer() )
		{
			const SerializedVar<T*>* casted = dynamic_cast<const SerializedVar<T*>*>(this);
			if ( !casted )
			TCMFATALERROR( "Serialized var is null !" );
			return casted->GetPointerless();
		}
		else
		{
			const SerializedVar<T>* casted = dynamic_cast<const SerializedVar<T>*>(this);
			if ( !casted )
			TCMFATALERROR( "Serialized var is null !" );
			return casted->GetPointerless();
		}
	}

	template<typename T>
	void SerializedInterface::Set( const T value )
	{
		SerializedVar<T>* casted = dynamic_cast<SerializedVar<T>*>(this);
		if ( !casted )
		TCMFATALERROR( "Serialized var is null !" );
		return casted->Set( value );
	}

	template<typename T>
	void SerializedInterface::ModifyVar( const T value )
	{
		SerializedVar<T>* casted = dynamic_cast<SerializedVar<T>*>(this);
		if ( !casted )
		TCMFATALERROR( "Serialized var is null !" );
		casted->ModifyVar( value );
	}

	template<typename T>
	void SerializedInterface::ModifyVarEditorPointer( const T value )
	{
		SerializedVar<T*>* casted = dynamic_cast<SerializedVar<T*>*>(this);
		if ( !casted )
		TCMFATALERROR( "Serialized var is null !" );
		casted->ModifyVarEditor( value );
	}

	template<typename T>
	void SerializedInterface::ModifyVarEditorNoPointer( const T value )
	{
		ModifyVar( value );
	}

	template<typename T>
	void SerializedVar<T>::ModifyVar( const T value )
	{
		Set( value );

		SerializedVarClass<T>* serializedVar = dynamic_cast<SerializedVarClass<T>*>(this);
		if ( serializedVar == nullptr )
		TCMFATALERROR( "Serialized var is null !" );
		serializedVar->ApplyVar();
	}

	template<typename T>
	template<typename U>
	typename std::enable_if<std::is_pointer<U>::value, void>::type SerializedVar<T>::ModifyVarEditor( const typename std::remove_pointer<U>::type value )
	{
		Set( value );

		SerializedVarClass<T>* serializedVar = dynamic_cast<SerializedVarClass<T>*>(this);
		if ( serializedVar == nullptr )
		TCMFATALERROR( "Serialized var is null !" );
		serializedVar->ApplyVar();
	}

	template<typename T>
	template<typename U>
	typename std::enable_if<!std::is_pointer<U>::value, void>::type SerializedVar<T>::ModifyVarEditor( const U value )
	{
		ModifyVar( value );
	}
}

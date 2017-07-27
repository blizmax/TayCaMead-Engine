#pragma once
#include <string>
#include "EditorComponent.h"

#if WIN32
// ReSharper disable once CppUnusedIncludeDirective
#include <windows.h> // used by macro
#define CREATEDIR(path) CreateDirectory(path, NULL)
#define MOVEDIR(from, to) MoveFileEx(from, to, MOVEFILE_WRITE_THROUGH)
#define DELDIR(path) RemoveDirectory(path)
#else
	static_assert(false && "NO API FILE MANAGEMENT SELECTED")
#endif

namespace TCM
{
	namespace Editor
	{
		struct FileData
		{
			FileData() = delete;

			FileData( std::string path, std::string name ) : m_path( path )
			                                               , m_name( name )
			                                               , m_completePath( path + name )
			{
			}

			std::string m_path;
			std::string m_name;
			std::string m_completePath;
		};

		class FileManager abstract : public EditorComponent
		{
		public:
			FileManager()
			{
			};
			virtual ~FileManager() = default;

			void CreateFileFromEditor( FileData file ) const;
			void MoveFileFromEditor( FileData from, FileData to ) const;
			void DeleteFileFromEditor( FileData file ) const;

			void CreateDirectoryFromEditor( FileData file ) const;
			void MoveDirectoryFromEditor( FileData from, FileData to ) const;
			void DeleteDirectoryFromEditor( FileData file ) const;

			virtual void ShowFiles() = 0;
			virtual void ShowFolders() = 0;
		};
	}
}

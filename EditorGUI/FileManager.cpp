#include "FileManager.h"
#include <fstream>

namespace TCM
{
	namespace Editor
	{
		void FileManager::CreateFileFromEditor( FileData file ) const
		{
			std::ofstream stream( file.m_completePath, std::ios::out );
		}

		void FileManager::MoveFileFromEditor( FileData from, FileData to ) const
		{
			rename( from.m_name.data(), to.m_name.data() );
		}

		void FileManager::DeleteFileFromEditor( FileData file ) const
		{
			remove( file.m_name.data() );
		}

		void FileManager::CreateDirectoryFromEditor( FileData file ) const
		{
			std::wstring bigChar( file.m_name.begin(), file.m_name.end() );
			CREATEDIR( bigChar.data() );
		}

		void FileManager::MoveDirectoryFromEditor( FileData from, FileData to ) const
		{
			std::wstring bigFrom( from.m_name.begin(), from.m_name.end() );
			std::wstring bigTo( to.m_name.begin(), to.m_name.end() );
			MOVEDIR( bigFrom.data(), bigTo.data() );
		}

		void FileManager::DeleteDirectoryFromEditor( FileData file ) const
		{
			std::wstring bigChar( file.m_name.begin(), file.m_name.end() );
			DELDIR( bigChar.data() );
		}
	}
}

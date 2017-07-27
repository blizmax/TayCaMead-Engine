#pragma once

#include <vector>

#include "ObjectLoaderDefines.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			/**
			 * \brief Class used for loading Meshes
			 */
			class MeshLoader final
			{
			public:
				~MeshLoader() = default;

				/**
				 * \brief Loads objects from a file
				 * \param filepath _IN_ Path of the file to be parsed
				 * \return Scene object of every objects loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				static OBJECTLOADER_DLL_EXPORT Graphics::Skeleton* LoadMesh( const std::string& filepath );

				/**
				 * \brief Loads objects from a file
				 * \param directory _IN_ Path of the directory where the file to be parsed is loacted
				 * \param filename _IN_ Name of the file to be parsed
				 * \return Scene object of every objects loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				static OBJECTLOADER_DLL_EXPORT Graphics::Skeleton* LoadMesh( const std::string& directory, const std::string& filename );

			protected:
				MeshLoader() = default;

			private:
				/**
				 * \brief Get the unique instance of the MeshLoader
				 * \return A reference of the MeshLoader
				 */
				static MeshLoader& GetInstance();

				/**
				 * \brief Extensions supported by the MeshLoader
				 */
				enum FileType
				{
					OBJ,
					FBX,
					NONE
				};

				/**
				* \brief Loads objects from a file
				* \param filepath _IN_ Path of the file to be parsed
				* \return Scene object every objects loaded using the file\n
				*		   Returns nullptr if nothing could be loaded
				*/
				Graphics::Skeleton* CoreLoadMesh( const std::string& filepath ) const;

				/**
				* \brief Loads objects from a file
				* \param directory _IN_ Path of the directory where the file to be parsed is loacted
				* \param filename _IN_ Name of the file to be parsed
				* \return Scene object every objects loaded using the file\n
				*		   Returns nullptr if nothing could be loaded
				*/
				Graphics::Skeleton* CoreLoadMesh( const std::string& directory, const std::string& filename ) const;

				FileType GetFileType( const std::string& filename ) const;

				void ComputeTangents( std::vector<Graphics::VertexData *>* vecFragmentVertexObject, std::vector<Graphics::Fragment *>* vecFragmentObject ) const;
			};
		}
	}
}

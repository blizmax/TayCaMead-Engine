#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "MathLib/Vec2.h"
#include "MathLib/Vec3.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_Fragment;
			struct s_TCMAbstractMaterial;

			typedef struct s_FragmentVertexIdx final
			{
				uint16_t m_idxVertex;
				uint16_t m_idxNormal;
				uint16_t m_idxTexture;

				bool operator==( const s_FragmentVertexIdx& other ) const
				{
					if ( m_idxVertex == other.m_idxVertex
						&& m_idxNormal == other.m_idxNormal
						&& m_idxTexture == other.m_idxTexture )
						return true;
					return false;
				}

				std::string to_string() const
				{
					return std::to_string( m_idxVertex ) + " " +
							std::to_string( m_idxNormal ) + " " +
							std::to_string( m_idxTexture );
				}
			} FragmentVertexIdx;
		}

		namespace Resources
		{
			/**
			 * \brief Class used for loading .obj
			 */
			class TCMObjLoader final
			{
			protected:
				/**
				 * \brief Parameters supported by the TCMObjLoader
				 */
				enum ELineType : uint8_t
				{
					EMPTY = 0,
					V, // Vertex
					VT, // Vertex Texture
					VN, // Vertex Normal
					F, // Face
					O, // Object
					G, // Group
					MTLLIB,
					USEMTL,
					BADTYPE = 255,
				};

			public:
				~TCMObjLoader() = default;

				/**
				 * \brief Loads objects from an .obj file, using its proper directory
				 * \param directory _IN_ Directory where fileName is located
				 * \param fileName _IN_ Name of the file
				 * \return Vector of every objects loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				static Graphics::s_Skeleton* LoadFromFile( const std::string& directory, const std::string& fileName );

				/**
				 * \brief Loads objects from an .obj file
				 * \param path _IN_ Path of the file to be parsed
				 * \return Vector of every objects loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				static Graphics::s_Skeleton* LoadFromFile( const std::string& path );

			protected:
				TCMObjLoader();

				/**
				 * \brief Clear every member vector of the instance
				 */
				void ClearTemporaryData();

				/**
				 * \brief Parses an .obj file and adds objects to vector
				 * \param fileContent _IN_ Content of the file
				 * \param skeleton _OUT_ Vector of the objects to be returned
				 */
				void TCMObjLoader::ParseObjFile( std::string& fileContent, Graphics::Skeleton*& skeleton );

				/**
				 * \brief Get the type of the line parsed
				 * \param splitted _IN_ Current slitted line
				 * \return The type of the line\n
				 *		   Returns ELineType::BADTYPE if not supported
				 */
				ELineType GetLineType( const std::vector<std::string>& splitted ) const;

				/**
				 * \brief Parses one vertex
				 * \param splitted _IN_ Current splitted line
				 */
				void ParseVertex( const std::vector<std::string>& splitted );

				/**
				 * \brief Parses one normal
				 * \param splitted _IN_ Current splitted line
				 */
				void ParseNormal( const std::vector<std::string>& splitted );

				/**
				 * \brief Parses one texture
				 * \param splitted _IN_ Current splitted line
				 */
				void ParseTexture( const std::vector<std::string>& splitted );

				/**
				 * \brief Parses one face, three or four fragments
				 * \param splitted _IN_ Current splitted line
				 */
				void ParseFragment( const std::vector<std::string>& splitted );

				/**
				 * \brief Creates a square from four fragments
				 * \param first _IN_ Index of the first fragment
				 * \param second _IN_ Index of the second fragment
				 * \param third _IN_ Index of the third fragment
				 * \param fourth _IN_ Index of the fourth fragment
				 */
				void CreateQuad( const uint16_t first, const uint16_t second, const uint16_t third, const uint16_t fourth );

				/**
				 * \brief Creates a triangle from three fragments
				 * \param first _IN_ Index of the first fragment
				 * \param second _IN_ Index of the second fragment
				 * \param third _IN_ Index of the third fragment
				 */
				void CreateFragment( const uint16_t first, const uint16_t second, const uint16_t third );

				/**
				 * \brief Parses a fragment in a face line
				 * \param splitted _IN_ Current splitted line
				 * \param vertIdx _IN_ Place in the line of the fragment to be parsed
				 * \return Index of the parsed fragment
				 */
				uint16_t ParseFragmentVertex( const std::vector<std::string>& splitted,
				                              const uint8_t vertIdx );

				/**
				 * \brief Creates a fragment using its vertex, texture and normal
				 * \param idxVertex _IN_ Index of the vertex
				 * \param idxTexture _IN_ Index of the texture
				 * \param idxNormal _IN_ Index of the normal
				 * \return Index of the created fragment
				 */
				uint16_t CreateFragmentVertex( const uint16_t& idxVertex, const uint16_t& idxTexture, const uint16_t& idxNormal );

				/**
				 * \brief Parses a object line
				 * \param splitted _IN_ Current splitted line
				 * \param currentBone _IN_OUT_ Current Bone being modified
				 */
				void ParseObject( const std::vector<std::string>& splitted,
				                  Graphics::SubMesh*& currentBone );

				/**
				 * \brief Parses a group line
				 * \param splitted _IN_ Current splitted line
				 * \param currentBone _IN_OUT_ Current component being modified
				 */
				void ParseGroup( const std::vector<std::string>& splitted,
				                 Graphics::SubMesh*& currentBone );

				/**
				 * \brief Parses a mtllib line
				 * \param splittedLine _IN_ Current splitted line
				 * \param vecListMaterials _OUT_ Vector of materials loaded
				 */
				void ParseMtlLib( const std::vector<std::string>& splittedLine,
				                  std::vector<Graphics::s_TCMAbstractMaterial *>& vecListMaterials ) const;

				/**
				 * \brief Parses an usemtl line
				 * \param splittedLine _IN_ Current splitted line
				 * \param currentBone _IN_ Current bone which will use the material
				 * \param vecListMaterials _OUT_ Vector of materials loaded
				 */
				void ParseUseMtl( const std::vector<std::string>& splittedLine,
				                  Graphics::SubMesh* currentBone, const std::vector<Graphics::s_TCMAbstractMaterial *>& vecListMaterials ) const;

				/**
				 * \brief Set directory for future relative paths
				 * \param directory _IN_ Directory to be set as working directory
				 */
				void SetTemporaryObjDirectory( const std::string& directory );

				void ComputeTangents( std::vector<Graphics::VertexData *>* vecFragmentVertexObject, std::vector<Graphics::Fragment *>* vecFragmentObject ) const;

				void CreateBone( Graphics::SubMesh*& bone, const std::string& name );
			private:

				/**
				 * \brief Get the unique instance of the TCMObjLoader
				 * \return A reference of the TCMObjLoader
				 */
				static TCMObjLoader& GetInstance();

				Math::Bounds::Box GetBoundingBox( const size_t begin, const size_t count );

				/**
				 * \brief Directory in which the actual .obj is
				 */
				std::string m_objDirectory;

				/**
				 * \brief Vector of every vertex coordinates
				 * \note x, y, z
				 */
				std::vector<Math::Vec3> m_vecVertex;

				/**
				 * \brief Vector of every normal coordinates
				 * \note Nx, Ny, Nz
				 */
				std::vector<Math::Vec3> m_vecNormal;

				/**
				 * \brief Vector of every texture coordinates
				 * \note U, V
				 */
				std::vector<Math::Vec2> m_vecTexture;

				/**
				* \brief Vector of every fragment vertex
				* \note Possesses vertex, normal, texture, sent to the mesh
				*/
				std::vector<Graphics::VertexData *> m_vertexDataList;

				/**
				 * \brief Vector of every fragment vertex
				 * \note Possesses index to vertex, normal, texture
				 */
				std::vector<Graphics::FragmentVertexIdx *> m_vecFragmentVertex;

				/**
				 * \brief Vector of every fragment
				 * \note Possesses index to three fragment vertex, sent to the mesh
				 */
				std::vector<Graphics::s_Fragment *> m_vecFragment;

				/**
				 * \brief Map of every FragmentVertexIdx by index associated
				 */
				std::unordered_map<size_t, int> m_mapFragVertexIndex;

				size_t m_offset = 0;

				Math::Vec3 m_lowerLeftFront;

				Math::Vec3 m_upperRightBack;
			};
		}
	}
}

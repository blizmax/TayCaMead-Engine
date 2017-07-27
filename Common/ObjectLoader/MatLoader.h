#pragma once

#include <cstdint>
#include <vector>

#include "ObjectLoader/ObjectLoaderDefines.h"

#include "MathLib/ColorRGB.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_TCMAbstractMaterial;
			struct s_AbstractTexture;
		}

		namespace Resources
		{
			/**
			 * \brief Class used for loading Materials
			 */
			class OBJECTLOADER_DLL_EXPORT MatLoader final
			{
			public:
				/**
				 * \brief Loads Materials from an .obj file using its proper directory
				 * \param directory _IN_ Directory where fileName is located
				 * \param fileName _IN_ Name of the file
				 * \return Vector of every Material loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				static std::vector<Graphics::s_TCMAbstractMaterial *> LoadFromFile( const std::string& directory, const std::string& fileName );

				static Graphics::AbstractTexture* LoadTexFromFile( const std::string& path );

			protected:
				/**
				 * \brief Parameters supported by the TCMMatLoader
				 */
				enum ELineType : uint8_t
				{
					EMPTY = 0,
					NEWMTL, // New Material
					KA, // Ambient color
					KD, // Diffuse Color
					KS, // Specular Color
					KE, // Emissive Color
					TF, // Transmission Filter
					NI, // Optical Density
					NS, // Shininess
					D, // Alpha
					SHARPNESS, // Sharpness
					TR, // Transparency reverse
					ILLUM, // Light Param 

					MAP_KA, // Map Ambient color
					MAP_KD, // Map Diffuse Color
					MAP_KS, // Map Specular Color
					MAP_NS, // Map Shininess
					MAP_BUMP, // Map Normal
					MAP_D, // Map Alpha

					BADTYPE = 255
				};

			private:
				/**
				 * \brief Get the unique instance of the TCMMatLoader
				 * \return A reference of the TCMMatLoader
				 */
				static MatLoader& GetInstance();

				/**
				 * \brief Loads Materials from an .obj file using its proper directory
				 * \param directory _IN_ Directory where fileName is located
				 * \param fileName _IN_ Name of the file
				 * \return Vector of every Material loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				std::vector<Graphics::s_TCMAbstractMaterial *> ReadListMatFile( const std::string& directory, const std::string& fileName ) const;

				/**
				 * \brief Get the type of the line parsed
				 * \param lineheader _IN_ Line header
				 * \return The type of the line\n
				 *		   Returns ELineType::BADTYPE if not supported
				 */
				ELineType GetLineType( const char* lineheader ) const;

				/**
				 * \brief Read three floats of the file
				 * \param file _IN_ Filestream into the parsed file
				 * \return A Vec3 containing the three floats
				 */
				static Math::ColorRGB ReadColorRGB( FILE* file );

				/**
				 * \brief Read one float of the file
				 * \param file _IN_ Filestream into the parsed file
				 * \return The next float
				 */
				static float ReadFloat( FILE* file );

				/**
				 * \brief Read one int of the file
				 * \param file _IN_ Filestream into the parsed file
				 * \return The next int
				 */
				static int ReadInt( FILE* file );

				/**
				 * \brief Read one line of the file
				 * \param file _IN_ Filestream into the parsed file
				 * \return The actual line
				 * \note Line returned does not contain any ' ', '\\n' or '\\r'
				 */
				static std::string ReadString( FILE* file );

				/**
				 * \brief Read the map of a texture
				 * \param file _IN_ Filestream into the parsed file
				 * \param directory _IN_ Directory where's located the file
				 * \return New well-typed TCM::Engine::Graphics::AbstractTexture
				 *		   Returns nullptr if something wrong happened of Texture could not be loaded
				 */
				static Graphics::s_AbstractTexture* ReadMap( FILE* file, const std::string& directory );
			};
		}
	}
}

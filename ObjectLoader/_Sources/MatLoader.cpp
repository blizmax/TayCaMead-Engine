#include "ObjectLoader/MatLoader.h"

#include <memory>
#include <mutex>

#include "TCMLogger/Logger.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"
#include "TCMResourceManager/TCMResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "TCMRenderer/Rendering/RendererFactory.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			std::vector<Graphics::s_TCMAbstractMaterial*> MatLoader::LoadFromFile( const std::string& directory, const std::string& fileName )
			{
				return GetInstance().ReadListMatFile( directory, fileName );
			}

			Graphics::AbstractTexture* MatLoader::LoadTexFromFile( const std::string& path )
			{
				int width, height, type;

				uint8_t* texData = stbi_load( path.c_str(), &width, &height, &type, STBI_rgb_alpha );

				if ( texData == nullptr )
				{
					TCMWARNING( "Failed to load texture [" + std::string( path ) + "]" );
					return nullptr;
				}

				Graphics::AbstractTexture* texture = Graphics::RendererFactory::CreateTexture( type, width, height, texData );
				texture->SetKey( path );

				if ( texData )
					stbi_image_free( static_cast<void *>(texData) );

				return texture;
			}

			MatLoader& MatLoader::GetInstance()
			{
				static std::unique_ptr<MatLoader> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new MatLoader() );
				                }
				);
				return *( m_instance.get() );
			}

			std::vector<Graphics::s_TCMAbstractMaterial *> MatLoader::ReadListMatFile( const std::string& directory, const std::string& fileName ) const
			{
				std::vector<Graphics::AbstractMaterial *> vecMaterials;

				std::string path = directory + fileName;
				FILE* file;
				fopen_s( &file, path.c_str(), "r" );
				if ( file == nullptr )
				{
					TCMFAILURE("Could not open file [" + path + "]");
					return vecMaterials;
				}

				Graphics::AbstractMaterial* m = nullptr;

				while ( true )
				{
					char lineHeader[128];
					// read the first word of the line
					int res = fscanf_s( file, "%s", lineHeader, 128 );
					if ( res == EOF )
						break; // EOF = End Of File. Quit the loop.

					ELineType lineType = GetLineType( lineHeader );

					switch ( lineType )
					{
						case EMPTY:
							fgets( lineHeader, sizeof lineHeader, file );
							break;
						case NEWMTL:
							if ( m )
								m->UpdateData();
							m = Graphics::RendererFactory::CreateMaterial();
							m->SetName( ReadString( file ) );
							vecMaterials.push_back( m );
							break;
						case KA:
							m->SetAmbientColor( ReadColorRGB( file ) );
							break;
						case KD:
							m->SetDiffuseColor( ReadColorRGB( file ) );
							break;
						case KS:
							m->SetSpecularColor( ReadColorRGB( file ) );
							break;
						case KE:
							m->SetEmissiveColor( ReadColorRGB( file ) );
							break;
						case TF:
							m->SetTransmissionFilter( ReadColorRGB( file ) );
							break;
						case NI:
							m->SetOpticalDensity( ReadFloat( file ) );
							break;
						case NS:
							m->SetShininess( ReadFloat( file ) );
							break;
						case D:
							m->SetAlpha( ReadFloat( file ) );
							break;
						case SHARPNESS:
							m->SetSharpness( ReadInt( file ) );
							break;
						case TR:
							m->SetTransparentReverse( ReadFloat( file ) );
							break;
						case ILLUM:
							m->SetLightParam( ReadInt( file ) );
							break;
						case MAP_KA:
							m->SetAmbientMap( ReadMap( file, directory ) );
							break;
						case MAP_KD:
							m->SetDiffuseMap( ReadMap( file, directory ) );
							break;
						case MAP_KS:
							m->SetSpecularColorMap( ReadMap( file, directory ) );
							break;
						case MAP_NS:
							m->SetShininessMap( ReadMap( file, directory ) );
							break;
						case MAP_BUMP:
							m->SetBumpMap( ReadMap( file, directory ) );
							break;
						case MAP_D:
							m->SetAlphaMap( ReadMap( file, directory ) );
							break;
						case BADTYPE:
							TCMWARNING("Got a ELineType::BADTYPE ! [" + std::string(lineHeader) + "]");
							fgets( lineHeader, sizeof lineHeader, file );
							break;
						default:
							break;
					}
				}

				fclose( file );
				return vecMaterials;
			}

			MatLoader::ELineType MatLoader::GetLineType( const char* lineheader ) const
			{
				size_t strHash = std::hash<std::string>()( lineheader );
				if ( strHash == std::hash<std::string>()( "newmtl" ) ) return NEWMTL;
				else if ( strHash == std::hash<std::string>()( "Ka" ) ) return KA;
				else if ( strHash == std::hash<std::string>()( "Kd" ) ) return KD;
				else if ( strHash == std::hash<std::string>()( "Ks" ) ) return KS;
				else if ( strHash == std::hash<std::string>()( "Ke" ) ) return KE;
				else if ( strHash == std::hash<std::string>()( "Tf" ) ) return TF;
				else if ( strHash == std::hash<std::string>()( "Ni" ) ) return NI;
				else if ( strHash == std::hash<std::string>()( "Ns" ) ) return NS;
				else if ( strHash == std::hash<std::string>()( "d" ) ) return D;
				else if ( strHash == std::hash<std::string>()( "sharpness" ) ) return SHARPNESS;
				else if ( strHash == std::hash<std::string>()( "Tr" ) ) return TR;
				else if ( strHash == std::hash<std::string>()( "illum" ) ) return ILLUM;
				else if ( strHash == std::hash<std::string>()( "map_Ka" ) ) return MAP_KA;
				else if ( strHash == std::hash<std::string>()( "map_Kd" ) ) return MAP_KD;
				else if ( strHash == std::hash<std::string>()( "map_Ks" ) ) return MAP_KS;
				else if ( strHash == std::hash<std::string>()( "map_bump" ) ) return MAP_BUMP;
				else if ( strHash == std::hash<std::string>()( "map_d" ) ) return MAP_D;
				else if ( strHash == std::hash<std::string>()( "#" ) ) return EMPTY;

				return BADTYPE;
			}

			Math::ColorRGB MatLoader::ReadColorRGB( FILE* file )
			{
				float x, y, z = 0.0f;
				if ( fscanf_s( file, "%f %f %f", &x, &y, &z ) != 3 )
				TCMFAILURE("Not enough values read");
				return Math::ColorRGB( x, y, z );
			}

			float MatLoader::ReadFloat( FILE* file )
			{
				float a = 0.0f;
				if ( fscanf_s( file, "%f", &a ) != 1 )
				TCMFAILURE("Not enough values read");
				return a;
			}

			int MatLoader::ReadInt( FILE* file )
			{
				int a = 0;
				if ( fscanf_s( file, "%d", &a ) != 1 )
				TCMFAILURE("Not enough values read");
				return a;
			}

			std::string MatLoader::ReadString( FILE* file )
			{
				char buff[128];
				fgets( buff, sizeof( buff ), file );
				std::string str( buff );

				str.erase( remove( str.begin(), str.end(), ' ' ), str.end() );
				str.erase( remove( str.begin(), str.end(), '\n' ), str.end() );
				str.erase( remove( str.begin(), str.end(), '\r' ), str.end() );

				return str;
			}

			Graphics::s_AbstractTexture* MatLoader::ReadMap( FILE* file, const std::string& directory )
			{
				std::string path = directory + move( ReadString( file ) );
				return LoadTexFromFile( path );
			}
		}
	}
}

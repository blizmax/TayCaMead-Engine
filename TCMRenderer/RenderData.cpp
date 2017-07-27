#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "TCMRenderer/RHI/Generic/AbstractRenderer.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			s_FrameBuffer::s_FrameBuffer( FrameBufferType type )
				: m_type( type )
				, m_width( 0 )
				, m_height( 0 )
			{
			}

			s_FrameBuffer::s_FrameBuffer( FrameBufferType type, const int width, const int height, int count )
				: m_type( type )
				, m_count( count )
				, m_width( width )
				, m_height( height )
			{
			}

			void s_FrameBuffer::Resize( const int width, const int height )
			{
				m_width = width;
				m_height = height;
				CoreResize( m_width, m_height );
			}

			s_FrameBuffer::~s_FrameBuffer()
			{
				for ( int i = 0; i < mTextures.size(); i++ )
					delete mTextures[i];
			}

			void s_FrameBuffer::AttachTexture( TextureType type, AbstractTexture* texture )
			{
				mTypes.push_back( type );
				mTextures.push_back( texture );
			}

			AbstractTexture* s_FrameBuffer::GetTexture( int index ) const
			{
				return mTextures[index];
			}

			AbstractTexture* s_FrameBuffer::GetTextureOfType( TextureType type ) const
			{
				int i;
				for ( i = 0; i < mTypes.size(); i++ )
					if ( mTypes[i] == type )
						break;
				return i == mTypes.size() ? nullptr : mTextures[i];
			}

			FrameBufferType s_FrameBuffer::GetType() const
			{
				return m_type;
			}

			int s_FrameBuffer::GetCount() const
			{
				return m_count;
			}

			AbstractTexture* SpriteData::GetTexture() const
			{
				return m_texture;
			}

			void SpriteData::SetTexture( AbstractTexture* const texture )
			{
				m_texture = texture;
			}

			Math::Mat4 SpriteData::GetWorldMat() const
			{
				return m_worldMat;
			}

			void SpriteData::SetWorldMat( const Math::Mat4& worldMat )
			{
				m_worldMat = worldMat;
			}

			void SpriteData::Reset()
			{
				m_texture = nullptr;
				m_worldMat = Math::Mat4::Identity();
			}

			s_AbstractMesh::s_AbstractMesh( const uint32_t& indicesNum )
				: m_indicesCount( indicesNum )
			{
			}

			std::string s_AbstractMesh::GetKey() const
			{
				return m_key;
			}

			uint32_t s_AbstractMesh::GetIndicesCount() const
			{
				return m_indicesCount;
			}

			void s_AbstractMesh::SetBox( const Math::Bounds::Box& box )
			{
				m_box = box;
				m_sphere = box.ToSphere();
			}

			void s_AbstractMesh::SetSphere( const Math::Bounds::Sphere& sphere )
			{
				m_box = sphere.ToBox();
				m_sphere = sphere;
			}

			Math::Bounds::Box s_AbstractMesh::GetBox() const
			{
				return m_box;
			}

			Math::Bounds::Sphere s_AbstractMesh::GetSphere() const
			{
				return m_sphere;
			}

			uint32_t s_SubMesh::GetOffset() const
			{
				return m_offset;
			}

			uint32_t s_SubMesh::GetCount() const
			{
				return m_count;
			}

			void s_SubMesh::SetOffset( const uint32_t offset )
			{
				m_offset = offset;
			}

			void s_SubMesh::SetCount( const uint32_t count )
			{
				m_count = count;
			}

			void s_SubMesh::SetBox( const Math::Bounds::Box& box )
			{
				m_box = box;
				m_sphere = box.ToSphere();
			}

			Math::Bounds::Box s_SubMesh::GetBox() const
			{
				return m_box;
			}

			Math::Bounds::Sphere s_SubMesh::GetSphere() const
			{
				return m_sphere;
			}

			void s_SubMesh::SetSphere( const Math::Bounds::Sphere& sphere )
			{
				m_sphere = sphere;
				m_box = m_sphere.ToBox();
			}

			void s_SubMesh::SetParams( const uint32_t& offset, const uint32_t& count )
			{
				m_offset = offset;
				m_count = count;
			}

			AbstractMaterial* s_SubMesh::GetMaterial() const
			{
				return m_material;
			}

			void s_SubMesh::SetMaterial( AbstractMaterial* material )
			{
				m_material = material;
			}

			AbstractMesh* s_SubMesh::GetMesh() const
			{
				return mesh;
			}

			void s_SubMesh::SetMesh( AbstractMesh* mesh )
			{
				this->mesh = mesh;
			}

			std::string s_SubMesh::GetName() const
			{
				return m_name;
			}

			void s_SubMesh::SetName( const std::string& name )
			{
				m_name = name;
			}

			void s_AbstractMesh::SetKey( const std::string& key )
			{
				m_key = key;
			}

			RenderData::RenderData()
				: m_extra( nullptr )
				, m_mesh( nullptr )
				, m_material( nullptr )
				, m_shader( nullptr )
			{
			}

			void RenderData::SetMesh( AbstractMesh* mesh, const bool applyMeshValues )
			{
				m_mesh = mesh;
				if ( !m_mesh )
					return;

				if ( applyMeshValues )
				{
					m_fixedBoudingSphere = mesh->GetSphere();
					m_indicesOffset = 0;
					m_indicesCount = mesh->GetIndicesCount();
				}
				m_pivot = Math::Mat4::CreateTranslation( m_fixedBoudingSphere.position );
			}

			void RenderData::SetMesh( AbstractMesh* mesh, const Math::Bounds::Sphere& sphere )
			{
				m_mesh = mesh;
				m_fixedBoudingSphere = sphere;
				m_pivot = Math::Mat4::CreateTranslation( sphere.position );
			}

			void RenderData::SetMaterial( AbstractMaterial* material )
			{
				m_material = material;
			}

			void RenderData::SetShader( AbstractShader* shader )
			{
				m_shader = shader;
			}

			void RenderData::SetWorldMatrix( const Math::Mat4& parentMat, const Math::Mat4& localMat, const Math::Vec3& worldPos, const Math::Vec3& worldScale )
			{
				m_worldMat = localMat.Transposed() * Math::Mat4::NegateTranslate( m_pivot ) * parentMat.Transposed();
				m_worldMat.Transpose();

				float max = worldScale.x;
				if ( max < worldScale.y )
					max = worldScale.y;
				if ( max < worldScale.z )
					max = worldScale.z;
				CalcBoundingSphere( worldPos, max );
			}

			AbstractMesh* RenderData::GetMesh() const
			{
				return m_mesh;
			}

			AbstractMaterial* RenderData::GetMaterial() const
			{
				return m_material;
			}

			AbstractShader* RenderData::GetShader() const
			{
				return m_shader;
			}

			Math::Mat4 RenderData::GetWorldMat() const
			{
				return m_worldMat;
			}

			Math::Bounds::Sphere RenderData::GetBoundingSphere() const
			{
				return m_realBoudingSphere;
			}

			uint64_t RenderData::GetIndicesOffset() const
			{
				return m_indicesOffset;
			}

			void RenderData::SetIndicesOffset( const uint64_t offset )
			{
				m_indicesOffset = offset;
			}

			uint64_t RenderData::GetIndicesCount() const
			{
				return m_indicesCount;
			}

			void RenderData::SetIndicesCount( const uint64_t count )
			{
				m_indicesCount = count;
			}

			void RenderData::Reset()
			{
				m_extra = nullptr;

				m_mesh = nullptr;
				m_shader = nullptr;

				m_worldMat = Math::Mat4::Identity();
				m_pivot = m_worldMat;

				m_fixedBoudingSphere = Math::Bounds::Sphere();
				m_realBoudingSphere = m_fixedBoudingSphere;

				m_indicesOffset = 0;
				m_indicesCount = 0;
			}

			void RenderData::CalcBoundingSphere( const Math::Vec3& worldPosition, const float scaleMax )
			{
				if ( !m_mesh )
					return;

				m_realBoudingSphere.position = worldPosition;
				m_realBoudingSphere.radius = m_fixedBoudingSphere.radius * scaleMax;
			}

			bool s_LightData::IsDirectionnal() const
			{
				return m_direction.w == 0.0f;
			}

			float s_LightData::GetRadius() const
			{
				float lightMax = fmaxf( fmaxf( m_ambientColor.r, m_ambientColor.g ), m_ambientColor.b );
				float linear = m_linearAttenuation;
				float quadratic = m_quadraticAttenuation;
				float constant = m_constantAttenuation;
				return
						( -linear + sqrtf( linear * linear - 4.0f * quadratic * ( constant - ( 51.2f ) * lightMax ) ) )
						/ ( 2.0f * quadratic );
			}

			Math::Bounds::Sphere s_LightData::GetBoundingSphere() const
			{
				Math::Bounds::Sphere s;
				s.position = m_position;
				s.radius = GetRadius();
				return s;
			}

			AbstractTexture* s_TCMAbstractMaterial::GetAmbientMap() const
			{
				return m_maps.m_ambientMap;
			}

			void s_TCMAbstractMaterial::SetAmbientMap( AbstractTexture* const ambientMap )
			{
				m_maps.m_ambientMap = ambientMap;
				float old = m_ambientMapEnabled;
				m_ambientMapEnabled = ambientMap != nullptr ? 1.0f : 0.0f;

				if ( old != m_ambientMapEnabled )
					UpdateData();
			}

			AbstractTexture* s_TCMAbstractMaterial::GetDiffuseMap() const
			{
				return m_maps.m_diffuseMap;
			}

			void s_TCMAbstractMaterial::SetDiffuseMap( AbstractTexture* const diffuseMap )
			{
				m_maps.m_diffuseMap = diffuseMap;
				float old = m_diffuseMapEnabled;
				m_diffuseMapEnabled = diffuseMap != nullptr ? 1.0f : 0.0f;

				if ( old != m_diffuseMapEnabled )
					UpdateData();
			}

			AbstractTexture* s_TCMAbstractMaterial::GetSpecularColorMap() const
			{
				return m_maps.m_specularColorMap;
			}

			void s_TCMAbstractMaterial::SetSpecularColorMap( AbstractTexture* const specularColorMap )
			{
				m_maps.m_specularColorMap = specularColorMap;
				float old = m_specularMapEnabled;
				m_specularMapEnabled = specularColorMap != nullptr ? 1.0f : 0.0f;

				if ( old != m_specularMapEnabled )
					UpdateData();
			}

			AbstractTexture* s_TCMAbstractMaterial::GetShininessMap() const
			{
				return m_maps.m_shininessMap;
			}

			void s_TCMAbstractMaterial::SetShininessMap( AbstractTexture* const shininessMap )
			{
				m_maps.m_shininessMap = shininessMap;
				float old = m_shininessMapEnabled;
				m_shininessMapEnabled = shininessMap != nullptr ? 1.0f : 0.0f;

				if ( old != m_shininessMapEnabled )
					UpdateData();
			}

			AbstractTexture* s_TCMAbstractMaterial::GetBumpMap() const
			{
				return m_maps.m_bumpMap;
			}

			void s_TCMAbstractMaterial::SetBumpMap( AbstractTexture* const bumpMap )
			{
				m_maps.m_bumpMap = bumpMap;
				m_bumpMapEnabled = bumpMap != nullptr ? 1.0f : 0.0f;
			}

			AbstractTexture* s_TCMAbstractMaterial::GetAlphaMap() const
			{
				return m_maps.m_alphaMap;
			}

			void s_TCMAbstractMaterial::SetAlphaMap( AbstractTexture* const alphaMap )
			{
				m_maps.m_alphaMap = alphaMap;
				m_alphaMapEnabled = alphaMap != nullptr ? 1.0f : 0.0f;
			}

			s_TCMAbstractMaterial::s_TCMAbstractMaterial() : m_name( new std::string( "Mat_Default" ) )
			{
			}

			s_TCMAbstractMaterial::s_TCMAbstractMaterial( const s_TCMAbstractMaterial& other )
				: m_maps( other.m_maps )
				, m_name( other.m_name )
				, m_size( other.m_size )
				, _ambientColor( other._ambientColor )
				, m_ambientMapEnabled( other.m_ambientMapEnabled )
				, _diffuseColor( other._diffuseColor )
				, m_diffuseMapEnabled( other.m_diffuseMapEnabled )
				, _specularColor( other._specularColor )
				, m_specularMapEnabled( other.m_specularMapEnabled )
				, _emissiveColor( other._emissiveColor )
				, m_emissiveMapEnabled( other.m_emissiveMapEnabled )
				, _transmissionFilter( other._transmissionFilter )
				, _opticalDensity( other._opticalDensity )
				, _shininess( other._shininess )
				, m_shininessMapEnabled( other.m_shininessMapEnabled )
				, _alpha( other._alpha )
				, m_alphaMapEnabled( other.m_alphaMapEnabled )
				, m_bumpMapEnabled( other.m_bumpMapEnabled )
				, _sharpness( other._sharpness )
				, _transparentReverse( other._transparentReverse )
				, m_lightParam( other.m_lightParam )
			{
			}

			s_TCMAbstractMaterial& s_TCMAbstractMaterial::operator=( const s_TCMAbstractMaterial& other )
			{
				if ( this == &other )
					return *this;
				_ambientColor = other._ambientColor;
				m_ambientMapEnabled = other.m_ambientMapEnabled;
				_diffuseColor = other._diffuseColor;
				m_diffuseMapEnabled = other.m_diffuseMapEnabled;
				_specularColor = other._specularColor;
				m_specularMapEnabled = other.m_specularMapEnabled;
				_emissiveColor = other._emissiveColor;
				m_emissiveMapEnabled = other.m_emissiveMapEnabled;
				_transmissionFilter = other._transmissionFilter;
				_opticalDensity = other._opticalDensity;
				_shininess = other._shininess;
				m_shininessMapEnabled = other.m_shininessMapEnabled;
				_alpha = other._alpha;
				m_alphaMapEnabled = other.m_alphaMapEnabled;
				m_bumpMapEnabled = other.m_bumpMapEnabled;
				_sharpness = other._sharpness;
				_transparentReverse = other._transparentReverse;
				m_lightParam = other.m_lightParam;
				UpdateData();
				return *this;
			}

			void s_TCMAbstractMaterial::CopyData( const s_TCMAbstractMaterial& other )
			{
				m_maps = other.m_maps;
				_ambientColor = other._ambientColor;
				m_ambientMapEnabled = other.m_ambientMapEnabled;
				_diffuseColor = other._diffuseColor;
				m_diffuseMapEnabled = other.m_diffuseMapEnabled;
				_specularColor = other._specularColor;
				m_specularMapEnabled = other.m_specularMapEnabled;
				_emissiveColor = other._emissiveColor;
				m_emissiveMapEnabled = other.m_emissiveMapEnabled;
				_transmissionFilter = other._transmissionFilter;
				_opticalDensity = other._opticalDensity;
				_shininess = other._shininess;
				m_shininessMapEnabled = other.m_shininessMapEnabled;
				_alpha = other._alpha;
				m_alphaMapEnabled = other.m_alphaMapEnabled;
				m_bumpMapEnabled = other.m_bumpMapEnabled;
				_sharpness = other._sharpness;
				_transparentReverse = other._transparentReverse;
				m_lightParam = other.m_lightParam;

				UpdateData();
			}

			std::string s_TCMAbstractMaterial::GetName() const
			{
				return *m_name;
			}

			void s_TCMAbstractMaterial::SetName( const std::string& name ) const
			{
				m_name->assign( name );
			}

			Math::ColorRGB s_TCMAbstractMaterial::GetAmbientColor() const
			{
				return _ambientColor;
			}

			void s_TCMAbstractMaterial::SetAmbientColor( const Math::ColorRGB& ambientColor )
			{
				_ambientColor = ambientColor;
				UpdateData();
			}

			Math::ColorRGB s_TCMAbstractMaterial::GetDiffuseColor() const
			{
				return _diffuseColor;
			}

			void s_TCMAbstractMaterial::SetDiffuseColor( const Math::ColorRGB& diffuseColor )
			{
				_diffuseColor = diffuseColor;
				UpdateData();
			}

			Math::ColorRGB s_TCMAbstractMaterial::GetSpecularColor() const
			{
				return _specularColor;
			}

			void s_TCMAbstractMaterial::SetSpecularColor( const Math::ColorRGB& specularColor )
			{
				_specularColor = specularColor;
				UpdateData();
			}

			Math::ColorRGB s_TCMAbstractMaterial::GetEmissiveColor() const
			{
				return _emissiveColor;
			}

			void s_TCMAbstractMaterial::SetEmissiveColor( const Math::ColorRGB& emissiveColor )
			{
				_emissiveColor = emissiveColor;
				UpdateData();
			}

			Math::ColorRGB s_TCMAbstractMaterial::GetTransmissionFilter() const
			{
				return _transmissionFilter;
			}

			void s_TCMAbstractMaterial::SetTransmissionFilter( const Math::ColorRGB& transmissionFilter )
			{
				_transmissionFilter = transmissionFilter;
				UpdateData();
			}

			float s_TCMAbstractMaterial::GetOpticalDensity() const
			{
				return _opticalDensity;
			}

			void s_TCMAbstractMaterial::SetOpticalDensity( const float opticalDensity )
			{
				_opticalDensity = opticalDensity;
				UpdateData();
			}

			float s_TCMAbstractMaterial::GetShininess() const
			{
				return _shininess;
			}

			void s_TCMAbstractMaterial::SetShininess( const float shininess )
			{
				_shininess = shininess;
				UpdateData();
			}

			float s_TCMAbstractMaterial::GetAlpha() const
			{
				return _alpha;
			}

			void s_TCMAbstractMaterial::SetAlpha( const float alpha )
			{
				_alpha = alpha;
				UpdateData();
			}

			int s_TCMAbstractMaterial::GetSharpness() const
			{
				return _sharpness;
			}

			void s_TCMAbstractMaterial::SetSharpness( const int sharpness )
			{
				_sharpness = sharpness;
				UpdateData();
			}

			float s_TCMAbstractMaterial::GetTransparentReverse() const
			{
				return _transparentReverse;
			}

			void s_TCMAbstractMaterial::SetTransparentReverse( const float transparentReverse )
			{
				_transparentReverse = transparentReverse;
				UpdateData();
			}

			int s_TCMAbstractMaterial::GetLightParam() const
			{
				return m_lightParam;
			}

			void s_TCMAbstractMaterial::SetLightParam( const int lightParam )
			{
				m_lightParam = lightParam;
				UpdateData();
			}

			s_Skeleton::~s_Skeleton()
			{
				for ( int i = 0; i < m_bones.size(); ++i )
					delete m_bones[i];
				m_bones.clear();
			}

			void Skeleton::AddSubMesh( SubMesh* bone )
			{
				if ( bone == nullptr )
					return;
				m_bones.push_back( bone );
			}

			size_t Skeleton::GetSubMeshCount() const
			{
				return m_bones.size();
			}

			SubMesh* Skeleton::GetSubMesh( const int index )
			{
				if ( index < 0 || index >= m_bones.size() )
					return nullptr;

				return m_bones[index];
			}

			AbstractMesh* s_Skeleton::GetMesh() const
			{
				return m_mesh;
			}

			void s_Skeleton::SetMesh( AbstractMesh* mesh )
			{
				m_mesh = mesh;
			}

			s_AbstractTexture::s_AbstractTexture( const int type, const int width, const int height )
				: m_type( type )
				, m_width( width )
				, m_height( height )
			{
			}

			int s_AbstractTexture::GetWidth() const
			{
				return m_width;
			}

			void s_AbstractTexture::SetWidth( const int width )
			{
				m_width = width;
			}

			int s_AbstractTexture::GetHeight() const
			{
				return m_height;
			}

			void s_AbstractTexture::SetHeight( const int height )
			{
				m_height = height;
			}

			std::string s_AbstractTexture::GetKey() const
			{
				return m_key;
			}

			void s_AbstractTexture::SetKey( const std::string& key )
			{
				m_key = key;
			}

			int s_AbstractTexture::GetType() const
			{
				return m_type;
			}

			void s_AbstractTexture::SetType( const int type )
			{
				m_type = type;
			}
		}
	}
}

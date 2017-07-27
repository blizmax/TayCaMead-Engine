#pragma once

#include <cstdint>
#include <string>

#include "TCMRenderer/RendererDefines.h"

#include "MathLib/Vec3.h"
#include "MathLib/Vec4.h"
#include "MathLib/Bounds.h"
#include "MathLib/Vec2.h"
#include "MathLib/Mat4.h"
#include "MathLib/ColorRGB.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Camera
			{
				struct CameraData;
			}

			/**
			 * \brief List of PostProcessEffect's supported
			 */
			enum class TCMRENDERER_DLL PostProcessEffect : int
			{
				BLOOM_EFFECT = 0,
				TONE_MAPPING,
				GRAYSCALE,
				INVERTED,
				BLUR,
				GAUSS_BLUR,
				KERNEL_EFFECT,
				EDGE,
				COUNT,
				NONE = -1
			};

			/**
			 * \brief Structure containing data for one VertexData
			 * \note Vec3 Vertex, Vec3 Normal, Vce2 Texture, Vec3 Tangent
			 */
			typedef struct TCMRENDERER_DLL s_VertexData
			{
				/**
				 * \brief Vec3 Vertex
				 */
				Math::Vec3 m_vertex;

				/**
				 * \brief Vec3 Vertex Normal
				 */
				Math::Vec3 m_normal;

				/**
				 * \brief Vec2 Vertex Texture
				 */
				Math::Vec2 m_texture;

				/**
				 * \brief Vec3 Vertex Tangent
				 */
				Math::Vec3 m_tangent;
			} VertexData;

			/**
			 * \brief One face, contains indices to three VertexData
			 */
			typedef struct TCMRENDERER_DLL s_Fragment
			{
				/**
				 * \brief Indices of the associated VertexData
				 */
				uint16_t m_idxFragmentVertex[3];
			} Fragment;

			/**
			* \brief Type of texture supported
			*/
			enum class TextureType : int
			{
				Color0 = 0,
				Color1,
				Color2,
				Color3,
				Color4,
				Color5,
				Color6,
				Color7,
				Color8,
				Color9,
				Color10,
				Depth,
				Stencil,
				Depth_Stencil,
				Count
			};

			// Special behavior for ++Colors
			inline TextureType& operator++( TextureType& c )
			{
				using IntType = std::underlying_type<TextureType>::type;
				c = static_cast<TextureType>(static_cast<IntType>(c) + 1);
				if ( c == TextureType::Count )
					c = static_cast<TextureType>(0);
				return c;
			}

			// Special behavior for Colors++
			inline TextureType operator++( TextureType& c, int )
			{
				TextureType result = c;
				++c;
				return result;
			}

			// Special behavior for Colors++
			inline TextureType operator+( TextureType c, int count )
			{
				using IntType = std::underlying_type<TextureType>::type;
				c = static_cast<TextureType>(static_cast<IntType>(c) + count);
				if ( c == TextureType::Count )
					c = static_cast<TextureType>(0);
				return c;
			}

			/**
			* \brief Structure containing the data needed for one Texture
			*/
			typedef struct s_AbstractTexture
			{
				s_AbstractTexture() = default;

				s_AbstractTexture( const int type, const int width, const int height );

				virtual ~s_AbstractTexture() = default;

				TCMRENDERER_DLL int GetWidth() const;
				void SetWidth( const int width );

				TCMRENDERER_DLL int GetHeight() const;
				void SetHeight( const int height );

				TCMRENDERER_DLL std::string GetKey() const;
				TCMRENDERER_DLL void SetKey( const std::string& key );

				int GetType() const;
				void SetType( const int type );

			protected:
				int m_type = 0;

				int m_width = 0, m_height = 0;

				/**
				* \brief The key/name of this texture
				*/
				std::string m_key = "Tex_Default";
			} AbstractTexture;

			enum class FrameBufferType : uint8_t
			{
				NONE,
				DEFERRED,
				MSAA
			};

			/**
			* \brief Abstract structure encapsulating FrameBuffers for easier implementation
			*/
			typedef struct s_FrameBuffer
			{
				s_FrameBuffer( FrameBufferType type = FrameBufferType::NONE );
				s_FrameBuffer( FrameBufferType type, const int width, const int height, int count = 1 );

				/**
				* \brief Bind framebuffer to render on it
				*/
				virtual void BindBuffer() const = 0;
				/**
				* \brief Unbind framebuffer to stop to render on it
				*/
				virtual void UnbindBuffer() = 0;
				/**
				* \brief Clear the buffer (color, depht, stencil)
				* \param color _IN_ Color of the buffer after the clear
				*/
				virtual void ClearBuffer( Math::ColorRGB color = { 0.0f, 0.0f, 0.0f } ) = 0;

				void Resize( const int width, const int height );
				virtual void CoreResize( const int width, const int height ) const = 0;
				virtual ~s_FrameBuffer();

				/**
				* \brief Generate one texture and attach it to the framebuffer
				* \param type _IN_ TextureType of the texture
				* \param width _IN_ Width of the texture
				* \param height _IN_ Height of the texture
				* \return TCM::Engine::Graphics::AbstractTexture created
				*/
				virtual AbstractTexture* GenTexture( TextureType type, int width, int height ) = 0;

				/**
				* \brief Generate one texture and attach it to the framebuffer
				* \param type _IN_ TextureType of the texture
				* \param width _IN_ Width of the texture
				* \param height _IN_ Height of the texture
				* \return TCM::Engine::Graphics::AbstractTexture created
				*/
				virtual AbstractTexture* ResizeTexture( TextureType type, int width, int height ) = 0;

				/**
				* \brief Attach texture already created to the framebuffer
				* \param type _IN_ TextureType of the texture
				* \param texture _IN_ TCM::Engine::Graphics::AbstractTexture to attach
				*/
				virtual void AttachTexture( TextureType type, AbstractTexture* texture );

				/**
				* \brief Return texture at index, attached to the framebuffer
				* \param index _IN_ Index of the texture
				* \return Texture found
				*/
				AbstractTexture* GetTexture( int index = 0 ) const;
				/**
				* \brief Return first texture of TextureType type, attached to the framebuffer
				* \param type _IN_ TextureType of the texture
				* \return Texture found
				*/
				AbstractTexture* GetTextureOfType( TextureType type ) const;

				/**
				* \brief Get the ID of the framebuffer
				* \return ID of the framebuffer
				*/
				virtual int GetID() const = 0;

				FrameBufferType GetType() const;

				int GetCount() const;

			protected:

				virtual int ConvertType( TextureType t ) const = 0;

				std::vector<TextureType> mTypes;
				std::vector<AbstractTexture *> mTextures;

				FrameBufferType m_type = FrameBufferType::NONE;
				int m_count = 1;

				int m_width;
				int m_height;
			} FrameBuffer;

			/**
			 * \brief Structure containing the data needed for one light
			 */
			typedef struct TCMRENDERER_DLL s_LightData
			{
				s_LightData( const int width, const int height ): m_shadowFrameBuffer( nullptr )
				                                                , m_constantAttenuation( 0 )
				                                                , m_linearAttenuation( 0 )
				                                                , m_quadraticAttenuation( 0 )
				                                                , m_cutOff( 0 )
				                                                , m_outerCutOff( 0 )
				{
				}

				s_LightData& operator=( const s_LightData& other )
				{
					if ( this == &other )
						return *this;

					m_direction = other.m_direction;
					m_position = other.m_position;
					m_constantAttenuation = other.m_constantAttenuation;
					m_ambientColor = other.m_ambientColor;
					m_linearAttenuation = other.m_linearAttenuation;
					m_diffuseColor = other.m_diffuseColor;
					m_quadraticAttenuation = other.m_quadraticAttenuation;
					m_specularColor = other.m_specularColor;
					m_cutOff = other.m_cutOff;
					m_outerCutOff = other.m_outerCutOff;

					return *this;
				}

				s_LightData& operator=( s_LightData&& other ) noexcept = delete;

				virtual s_LightData::~s_LightData()
				{
					delete m_shadowFrameBuffer;
				};

				friend bool operator==( const s_LightData& lhs, const s_LightData& rhs )
				{
					return lhs.m_constantAttenuation == rhs.m_constantAttenuation
							&& lhs.m_ambientColor == rhs.m_ambientColor
							&& lhs.m_linearAttenuation == rhs.m_linearAttenuation
							&& lhs.m_diffuseColor == rhs.m_diffuseColor
							&& lhs.m_quadraticAttenuation == rhs.m_quadraticAttenuation
							&& lhs.m_specularColor == rhs.m_specularColor
							&& lhs.m_cutOff == rhs.m_cutOff
							&& lhs.m_outerCutOff == rhs.m_outerCutOff;
				}

				friend bool operator!=( const s_LightData& lhs, const s_LightData& rhs )
				{
					return !( lhs == rhs );
				}

				virtual void UpdateData() = 0;

				bool m_enabled = true;

				/**
				* \brief FrameBuffer for shadow mapping
				*/
				FrameBuffer* m_shadowFrameBuffer;

				/**
				 * \brief Size of the structure
				 * \note Change it if you add more data
				 */
				const uint32_t m_size = sizeof(Math::Mat4) + sizeof(Math::Vec4) + sizeof(Math::Vec3)
				+ sizeof ( Math::ColorRGB) * 3 + sizeof(float) * 5;

				Math::Mat4 m_lightSpace;
				/**
				 * \brief Direction of the light
				 */
				Math::Vec4 m_direction;

				/**
				 * \brief Position of the light
				 */
				Math::Vec3 m_position;

				/**
				 * \brief Constant Attenuation of the light
				 */
				float m_constantAttenuation;

				/**
				 * \brief Ambient color of the light
				 */
				Math::ColorRGB m_ambientColor;

				/**
				 * \brief Linear Attenuation of the light
				 */
				float m_linearAttenuation;

				/**
				 * \brief Diffuse color of the light
				 */
				Math::ColorRGB m_diffuseColor;

				/**
				 * \brief Quadratic Attenuation of the light
				 */
				float m_quadraticAttenuation;

				/**
				 * \brief Specular color of the light
				 */
				Math::ColorRGB m_specularColor;

				/**
				 * \brief Inner shadow distance
				 */
				float m_cutOff;

				/**
				 * \brief Outer shadow distance
				 */
				float m_outerCutOff;

				/**
				 * \brief Tells if the light is directionnal or not
				 * \return True if is directionnal, false otherwise
				 */
				bool IsDirectionnal() const;

				/**
				 * \brief Get the Radius of the light, where it has effect
				 * \return Radius of the light
				 */
				float GetRadius() const;

				/**
				 * \brief Get the Bouding Sphere of the light
				 * \return Bounds::Sphere of the light
				 */
				Math::Bounds::Sphere GetBoundingSphere() const;
			} LightData;

			/**
			 * \brief Structure containing the data needed for one Shader
			 */
			typedef struct TCMRENDERER_DLL s_AbstractShader
			{
				virtual void Bind() const = 0;

				virtual s_AbstractShader::~s_AbstractShader()
				{
				};

				bool m_isDeferred = false;
			} AbstractShader;

			/**
			 * \brief Structure containing the data needed for one Skybox
			 */
			typedef struct TCMRENDERER_DLL s_AbstractSkybox
			{
				virtual s_AbstractSkybox::~s_AbstractSkybox()
				{
				}
			} AbstractSkybox;

#pragma region Material

			/**
			 * \brief Structure containing the maps of a Material
			 */
			typedef struct TCMRENDERER_DLL s_TCMMatMapContainer
			{
				/**
				 * \brief Ka - ID of the ambient map of the object
				 */
				AbstractTexture* m_ambientMap = nullptr;

				/**
				 * \brief Kd - ID of the diffuse map of the object
				 */
				AbstractTexture* m_diffuseMap = nullptr;

				/**
				 * \brief Ks - ID of the specular color map of the object
				 */
				AbstractTexture* m_specularColorMap = nullptr;

				/**
				 * \brief Ns - ID of the specular highlight map of the object
				 */
				AbstractTexture* m_shininessMap = nullptr;

				/**
				 * \brief bump - ID of the bump map of the object
				 */
				AbstractTexture* m_bumpMap = nullptr;

				/**
				 * \brief d - ID of the alpha map of the object
				 */
				AbstractTexture* m_alphaMap = nullptr;
			} TCMMatMapContainer;

			/**
			 * \brief Structure containing the data needed for one Material
			 */
			typedef struct TCMRENDERER_DLL s_TCMAbstractMaterial
			{
				s_TCMAbstractMaterial();

				virtual s_TCMAbstractMaterial::~s_TCMAbstractMaterial()
				{
					delete m_name;
				}

				s_TCMAbstractMaterial( const s_TCMAbstractMaterial& other );

				s_TCMAbstractMaterial& operator=( const s_TCMAbstractMaterial& other );

				friend bool operator==( const s_TCMAbstractMaterial& lhs, const s_TCMAbstractMaterial& rhs )
				{
					return lhs._ambientColor == rhs._ambientColor
							&& lhs.m_ambientMapEnabled == rhs.m_ambientMapEnabled
							&& lhs._diffuseColor == rhs._diffuseColor
							&& lhs.m_diffuseMapEnabled == rhs.m_diffuseMapEnabled
							&& lhs._specularColor == rhs._specularColor
							&& lhs.m_specularMapEnabled == rhs.m_specularMapEnabled
							&& lhs._emissiveColor == rhs._emissiveColor
							&& lhs.m_emissiveMapEnabled == rhs.m_emissiveMapEnabled
							&& lhs._transmissionFilter == rhs._transmissionFilter
							&& lhs._opticalDensity == rhs._opticalDensity
							&& lhs._shininess == rhs._shininess
							&& lhs.m_shininessMapEnabled == rhs.m_shininessMapEnabled
							&& lhs._alpha == rhs._alpha
							&& lhs.m_alphaMapEnabled == rhs.m_alphaMapEnabled
							&& lhs.m_bumpMapEnabled == rhs.m_bumpMapEnabled
							&& lhs._sharpness == rhs._sharpness
							&& lhs._transparentReverse == rhs._transparentReverse
							&& lhs.m_lightParam == rhs.m_lightParam;
				}

				friend bool operator!=( const s_TCMAbstractMaterial& lhs, const s_TCMAbstractMaterial& rhs )
				{
					return !( lhs == rhs );
				}

				void CopyData( const s_TCMAbstractMaterial& other );

				std::string GetName() const;
				void SetName( const std::string& name ) const;
				Math::ColorRGB GetAmbientColor() const;
				void SetAmbientColor( const Math::ColorRGB& ambientColor );
				Math::ColorRGB GetDiffuseColor() const;
				void SetDiffuseColor( const Math::ColorRGB& diffuseColor );
				Math::ColorRGB GetSpecularColor() const;
				void SetSpecularColor( const Math::ColorRGB& specularColor );
				Math::ColorRGB GetEmissiveColor() const;
				void SetEmissiveColor( const Math::ColorRGB& emissiveColor );
				Math::ColorRGB GetTransmissionFilter() const;
				void SetTransmissionFilter( const Math::ColorRGB& transmissionFilter );
				float GetOpticalDensity() const;
				void SetOpticalDensity( const float opticalDensity );
				float GetShininess() const;
				void SetShininess( const float shininess );
				float GetAlpha() const;
				void SetAlpha( const float alpha );
				int GetSharpness() const;
				void SetSharpness( const int sharpness );
				float GetTransparentReverse() const;
				void SetTransparentReverse( const float transparentReverse );
				int GetLightParam() const;
				void SetLightParam( const int lightParam );

				AbstractTexture* GetAmbientMap() const;
				void SetAmbientMap( AbstractTexture* const ambientMap );
				AbstractTexture* GetDiffuseMap() const;
				void SetDiffuseMap( AbstractTexture* const diffuseMap );
				AbstractTexture* GetSpecularColorMap() const;
				void SetSpecularColorMap( AbstractTexture* const specularColorMap );
				AbstractTexture* GetShininessMap() const;
				void SetShininessMap( AbstractTexture* const shininessMap );
				AbstractTexture* GetBumpMap() const;
				void SetBumpMap( AbstractTexture* const bumpMap );
				AbstractTexture* GetAlphaMap() const;
				void SetAlphaMap( AbstractTexture* const alphaMap );

				virtual void UpdateData() = 0;

			protected:
				/**
				 * \brief Map for every texture
				 */
				TCMMatMapContainer m_maps;

				/**
				 * \brief Name of this material
				 */
				std::string* m_name = nullptr;

				/**
				 * \brief Size of the actual structure sent to the API
				 */
				const uint32_t m_size = sizeof(Math::ColorRGB) * 5 + sizeof(float) * 10 + sizeof(int);

			public :

				/**
				 * \brief Ka - The color globally emited by the object
				 */
				Math::ColorRGB _ambientColor = { 0.f, 0.f, 0.f };

			protected:
				/**
				 * \brief Whether the ambiant map is enabled
				 */
				float m_ambientMapEnabled = 0;

			public:
				/**
				 * \brief Kd - The color diffused by the material
				 */
				Math::ColorRGB _diffuseColor = { 1.f, 1.f, 1.f };

			protected:
				/**
				 * \brief Whether the diffuse map is enabled
				 */
				float m_diffuseMapEnabled = 0;

			public:
				/**
				 * \brief Ks - The surface more white on object that reflect
				 */
				Math::ColorRGB _specularColor = { 0.f, 0.f, 0.f };

			protected:
				/**
				 * \brief Whether the specular map is enabled
				 */
				float m_specularMapEnabled = 0;

			public:
				/**
				 * \brief Ke - The color emitted by the object
				 */
				Math::ColorRGB _emissiveColor = { 0.f, 0.f, 0.f };

			protected:
				/**
				 * \brief Whether the emissive map is enabled
				 */
				float m_emissiveMapEnabled = 0;

			public:
				/**
				 * \brief Tf - Which color the material lets pass through or being reflected
				 */
				Math::ColorRGB _transmissionFilter = { 1.f, 1.f, 1.f };

				/**
				 * \brief Ni - Index of refraction
				 * \note From 0.001 to 10
				 */
				float _opticalDensity = 1.0f;

				/**
				 * \brief Ns - The range of the specular light
				 * \note From 0.001 to 10
				 */
				float _shininess = 10.0f;

			protected:
				/**
				 * \brief Whether the shininess map is enabled
				 */
				float m_shininessMapEnabled = 0;
			public:
				/**
				 * \brief d - Transparency
				 * \note From 0.0 to 1.0
				 */
				float _alpha = 1.0f;

			protected:
				/**
				 * \brief Whether the alpha map is enabled or not
				 */
				float m_alphaMapEnabled = 0;

				/**
				 * \brief Whether the bump map is enabled or not
				 */
				float m_bumpMapEnabled = 0;
			public:
				/**
				 * \brief Sharpness
				 * \note From 0.001 to 1000
				 */
				int _sharpness = 60;

				/**
				 * \brief Transparency reverse
				 */
				float _transparentReverse = 0;

				/**
				 * \brief Illumination
				 */
				int m_lightParam = -1;
			} AbstractMaterial;
#pragma endregion

			/**
			 * \brief Structure used to send extra debug data
			 */
			typedef struct TCMRENDERER_DLL s_ExtraRenderData
			{
				/**
				 * \brief Cast shadows
				 */
				bool m_castShadows = true;

				/**
				 * \brief Receive Shadows
				 */
				bool m_receiveShadows = true;
				/**
				 * \brief Is Object Selected inside the Editor
				 */
				bool m_isSelected = false;
			} ExtraRenderData;

			enum class TCMRENDERER_DLL SpriteAnchor : int
			{
				CENTER = 0,
				LEFT,
				RIGHT,
				TOP,
				BOTTOM,
				TOP_LEFT,
				TOP_RIGHT,
				BOTTOM_LEFT,
				BOTTOM_RIGHT
			};

			typedef struct TCMRENDERER_DLL s_SpriteTransform
			{
				/**
				* \brief Position of the object
				*/
				Math::Vec3 position = Math::Vec3( 0.0f, 0.0f, 0.0f );

				/**
				* \brief Euler angles of the object
				*/
				Math::Vec3 eulerAngles;

				/**
				* \brief Scale of the object
				*/
				Math::Vec3 scale = Math::Vec3( 1.0f, 1.0f, 1.0f );

				inline void Reset()
				{
					position = Math::Vec3( 0.0f, 0.0f, 0.0f );
					eulerAngles = Math::Vec3( 0.0f, 0.0f, 0.0f );
					scale = Math::Vec3( 1.0f, 1.0f, 1.0f );
				}
			} TCMSpriteTransform;

			typedef struct s_CameraSpecificData
			{
				virtual ~s_CameraSpecificData()
				{
				}

				virtual void UpdateData( Camera::CameraData* data ) = 0;
			} CameraSpecificData;

			class TCMRENDERER_DLL SpriteData
			{
			public:
				Graphics::SpriteData() = default;

				AbstractTexture* GetTexture() const;
				void SetTexture( AbstractTexture* const texture );

				Math::Mat4 GetWorldMat() const;
				void SetWorldMat( const Math::Mat4& worldMat );

				void Reset();

				bool m_enabled = true;

			private:
				/**
				 * \brief Texture to render
				 */
				AbstractTexture* m_texture;

				/**
				 * \brief World matrix of the object
				 */
				Math::Mat4 m_worldMat;
			};

			/**
			* \brief Structure containing the data needed for one Mesh
			*/
			typedef struct s_AbstractMesh
			{
				s_AbstractMesh( const uint32_t& indicesNum );
				virtual ~s_AbstractMesh() = default;

				/**
				 * \brief Set key of the mesh
				 */
				TCMRENDERER_DLL void SetKey( const std::string& key );

				TCMRENDERER_DLL std::string GetKey() const;

				TCMRENDERER_DLL uint32_t GetIndicesCount() const;

				/**
				* \brief Set bounding box of the mesh
				* \note Calculate the bounding sphere too
				*/
				TCMRENDERER_DLL void SetBox( const Math::Bounds::Box& box );

				/**
				* \brief Set bounding sphere of the mesh
				* \note Calculate the bounding box too
				*/
				TCMRENDERER_DLL void SetSphere( const Math::Bounds::Sphere& sphere );

				TCMRENDERER_DLL Math::Bounds::Box GetBox() const;
				TCMRENDERER_DLL Math::Bounds::Sphere GetSphere() const;

			protected:

				std::string m_key;

				/**
				* \brief Number of indices in the IBO
				*/
				uint32_t m_indicesCount = 0;

				Math::Bounds::Box m_box;
				Math::Bounds::Sphere m_sphere;
			} AbstractMesh;

			typedef struct s_SubMesh
			{
				s_SubMesh() = default;

				/**
				* \brief Set bounding box of the mesh
				* \note Calculate the bounding sphere too
				*/
				TCMRENDERER_DLL void SetBox( const Math::Bounds::Box& box );

				/**
				* \brief Set bounding sphere of the mesh
				* \note Calculate the bounding box too
				*/
				TCMRENDERER_DLL void SetSphere( const Math::Bounds::Sphere& sphere );

				TCMRENDERER_DLL Math::Bounds::Box GetBox() const;
				TCMRENDERER_DLL Math::Bounds::Sphere GetSphere() const;

				TCMRENDERER_DLL void SetParams( const uint32_t& offset, const uint32_t& count );

				TCMRENDERER_DLL AbstractMaterial* GetMaterial() const;
				TCMRENDERER_DLL void SetMaterial( AbstractMaterial* material );

				TCMRENDERER_DLL AbstractMesh* GetMesh() const;
				TCMRENDERER_DLL void SetMesh( AbstractMesh* mesh );

				TCMRENDERER_DLL std::string GetName() const;
				TCMRENDERER_DLL void SetName( const std::string& name );

				TCMRENDERER_DLL uint32_t GetOffset() const;
				TCMRENDERER_DLL uint32_t GetCount() const;

				TCMRENDERER_DLL void SetOffset( const uint32_t offset );
				TCMRENDERER_DLL void SetCount( const uint32_t count );

			protected:
				AbstractMesh* mesh = nullptr;
				AbstractMaterial* m_material = nullptr;

				uint32_t m_offset = 0;
				uint32_t m_count = 0;

				Math::Bounds::Box m_box;
				Math::Bounds::Sphere m_sphere;

				std::string m_name;
			} SubMesh;

			typedef struct s_Skeleton
			{
				TCMRENDERER_DLL ~s_Skeleton();

				TCMRENDERER_DLL void AddSubMesh( SubMesh* bone );
				TCMRENDERER_DLL size_t GetSubMeshCount() const;
				TCMRENDERER_DLL SubMesh* GetSubMesh( const int index );

				TCMRENDERER_DLL AbstractMesh* GetMesh() const;
				TCMRENDERER_DLL void SetMesh( AbstractMesh* mesh );

			protected:

				AbstractMesh* m_mesh = nullptr;
				std::vector<SubMesh *> m_bones;
			} Skeleton;

			/**
			 * \brief Structure containing data for rendering objects
			 */
			class TCMRENDERER_DLL RenderData
			{
			public:
				RenderData::RenderData();

				virtual RenderData::~RenderData() = default;

				/**
				* \brief Sets the Mesh of this RenderData
				* \param mesh The new Mesh to be contained
				* \param applyMeshValues _IN_OPT_ Use mesh indice count
				* \note Old Mesh is not automatically deleted
				*/
				void SetMesh( AbstractMesh* mesh, const bool applyMeshValues = true );

				/**
				 * \brief Sets the Mesh of this RenderData
				 * \param mesh The new Mesh to be contained
				 * \param sphere The bouding sphere of the submesh
				 * \note Old Mesh is not automatically deleted
				 */
				void SetMesh( AbstractMesh* mesh, const Math::Bounds::Sphere& sphere );

				/**
				* \brief Sets the Material of this RenderData
				* \param material The new Material to be contained
				* \note Old Material is not automatically deleted
				*/
				void SetMaterial( AbstractMaterial* material );

				/**
				 * \brief Sets the Shader of this RenderData
				 * \param shader The new Shader to be contained
				 * \note Old Shader is not automatically deleted
				 */
				void SetShader( AbstractShader* shader );

				void SetWorldMatrix( const Math::Mat4& parentMat, const Math::Mat4& localMat, const Math::Vec3& worldPos, const Math::Vec3& worldScale );

				/**
				 * \brief Get the Mesh contained by this RenderData
				 * \return The contained Mesh
				 */
				AbstractMesh* GetMesh() const;

				/**
				* \brief Get the Material contained by this RenderData
				* \return The contained Material
				*/
				AbstractMaterial* GetMaterial() const;

				/**
				 * \brief Get the Shader contained by this RenderData
				 * \return The contained Shader
				 */
				AbstractShader* GetShader() const;

				/**
				* \brief Get the world matrix contained by this RenderData
				* \return The world matrix
				*/
				Math::Mat4 GetWorldMat() const;

				/**
				* \brief Get the bounding sphere of the render data
				* \return The world matrix
				*/
				Math::Bounds::Sphere GetBoundingSphere() const;

				uint64_t GetIndicesOffset() const;
				void SetIndicesOffset( const uint64_t offset );
				uint64_t GetIndicesCount() const;
				void SetIndicesCount( const uint64_t count );

				void Reset();

				void* m_caller = nullptr;

				/**
				 * \brief Extra data to be sent if needed
				 */
				ExtraRenderData* m_extra;

				bool m_enabled = true;

			protected:
				/**
				 * \brief Mesh data
				 */
				AbstractMesh* m_mesh;

				/**
				* \brief Material data
				*/
				AbstractMaterial* m_material;

				/**
				 * \brief Shader data
				 */
				AbstractShader* m_shader;

				void CalcBoundingSphere( const Math::Vec3& worldPosition, const float scaleMax );

				uint64_t m_indicesOffset = 0;
				uint64_t m_indicesCount = 0;

				Math::Bounds::Sphere m_fixedBoudingSphere;
				Math::Bounds::Sphere m_realBoudingSphere;

				Math::Mat4 m_worldMat;
				Math::Mat4 m_pivot;
			};

			struct RenderDataComparer
			{
				virtual ~RenderDataComparer() = default;

				virtual bool operator()( const RenderData* dataLeft, const RenderData* dataRight ) const
				{
					return true;
				};
			};
		}
	}
}

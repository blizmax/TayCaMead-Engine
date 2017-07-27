#include "ObjectLoader/PrimitiveLoader.h"
#include <sstream>

#include "TCMLogger/Logger.h"
#include "MathLib/Quat.h"
#include "MathLib/MathUtils.h"
#include <mutex>
#include "TCMRenderer/Rendering/RendererFactory.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			TCMPrimitiveLoader& TCMPrimitiveLoader::GetInstance()
			{
				static std::unique_ptr<TCMPrimitiveLoader> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new TCMPrimitiveLoader() );
				                }
				);
				return *( m_instance.get() );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenSphere( float radius, int rings, int slices )
			{
				return GetInstance().CoreGenSphere( radius, rings, slices );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenTube( float height, int nbSides, float bottomRadius1, float bottomRadius2, float topRadius1, float topRadius2 )
			{
				return GetInstance().CoreGenTube( height, nbSides, bottomRadius1, bottomRadius2, topRadius1, topRadius2 );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenCone( float height, int nbSides, float bottomRadius, float topRadius )
			{
				return GetInstance().CoreGenCone( height, nbSides, bottomRadius, topRadius );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenTorus( float radius1, float radius2, int nbRadSeg, int nbSides )
			{
				return GetInstance().CoreGenTorus( radius1, radius2, nbRadSeg, nbSides );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenCapsule( float height, float radius, int nbSides )
			{
				return GetInstance().CoreGenCapsule( height, radius, nbSides );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenPlane( float length, float width, int resX, int resZ )
			{
				return GetInstance().CoreGenPlane( length, width, resX, resZ );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::GenBox( float length, float width, float height )
			{
				return GetInstance().CoreGenBox( length, width, height );
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenSphere( const float& radius, const int nbLong, const int nbLat ) const
			{
				std::ostringstream stringStream;
				stringStream << "Sphere " << radius << " " << nbLong << " " << nbLat;
				std::string name = stringStream.str();

				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( ( nbLong + 1 ) * nbLat + 2 );

				//vecFragmentObject.reserve(faces * 2);

				std::vector<Math::Vec3> vertices;
				vertices.resize( ( nbLong + 1 ) * nbLat + 2 );
				float _pi = Math::Utils::PI;
				float _2pi = _pi * 2.0f;

				vertices[0] = Math::Vec3::Up() * radius;
				for ( int lat = 0; lat < nbLat; lat++ )
				{
					float a1 = _pi * static_cast<float>(lat + 1) / ( nbLat + 1 );
					float sin1 = sin( a1 );
					float cos1 = cos( a1 );

					for ( int lon = 0; lon <= nbLong; lon++ )
					{
						float a2 = _2pi * static_cast<float>(lon == nbLong ? 0 : lon) / nbLong;
						float sin2 = sin( a2 );
						float cos2 = cos( a2 );

						vertices[lon + lat * ( nbLong + 1 ) + 1] = Math::Vec3( sin1 * cos2, cos1, sin1 * sin2 ) * radius;
					}
				}
				vertices[vertices.size() - 1] = Math::Vec3::Up() * -radius;

				std::vector<Math::Vec3> normales;
				normales.resize( vertices.size() );
				for ( int n = 0; n < vertices.size(); n++ )
					normales[n] = vertices[n].Normalized();

				std::vector<Math::Vec2> uvs;
				uvs.resize( vertices.size() );
				uvs[0] = Math::Vec2::Up();
				uvs[vertices.size() - 1] = Math::Vec2();
				for ( int lat = 0; lat < nbLat; lat++ )
					for ( int lon = 0; lon <= nbLong; lon++ )
						uvs[lon + lat * ( nbLong + 1 ) + 1] = Math::Vec2( static_cast<float>(lon) / nbLong, 1.0f - static_cast<float>(lat + 1) / ( nbLat + 1 ) );

				for ( int i = 0; i < vertices.size(); i++ )
				{
					Graphics::VertexData* fragmentVertex = new Graphics::VertexData();
					fragmentVertex->m_vertex = vertices[i];
					fragmentVertex->m_normal = normales[i];
					fragmentVertex->m_texture = uvs[i];
					vecFragmentVertexObject.push_back( fragmentVertex );
				}

				size_t nbFaces = vertices.size();
				size_t nbTriangles = nbFaces * 2;

				std::vector<Graphics::Fragment *> vecFragmentObject;
				vecFragmentObject.reserve( nbTriangles );
				//Top Cap
#ifdef _DEBUG
				if ( nbLong >= UINT16_MAX )
				TCMDEBUG("nbLong is greater than uint16_t max. Could lead to unexpected behaviour [1]");
#endif
				for ( size_t lon = 0; lon < nbLong; lon++ )
				{
					Graphics::Fragment* frag = new Graphics::Fragment();
					frag->m_idxFragmentVertex[0] = static_cast<uint16_t>(lon + 2);
					frag->m_idxFragmentVertex[1] = static_cast<uint16_t>(lon + 1);
					frag->m_idxFragmentVertex[2] = 0;
					vecFragmentObject.push_back( frag );
				}

				//Middle
				for ( size_t lat = 0; lat < nbLat - 1; lat++ )
				{
					for ( size_t lon = 0; lon < nbLong; lon++ )
					{
						size_t current = lon + lat * ( nbLong + 1 ) + 1;
						size_t next = current + nbLong + 1;

#ifdef _DEBUG
						if ( current >= UINT16_MAX )
						TCMDEBUG("Current is greater than uint16_t max. Could lead to unexpected behaviour");
						if ( next >= UINT16_MAX )
						TCMDEBUG("Next is greater than uint16_t max. Could lead to unexpected behaviour");
#endif

						Graphics::Fragment* frag1 = new Graphics::Fragment();
						frag1->m_idxFragmentVertex[0] = static_cast<uint16_t>(current);
						frag1->m_idxFragmentVertex[1] = static_cast<uint16_t>(current + 1);
						frag1->m_idxFragmentVertex[2] = static_cast<uint16_t>(next + 1);
						vecFragmentObject.push_back( frag1 );

						Graphics::Fragment* frag2 = new Graphics::Fragment();
						frag2->m_idxFragmentVertex[0] = static_cast<uint16_t>(current);
						frag2->m_idxFragmentVertex[1] = static_cast<uint16_t>(next + 1);
						frag2->m_idxFragmentVertex[2] = static_cast<uint16_t>(next);
						vecFragmentObject.push_back( frag2 );
					}
				}

				//Bottom Cap
#ifdef _DEBUG
				if ( nbLong >= UINT16_MAX )
				TCMDEBUG("nbLong is greater than uint16_t max. Could lead to unexpected behaviour [2]");
#endif
				for ( size_t lon = 0; lon < nbLong; lon++ )
				{
					Graphics::Fragment* frag = new Graphics::Fragment();
					frag->m_idxFragmentVertex[0] = static_cast<uint16_t>(vertices.size() - 1);
					frag->m_idxFragmentVertex[1] = static_cast<uint16_t>(vertices.size() - ( lon + 2 ) - 1);
					frag->m_idxFragmentVertex[2] = static_cast<uint16_t>(vertices.size() - ( lon + 1 ) - 1);
					vecFragmentObject.push_back( frag );
				}

				ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Bounds::Sphere sphere;
				sphere.radius = radius * 2;
				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );
				mesh->SetSphere( sphere );

				return mesh;
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenTube( float height, int nbSides, float bottomRadius1, float bottomRadius2,
			                                                         float topRadius1, float topRadius2 ) const
			{
				std::ostringstream stringStream;
				stringStream << "Tube " << height << " " << nbSides << " " << bottomRadius1 << " " << bottomRadius2 << " " << topRadius1 << " " << topRadius2;
				std::string name = stringStream.str();

				int nbVerticesCap = nbSides * 2 + 2;
				int nbVerticesSides = nbSides * 2 + 2;

				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( nbVerticesCap * 2 + nbVerticesSides * 2 );

				// bottom + top + sides
				std::vector<Math::Vec3> vertices;
				vertices.resize( nbVerticesCap * 2 + nbVerticesSides * 2 );
				int vert = 0;
				float _2pi = Math::Utils::PI * 2.0f;

				// Bottom cap
				int sideCounter = 0;
				while ( vert < nbVerticesCap )
				{
					sideCounter = sideCounter == nbSides ? 0 : sideCounter;

					float r1 = static_cast<float>(sideCounter++) / nbSides * _2pi;
					float vcos = cos( r1 );
					float vsin = sin( r1 );
					vertices[vert] = Math::Vec3( vcos * ( bottomRadius1 - bottomRadius2 * .5f ), 0.0f, vsin * ( bottomRadius1 - bottomRadius2 * .5f ) );
					vertices[vert + 1] = Math::Vec3( vcos * ( bottomRadius1 + bottomRadius2 * .5f ), 0.0f, vsin * ( bottomRadius1 + bottomRadius2 * .5f ) );
					vert += 2;
				}

				// Top cap
				sideCounter = 0;
				while ( vert < nbVerticesCap * 2 )
				{
					sideCounter = sideCounter == nbSides ? 0 : sideCounter;

					float r1 = static_cast<float>(sideCounter++) / nbSides * _2pi;
					float vcos = cos( r1 );
					float vsin = sin( r1 );
					vertices[vert] = Math::Vec3( vcos * ( topRadius1 - topRadius2 * .5f ), height, vsin * ( topRadius1 - topRadius2 * .5f ) );
					vertices[vert + 1] = Math::Vec3( vcos * ( topRadius1 + topRadius2 * .5f ), height, vsin * ( topRadius1 + topRadius2 * .5f ) );
					vert += 2;
				}

				// Sides (out)
				sideCounter = 0;
				while ( vert < nbVerticesCap * 2 + nbVerticesSides )
				{
					sideCounter = sideCounter == nbSides ? 0 : sideCounter;

					float r1 = static_cast<float>(sideCounter++) / nbSides * _2pi;
					float vcos = cos( r1 );
					float vsin = sin( r1 );

					vertices[vert] = Math::Vec3( vcos * ( topRadius1 + topRadius2 * .5f ), height, vsin * ( topRadius1 + topRadius2 * .5f ) );
					vertices[vert + 1] = Math::Vec3( vcos * ( bottomRadius1 + bottomRadius2 * .5f ), 0, vsin * ( bottomRadius1 + bottomRadius2 * .5f ) );
					vert += 2;
				}

				// Sides (in)
				sideCounter = 0;
				while ( vert < vertices.size() )
				{
					sideCounter = sideCounter == nbSides ? 0 : sideCounter;

					float r1 = static_cast<float>(sideCounter++) / nbSides * _2pi;
					float vcos = cos( r1 );
					float vsin = sin( r1 );

					vertices[vert] = Math::Vec3( vcos * ( topRadius1 - topRadius2 * .5f ), height, vsin * ( topRadius1 - topRadius2 * .5f ) );
					vertices[vert + 1] = Math::Vec3( vcos * ( bottomRadius1 - bottomRadius2 * .5f ), 0, vsin * ( bottomRadius1 - bottomRadius2 * .5f ) );
					vert += 2;
				}

				// bottom + top + sides
				std::vector<Math::Vec3> normales;
				normales.resize( vertices.size() );
				vert = 0;

				// Bottom cap
				while ( vert < nbVerticesCap )
				{
					normales[vert++] = Math::Vec3::Up() * -1;
				}

				// Top cap
				while ( vert < nbVerticesCap * 2 )
				{
					normales[vert++] = Math::Vec3::Up();
				}

				// Sides (out)
				sideCounter = 0;
				while ( vert < nbVerticesCap * 2 + nbVerticesSides )
				{
					sideCounter = sideCounter == nbSides ? 0 : sideCounter;

					float r1 = static_cast<float>(sideCounter++) / nbSides * _2pi;

					normales[vert] = Math::Vec3( cos( r1 ), 0.0f, sin( r1 ) );
					normales[vert + 1] = normales[vert];
					vert += 2;
				}

				// Sides (in)
				sideCounter = 0;
				while ( vert < vertices.size() )
				{
					sideCounter = sideCounter == nbSides ? 0 : sideCounter;

					float r1 = static_cast<float>(sideCounter++) / nbSides * _2pi;

					normales[vert] = ( Math::Vec3( cos( r1 ), 0.0f, sin( r1 ) ) ) * -1;
					normales[vert + 1] = normales[vert];
					vert += 2;
				}

				std::vector<Math::Vec2> uvs;
				uvs.resize( vertices.size() );

				vert = 0;
				// Bottom cap
				sideCounter = 0;
				while ( vert < nbVerticesCap )
				{
					float t = static_cast<float>(sideCounter++) / nbSides;
					uvs[vert++] = Math::Vec2( 0.0f, t );
					uvs[vert++] = Math::Vec2( 1.0f, t );
				}

				// Top cap
				sideCounter = 0;
				while ( vert < nbVerticesCap * 2 )
				{
					float t = static_cast<float>(sideCounter++) / nbSides;
					uvs[vert++] = Math::Vec2( 0.0f, t );
					uvs[vert++] = Math::Vec2( 1.0f, t );
				}

				// Sides (out)
				sideCounter = 0;
				while ( vert < nbVerticesCap * 2 + nbVerticesSides )
				{
					float t = static_cast<float>(sideCounter++) / nbSides;
					uvs[vert++] = Math::Vec2( t, 0.0f );
					uvs[vert++] = Math::Vec2( t, 1.0f );
				}

				// Sides (in)
				sideCounter = 0;
				while ( vert < vertices.size() )
				{
					float t = static_cast<float>(sideCounter++) / nbSides;
					uvs[vert++] = Math::Vec2( t, 0.0f );
					uvs[vert++] = Math::Vec2( t, 1.0f );
				}

				for ( int i = 0; i < vertices.size(); i++ )
				{
					Graphics::VertexData* fragmentVertex = new Graphics::VertexData();
					fragmentVertex->m_vertex = vertices[i];
					fragmentVertex->m_normal = normales[i];
					fragmentVertex->m_texture = uvs[i];
					vecFragmentVertexObject.push_back( fragmentVertex );
				}

				int nbFace = nbSides * 4;
				int nbTriangles = nbFace * 2;
				std::vector<Graphics::Fragment *> vecFragmentObject;
				vecFragmentObject.reserve( nbTriangles );

				// Bottom cap
				sideCounter = 0;
				while ( sideCounter < nbSides )
				{
					int current = sideCounter * 2;
					int next = sideCounter * 2 + 2;

					Graphics::Fragment* frag1 = new Graphics::Fragment();
					frag1->m_idxFragmentVertex[0] = next + 1;
					frag1->m_idxFragmentVertex[1] = next;
					frag1->m_idxFragmentVertex[2] = current;

					Graphics::Fragment* frag2 = new Graphics::Fragment();
					frag2->m_idxFragmentVertex[0] = current + 1;
					frag2->m_idxFragmentVertex[1] = next + 1;
					frag2->m_idxFragmentVertex[2] = current;

					vecFragmentObject.push_back( frag1 );
					vecFragmentObject.push_back( frag2 );

					sideCounter++;
				}

				// Top cap
				while ( sideCounter < nbSides * 2 )
				{
					int current = sideCounter * 2 + 2;
					int next = sideCounter * 2 + 4;

					Graphics::Fragment* frag1 = new Graphics::Fragment();
					frag1->m_idxFragmentVertex[0] = current;
					frag1->m_idxFragmentVertex[1] = next;
					frag1->m_idxFragmentVertex[2] = next + 1;

					Graphics::Fragment* frag2 = new Graphics::Fragment();
					frag2->m_idxFragmentVertex[0] = current;
					frag2->m_idxFragmentVertex[1] = next + 1;
					frag2->m_idxFragmentVertex[2] = current + 1;

					vecFragmentObject.push_back( frag1 );
					vecFragmentObject.push_back( frag2 );

					sideCounter++;
				}

				// Sides (out)
				while ( sideCounter < nbSides * 3 )
				{
					int current = sideCounter * 2 + 4;
					int next = sideCounter * 2 + 6;

					Graphics::Fragment* frag1 = new Graphics::Fragment();
					frag1->m_idxFragmentVertex[0] = current;
					frag1->m_idxFragmentVertex[1] = next;
					frag1->m_idxFragmentVertex[2] = next + 1;

					Graphics::Fragment* frag2 = new Graphics::Fragment();
					frag2->m_idxFragmentVertex[0] = current;
					frag2->m_idxFragmentVertex[1] = next + 1;
					frag2->m_idxFragmentVertex[2] = current + 1;

					vecFragmentObject.push_back( frag1 );
					vecFragmentObject.push_back( frag2 );

					sideCounter++;
				}

				// Sides (in)
				while ( sideCounter < nbSides * 4 )
				{
					int current = sideCounter * 2 + 6;
					int next = sideCounter * 2 + 8;

					Graphics::Fragment* frag1 = new Graphics::Fragment();
					frag1->m_idxFragmentVertex[0] = next + 1;
					frag1->m_idxFragmentVertex[1] = next;
					frag1->m_idxFragmentVertex[2] = current;

					Graphics::Fragment* frag2 = new Graphics::Fragment();
					frag2->m_idxFragmentVertex[0] = current + 1;
					frag2->m_idxFragmentVertex[1] = next + 1;
					frag2->m_idxFragmentVertex[2] = current;

					vecFragmentObject.push_back( frag1 );
					vecFragmentObject.push_back( frag2 );

					sideCounter++;
				}

				ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Vec3 box;
				box.y = height;
				float radius = topRadius1 < bottomRadius1 ? bottomRadius1 : topRadius1;
				box.x = radius * 2;
				box.z = box.x;

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );
				mesh->SetBox( Math::Bounds::Box::FromVec3( box ) );

				return mesh;
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenCone( float height, int nbSides, float bottomRadius, float topRadius ) const
			{
				std::ostringstream stringStream;
				stringStream << "Cone " << height << " " << nbSides << " " << bottomRadius << " " << topRadius;
				std::string name = stringStream.str();

				int nbVerticesCap = nbSides + 1;

				// bottom + top + sides
				std::vector<Math::Vec3> vertices;
				vertices.resize( nbVerticesCap + nbVerticesCap + nbSides * 2 + 2 );
				int vert = 0;
				float _2pi = Math::Utils::PI * 2.0f;

				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( nbVerticesCap + nbVerticesCap + nbSides * 2 + 2 );

				// Bottom cap
				vertices[vert++] = Math::Vec3();
				while ( vert <= nbSides )
				{
					float rad = static_cast<float>(vert) / nbSides * _2pi;
					vertices[vert] = Math::Vec3( cos( rad ) * bottomRadius, 0.0f, sin( rad ) * bottomRadius );
					vert++;
				}

				// Top cap
				vertices[vert++] = Math::Vec3( 0.0f, height, 0.0f );
				while ( vert <= nbSides * 2 + 1 )
				{
					float rad = static_cast<float>(vert - nbSides - 1) / nbSides * _2pi;
					vertices[vert] = Math::Vec3( cos( rad ) * topRadius, height, sin( rad ) * topRadius );
					vert++;
				}

				// Sides
				int v = 0;
				while ( vert <= vertices.size() - 4 )
				{
					float rad = static_cast<float>(v) / nbSides * _2pi;
					vertices[vert] = Math::Vec3( cos( rad ) * topRadius, height, sin( rad ) * topRadius );
					vertices[vert + 1] = Math::Vec3( cos( rad ) * bottomRadius, 0, sin( rad ) * bottomRadius );
					vert += 2;
					v++;
				}
				vertices[vert] = vertices[nbSides * 2 + 2];
				vertices[vert + 1] = vertices[nbSides * 2 + 3];

				// bottom + top + sides
				std::vector<Math::Vec3> normales;
				normales.resize( vertices.size() );
				vert = 0;

				// Bottom cap
				while ( vert <= nbSides )
				{
					normales[vert++] = Math::Vec3::Up() * -1;
				}

				// Top cap
				while ( vert <= nbSides * 2 + 1 )
				{
					normales[vert++] = Math::Vec3::Up();
				}

				// Sides
				v = 0;
				while ( vert <= vertices.size() - 4 )
				{
					float rad = static_cast<float>(v) / nbSides * _2pi;
					float vcos = cos( rad );
					float vsin = sin( rad );

					normales[vert] = Math::Vec3( vcos, 0.0f, vsin );
					normales[vert + 1] = normales[vert];

					vert += 2;
					v++;
				}
				normales[vert] = normales[nbSides * 2 + 2];
				normales[vert + 1] = normales[nbSides * 2 + 3];

				std::vector<Math::Vec2> uvs;
				uvs.resize( vertices.size() );

				// Bottom cap
				int u = 0;
				uvs[u++] = Math::Vec2( 0.5f, 0.5f );
				while ( u <= nbSides )
				{
					float rad = static_cast<float>(u) / nbSides * _2pi;
					uvs[u] = Math::Vec2( cos( rad ) * .5f + .5f, sin( rad ) * .5f + .5f );
					u++;
				}

				// Top cap
				uvs[u++] = Math::Vec2( 0.5f, 0.5f );
				while ( u <= nbSides * 2 + 1 )
				{
					float rad = static_cast<float>(u) / nbSides * _2pi;
					uvs[u] = Math::Vec2( cos( rad ) * .5f + .5f, sin( rad ) * .5f + .5f );
					u++;
				}

				// Sides
				int u_sides = 0;
				while ( u <= vertices.size() - 4 )
				{
					float t = static_cast<float>(u_sides) / nbSides;
					uvs[u] = Math::Vec2( t, 1.0f );
					uvs[u + 1] = Math::Vec2( t, 0.0f );
					u += 2;
					u_sides++;
				}
				uvs[u] = Math::Vec2( 1.0f, 1.0f );
				uvs[u + 1] = Math::Vec2( 1.0f, 0.0f );

				for ( int i = 0; i < vertices.size(); i++ )
				{
					Graphics::VertexData* fragmentVertex = new Graphics::VertexData();
					fragmentVertex->m_vertex = vertices[i];
					fragmentVertex->m_normal = normales[i];
					fragmentVertex->m_texture = uvs[i];
					vecFragmentVertexObject.push_back( fragmentVertex );
				}

				int nbTriangles = nbSides + nbSides + nbSides * 2;

				std::vector<Graphics::Fragment *> vecFragmentObject;
				vecFragmentObject.reserve( nbTriangles + 1 );

				Graphics::Fragment* frag;

				// Bottom cap
				int tri = 0;
				while ( tri < nbSides - 1 )
				{
					frag = new Graphics::Fragment();

					frag->m_idxFragmentVertex[0] = 0;
					frag->m_idxFragmentVertex[1] = tri + 1;
					frag->m_idxFragmentVertex[2] = tri + 2;
					vecFragmentObject.push_back( frag );
					tri++;
				}
				frag = new Graphics::Fragment();

				frag->m_idxFragmentVertex[0] = 0;
				frag->m_idxFragmentVertex[1] = tri + 1;
				frag->m_idxFragmentVertex[2] = 1;
				vecFragmentObject.push_back( frag );
				tri++;

				// Top cap
				//tri++;
				while ( tri < nbSides * 2 )
				{
					frag = new Graphics::Fragment();

					frag->m_idxFragmentVertex[0] = tri + 2;
					frag->m_idxFragmentVertex[1] = tri + 1;
					frag->m_idxFragmentVertex[2] = nbVerticesCap;
					vecFragmentObject.push_back( frag );

					tri++;
				}

				frag = new Graphics::Fragment();

				frag->m_idxFragmentVertex[0] = nbVerticesCap + 1;
				frag->m_idxFragmentVertex[1] = tri + 1;
				frag->m_idxFragmentVertex[2] = nbVerticesCap;
				vecFragmentObject.push_back( frag );

				tri++;
				tri++;

				// Sides
				while ( tri <= nbTriangles )
				{
					frag = new Graphics::Fragment();

					frag->m_idxFragmentVertex[0] = tri + 2;
					frag->m_idxFragmentVertex[1] = tri + 1;
					frag->m_idxFragmentVertex[2] = tri;
					vecFragmentObject.push_back( frag );

					tri++;

					frag = new Graphics::Fragment();

					frag->m_idxFragmentVertex[0] = tri + 1;
					frag->m_idxFragmentVertex[1] = tri + 2;
					frag->m_idxFragmentVertex[2] = tri;
					vecFragmentObject.push_back( frag );
					tri++;
				}

				ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Vec3 box;
				box.y = height;
				float radius = topRadius < bottomRadius ? bottomRadius : topRadius;
				box.x = radius * 2;
				box.z = box.x;

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );

				return mesh;
			}

			//TODO: Debug hole
			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenTorus( float radius1, float radius2, int nbRadSeg, int nbSides ) const
			{
				std::ostringstream stringStream;
				stringStream << "Torus " << radius1 << " " << radius2 << " " << nbRadSeg << " " << nbSides;
				std::string name = stringStream.str();

				std::vector<Math::Vec3> vertices;
				vertices.resize( ( nbRadSeg + 1 ) * ( nbSides + 1 ) );
				float _2pi = Math::Utils::PI * 2.0f;
				for ( int seg = 0; seg <= nbRadSeg; seg++ )
				{
					int currSeg = seg == nbRadSeg ? 0 : seg;

					float t1 = static_cast<float>(currSeg) / nbRadSeg * _2pi;
					Math::Vec3 r1 = Math::Vec3( cos( t1 ) * radius1, 0.0f, sin( t1 ) * radius1 );

					for ( int side = 0; side <= nbSides; side++ )
					{
						int currSide = side == nbSides ? 0 : side;

						float t2 = static_cast<float>(currSide) / nbSides * _2pi;
						Math::Vec3 r2 = Math::Quat::AngleAxis( -Math::Utils::ToDegree( t1 ), Math::Vec3::Up() ).Rotate( Math::Vec3( sin( t2 ) * radius2, cos( t2 ) * radius2, 0 ) );

						vertices[side + seg * ( nbSides + 1 )] = r1 + r2;
					}
				}

				std::vector<Math::Vec3> normales;
				normales.resize( vertices.size() );
				for ( int seg = 0; seg <= nbRadSeg; seg++ )
				{
					int currSeg = seg == nbRadSeg ? 0 : seg;

					float t1 = static_cast<float>(currSeg) / nbRadSeg * _2pi;
					Math::Vec3 r1 = Math::Vec3( cos( t1 ) * radius1, 0.0f, sin( t1 ) * radius1 );

					for ( int side = 0; side <= nbSides; side++ )
					{
						normales[side + seg * ( nbSides + 1 )] = ( vertices[side + seg * ( nbSides + 1 )] - r1 ).Normalized();
					}
				}

				std::vector<Math::Vec2> uvs;
				uvs.resize( vertices.size() );
				for ( int seg = 0; seg <= nbRadSeg; seg++ )
					for ( int side = 0; side <= nbSides; side++ )
						uvs[side + seg * ( nbSides + 1 )] = Math::Vec2( static_cast<float>(seg) / nbRadSeg, static_cast<float>(side) / nbSides );

				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( vertices.size() );

				for ( int i = 0; i < vertices.size(); i++ )
				{
					Graphics::VertexData* fragmentVertex = new Graphics::VertexData();
					fragmentVertex->m_vertex = vertices[i];
					fragmentVertex->m_normal = normales[i];
					fragmentVertex->m_texture = uvs[i];
					vecFragmentVertexObject.push_back( fragmentVertex );
				}

				size_t nbFaces = vertices.size();
				size_t nbTriangles = nbFaces * 2;

				std::vector<Graphics::Fragment *> vecFragmentObject;
				vecFragmentObject.reserve( nbTriangles );

				Graphics::Fragment* frag;

				for ( int seg = 0; seg <= nbRadSeg; seg++ )
				{
					for ( int side = 0; side <= nbSides; side++ )
					{
						int current = side + seg * ( nbSides + 1 );
						int next = side + ( seg < ( nbRadSeg ) ? ( seg + 1 ) * ( nbSides + 1 ) : 0 );

						if ( vecFragmentObject.size() < nbTriangles )
						{
							frag = new Graphics::Fragment();

							frag->m_idxFragmentVertex[0] = current;
							frag->m_idxFragmentVertex[1] = next;
							frag->m_idxFragmentVertex[2] = next + 1;

							vecFragmentObject.push_back( frag );

							frag = new Graphics::Fragment();

							frag->m_idxFragmentVertex[0] = current;
							frag->m_idxFragmentVertex[1] = next + 1;
							frag->m_idxFragmentVertex[2] = current + 1;
							vecFragmentObject.push_back( frag );
						}
					}
				}

				ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Vec3 box;
				box.x = radius1 * 2;
				box.z = box.x;
				box.y = radius1 - radius2;

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );
				mesh->SetBox( Math::Bounds::Box::FromVec3( box ) );

				return mesh;
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenCapsule( float height, float radius, int nbSides ) const
			{
				std::ostringstream stringStream;
				stringStream << "Capsule " << height << " " << radius << " " << nbSides;
				std::string name = stringStream.str();

				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( ( nbSides + 1 ) * ( nbSides / 2 + 2 ) );

				int i, j;
				float theta, phi;
				for ( j = 0; j <= nbSides / 4; j++ )
				{ // top cap
					for ( i = 0; i <= nbSides; i++ )
					{
						theta = i * ( Math::Utils::PI * 2 ) / nbSides;
						phi = -( Math::Utils::PI / 2 ) + Math::Utils::PI * j / ( nbSides / 2 );

						Graphics::VertexData* fragmentVertex = new Graphics::VertexData();

						fragmentVertex->m_vertex.x = radius * cosf( phi ) * cosf( theta );
						fragmentVertex->m_vertex.y = radius * sinf( phi );
						fragmentVertex->m_vertex.z = radius * cosf( phi ) * sinf( theta );
						fragmentVertex->m_normal.x = fragmentVertex->m_vertex.x;
						fragmentVertex->m_normal.y = fragmentVertex->m_vertex.y;
						fragmentVertex->m_normal.z = fragmentVertex->m_vertex.z;
						fragmentVertex->m_vertex.y -= height / 2;

						vecFragmentVertexObject.push_back( fragmentVertex );
					}
				}

				for ( j = nbSides / 4; j <= nbSides / 2; j++ )
				{ // bottom cap
					for ( i = 0; i <= nbSides; i++ )
					{
						theta = i * ( Math::Utils::PI * 2 ) / nbSides;
						phi = -( Math::Utils::PI / 2 ) + Math::Utils::PI * j / ( nbSides / 2 );

						Graphics::VertexData* fragmentVertex = new Graphics::VertexData();

						fragmentVertex->m_vertex.x = radius * cosf( phi ) * cosf( theta );
						fragmentVertex->m_vertex.y = radius * sinf( phi );
						fragmentVertex->m_vertex.z = radius * cosf( phi ) * sinf( theta );
						fragmentVertex->m_normal.x = fragmentVertex->m_vertex.x;
						fragmentVertex->m_normal.y = fragmentVertex->m_vertex.y;
						fragmentVertex->m_normal.z = fragmentVertex->m_vertex.z;
						fragmentVertex->m_vertex.y += height / 2;

						vecFragmentVertexObject.push_back( fragmentVertex );
					}
				}
				int size = ( nbSides + 1 ) * ( nbSides / 2 + 2 );
				// Calculate texture coordinates
				for ( i = 0; i < size; i++ )
				{
					Graphics::VertexData* fragmentVertex = vecFragmentVertexObject[i];
					fragmentVertex->m_texture.x = atan2( fragmentVertex->m_vertex.z, fragmentVertex->m_vertex.x ) / ( Math::Utils::PI * 2 );
					if ( fragmentVertex->m_texture.x < 0 )
						fragmentVertex->m_texture.x = 1 + fragmentVertex->m_texture.x;
					fragmentVertex->m_texture.y = 0.5f + atan2( fragmentVertex->m_vertex.y, sqrt( fragmentVertex->m_vertex.x * fragmentVertex->m_vertex.x + fragmentVertex->m_vertex.z * fragmentVertex->m_vertex.z ) ) / Math::Utils::PI;
				}

				std::vector<Graphics::Fragment *> vecFragmentObject;

				int i1, i2, i3, i4;
				Graphics::Fragment* frag;
				for ( j = 0; j <= nbSides / 2; j++ )
				{
					for ( i = 0; i <= nbSides; i++ )
					{
						i1 = j * ( nbSides + 1 ) + i + 1;
						i2 = j * ( nbSides + 1 ) + ( i + 1 ) + 1;
						i3 = ( j + 1 ) * ( nbSides + 1 ) + ( i + 1 ) + 1;
						i4 = ( j + 1 ) * ( nbSides + 1 ) + i + 1;

						frag = new Graphics::Fragment();
						frag->m_idxFragmentVertex[0] = i1;
						frag->m_idxFragmentVertex[1] = i2;
						frag->m_idxFragmentVertex[2] = i3;
						vecFragmentObject.push_back( frag );

						frag = new Graphics::Fragment();
						frag->m_idxFragmentVertex[0] = i1;
						frag->m_idxFragmentVertex[1] = i3;
						frag->m_idxFragmentVertex[2] = i4;
						vecFragmentObject.push_back( frag );
					}
				}

				//ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Vec3 box;
				box.x = radius * 2;
				box.z = box.x;
				box.y = height + box.x;

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );
				mesh->SetBox( Math::Bounds::Box::FromVec3( box ) );

				return mesh;
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenPlane( float length, float width, int resX, int resZ ) const
			{
				std::ostringstream stringStream;
				stringStream << "Plane " << length << " " << width << " " << resX << " " << resZ;
				std::string name = stringStream.str();

				std::vector<Math::Vec3> vertices;
				vertices.resize( resX * resZ );
				for ( int z = 0; z < resZ; z++ )
				{
					// [ -length / 2, length / 2 ]
					float zPos = ( static_cast<float>(z) / ( resZ - 1 ) - .5f ) * length;
					for ( int x = 0; x < resX; x++ )
					{
						// [ -width / 2, width / 2 ]
						float xPos = ( static_cast<float>(x) / ( resX - 1 ) - .5f ) * width;
						vertices[x + z * resX] = Math::Vec3( xPos, 0.0f, zPos );
					}
				}

				std::vector<Math::Vec3> normales;
				normales.resize( vertices.size() );
				for ( int n = 0; n < vertices.size(); n++ )
					normales[n] = Math::Vec3::Up();

				std::vector<Math::Vec2> uvs;
				uvs.resize( vertices.size() );
				for ( int v = 0; v < resZ; v++ )
				{
					for ( int u = 0; u < resX; u++ )
					{
						uvs[u + v * resX] = Math::Vec2( static_cast<float>(u) / ( resX - 1 ), static_cast<float>(v) / ( resZ - 1 ) );
					}
				}

				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( resX * resZ );

				for ( int i = 0; i < vertices.size(); i++ )
				{
					Graphics::VertexData* fragmentVertex = new Graphics::VertexData();
					fragmentVertex->m_vertex = vertices[i];
					fragmentVertex->m_normal = normales[i];
					fragmentVertex->m_texture = uvs[i];
					vecFragmentVertexObject.push_back( fragmentVertex );
				}

				int nbFaces = ( resX - 1 ) * ( resZ - 1 );
				std::vector<Graphics::Fragment *> vecFragmentObject;
				vecFragmentObject.reserve( nbFaces * 2 );

				for ( int face = 0; face < nbFaces; face++ )
				{
					// Retrieve lower left corner from face ind
					int i = face % ( resX - 1 ) + ( face / ( resZ - 1 ) * resX );

					Graphics::Fragment* frag = new Graphics::Fragment();
					frag->m_idxFragmentVertex[0] = i + resX;
					frag->m_idxFragmentVertex[1] = i + 1;
					frag->m_idxFragmentVertex[2] = i;

					vecFragmentObject.push_back( frag );

					frag = new Graphics::Fragment();
					frag->m_idxFragmentVertex[0] = i + resX;
					frag->m_idxFragmentVertex[1] = i + resX + 1;
					frag->m_idxFragmentVertex[2] = i + 1;

					vecFragmentObject.push_back( frag );
				}

				ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Vec3 box;
				box.x = width;
				box.z = length;
				box.y = 0.1f;

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );
				mesh->SetBox( Math::Bounds::Box::FromVec3( box ) );
				return mesh;
			}

			Graphics::AbstractMesh* TCMPrimitiveLoader::CoreGenBox( float width, float height, float length ) const
			{
				std::ostringstream stringStream;
				stringStream << "Box " << length << " " << width << " " << height;
				std::string name = stringStream.str();

				Math::Vec3 p0 = Math::Vec3( -length * .5f, -width * .5f, height * .5f );
				Math::Vec3 p1 = Math::Vec3( length * .5f, -width * .5f, height * .5f );
				Math::Vec3 p2 = Math::Vec3( length * .5f, -width * .5f, -height * .5f );
				Math::Vec3 p3 = Math::Vec3( -length * .5f, -width * .5f, -height * .5f );

				Math::Vec3 p4 = Math::Vec3( -length * .5f, width * .5f, height * .5f );
				Math::Vec3 p5 = Math::Vec3( length * .5f, width * .5f, height * .5f );
				Math::Vec3 p6 = Math::Vec3( length * .5f, width * .5f, -height * .5f );
				Math::Vec3 p7 = Math::Vec3( -length * .5f, width * .5f, -height * .5f );

				Math::Vec3 vertices[] =
				{
					// Bottom
					p0, p1, p2, p3,

					// Left
					p7, p4, p0, p3,

					// Front
					p4, p5, p1, p0,

					// Back
					p6, p7, p3, p2,

					// Right
					p5, p6, p2, p1,

					// Top
					p7, p6, p5, p4
				};

				Math::Vec3 up = Math::Vec3::Up();
				Math::Vec3 down = up * -1;
				Math::Vec3 front = Math::Vec3::Forward();
				Math::Vec3 back = front * -1;
				Math::Vec3 right = Math::Vec3::Right();
				Math::Vec3 left = right * -1;

				Math::Vec3 normales[] =
				{
					// Bottom
					down, down, down, down,

					// Left
					left, left, left, left,

					// Front
					front, front, front, front,

					// Back
					back, back, back, back,

					// Right
					right, right, right, right,

					// Top
					up, up, up, up
				};

				Math::Vec2 _00 = Math::Vec2( 0.0f, 0.0f );
				Math::Vec2 _10 = Math::Vec2( 1.0f, 0.0f );
				Math::Vec2 _01 = Math::Vec2( 0.0f, 1.0f );
				Math::Vec2 _11 = Math::Vec2( 1.0f, 1.0f );

				Math::Vec2 uvs[] =
				{
					// Bottom
					_11, _01, _00, _10,

					// Left
					_11, _01, _00, _10,

					// Front
					_11, _01, _00, _10,

					// Back
					_11, _01, _00, _10,

					// Right
					_11, _01, _00, _10,

					// Top
					_11, _01, _00, _10,
				};

				int count = _countof( vertices );
				std::vector<Graphics::VertexData *> vecFragmentVertexObject;
				vecFragmentVertexObject.reserve( count );

				for ( int i = 0; i < count; i++ )
				{
					Graphics::VertexData* fragmentVertex = new Graphics::VertexData();
					fragmentVertex->m_vertex = vertices[i];
					fragmentVertex->m_normal = normales[i];
					fragmentVertex->m_texture = uvs[i];
					vecFragmentVertexObject.push_back( fragmentVertex );
				}

				std::vector<Graphics::Fragment *> vecFragmentObject;

				vecFragmentObject.push_back( new Graphics::Fragment( { 3, 1, 0 } ) );
				vecFragmentObject.push_back( new Graphics::Fragment( { 3, 2, 1 } ) );

				// Left
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 1, 1 + 4 * 1, 0 + 4 * 1 } ) );
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 1, 2 + 4 * 1, 1 + 4 * 1 } ) );

				// Front
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 2, 1 + 4 * 2, 0 + 4 * 2 } ) );
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 2, 2 + 4 * 2, 1 + 4 * 2 } ) );

				// Back
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 3, 1 + 4 * 3, 0 + 4 * 3 } ) );
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 3, 2 + 4 * 3, 1 + 4 * 3 } ) );

				// Right
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 4, 1 + 4 * 4, 0 + 4 * 4 } ) );
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 4, 2 + 4 * 4, 1 + 4 * 4 } ) );

				// Top
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 5, 1 + 4 * 5, 0 + 4 * 5 } ) );
				vecFragmentObject.push_back( new Graphics::Fragment( { 3 + 4 * 5, 2 + 4 * 5, 1 + 4 * 5 } ) );

				ComputeTangents( vecFragmentVertexObject, vecFragmentObject );

				Math::Vec3 box;
				box.x = width;
				box.z = length;
				box.y = height;

				Graphics::AbstractMesh* mesh = Graphics::RendererFactory::CreateMesh( vecFragmentVertexObject, vecFragmentObject );
				mesh->SetKey( name );
				mesh->SetBox( Math::Bounds::Box::FromVec3( box ) );
				return mesh;
			}

			void TCMPrimitiveLoader::ComputeTangents( const std::vector<Graphics::VertexData*>& vertexDataList, const std::vector<Graphics::Fragment*>& fragmentList ) const
			{
				auto vertexDataArray = vertexDataList.data();
				auto fragmentArray = fragmentList.data();

				size_t vertexDataCount = vertexDataList.size();
				size_t fragmentDataCount = fragmentList.size();

				for ( int i = 0; i < fragmentDataCount; ++i )
				{
					Graphics::Fragment* objectFragment = fragmentArray[i];
					// Shortcuts for vertices
					Math::Vec3& v0 = vertexDataArray[objectFragment->m_idxFragmentVertex[0]]->m_vertex;
					Math::Vec3& v1 = vertexDataArray[objectFragment->m_idxFragmentVertex[1]]->m_vertex;
					Math::Vec3& v2 = vertexDataArray[objectFragment->m_idxFragmentVertex[2]]->m_vertex;

					// Shortcuts for UVs
					Math::Vec2& uv0 = vertexDataArray[objectFragment->m_idxFragmentVertex[0]]->m_texture;
					Math::Vec2& uv1 = vertexDataArray[objectFragment->m_idxFragmentVertex[1]]->m_texture;
					Math::Vec2& uv2 = vertexDataArray[objectFragment->m_idxFragmentVertex[2]]->m_texture;

					// Edges of the triangle : postion delta
					Math::Vec3 deltaPos1 = v1 - v0;
					Math::Vec3 deltaPos2 = v2 - v0;

					// UV delta
					Math::Vec2 deltaUV1 = uv1 - uv0;
					Math::Vec2 deltaUV2 = uv2 - uv0;

					float r = 1.0f / ( deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x );
					Math::Vec3 tangent = ( deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y ) * r;

					vertexDataArray[objectFragment->m_idxFragmentVertex[0]]->m_tangent += tangent;
					vertexDataArray[objectFragment->m_idxFragmentVertex[1]]->m_tangent += tangent;
					vertexDataArray[objectFragment->m_idxFragmentVertex[2]]->m_tangent += tangent;
				}

				for ( int i = 0; i < vertexDataCount; ++i )
				{
					vertexDataArray[i]->m_tangent.Normalize();
				}
			}
		}
	}
}

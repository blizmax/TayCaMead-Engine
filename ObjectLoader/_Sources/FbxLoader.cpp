#include "FbxLoader.h"

#include "TCMLogger/Logger.h"
#include "MathLib/Vec2.h"
#include "MathLib/Vec3.h"

#include <memory>
#include <mutex>
#include "TCMRenderer/Rendering/RendererFactory.h"
#include "TCMRenderer/RHI/Generic/RenderData.h"
#include "ObjectLoader/MatLoader.h"

namespace TCM
{
	namespace Engine
	{
		namespace Resources
		{
			FbxLoader& FbxLoader::GetInstance()
			{
				static std::unique_ptr<FbxLoader> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new FbxLoader() );
				                }
				);
				return *( m_instance.get() );
			}

			void FbxLoader::Shutdown() const
			{
				TCMDEBUG("");
				m_Scene->Destroy();
				m_FbxManager->Destroy();
			}

			void FbxLoader::Initialize()
			{
				TCMDEBUG("");
				// 1. Creation Singleton
				m_FbxManager = FbxManager::Create();
				// 2. Initialisation des I/O
				FbxIOSettings* ioSettings =
						FbxIOSettings::Create( m_FbxManager, IOSROOT );
				m_FbxManager->SetIOSettings( ioSettings );
			}

			FbxLoader::FbxLoader()
			{
				Initialize();
			}

			FbxLoader::~FbxLoader()
			{
				Shutdown();
			}

			Graphics::Skeleton* FbxLoader::LoadFromFile( const std::string& name )
			{
				return GetInstance().CoreLoadFromFile( name );
			}

			FbxString FbxLoader::GetCurrentDocumentPath()
			{
				return GetInstance().m_Scene->GetPathToRootDocument();
			}

			Graphics::Skeleton* FbxLoader::CoreLoadFromFile( const std::string& name )
			{
				TCMINFO("Parse file " + name);

				// 1. Creation de la scene (stockage)
				m_Scene = FbxScene::Create( m_FbxManager, name.c_str() );

				// 2. Importation 
				FbxImporter* importer = FbxImporter::Create(
					m_FbxManager, "" );
				// 3. lecture du fichier fbx
				bool status = importer->Initialize( name.c_str(), -1,
				                                    m_FbxManager->GetIOSettings() );
				if ( !status )
				{
					TCMFAILURE("Can't load file : " + std::string(name));
					importer->Destroy();
					return nullptr;
				}
				// 4. copie du contenu dans notre scene
				status = importer->Import( m_Scene );
				if ( !status )
				{
					TCMFAILURE("Can't import scene from file : " + std::string(name));
					importer->Destroy();
					return nullptr;
				}
				// 5. l'importer n'est plus utile a ce stade
				importer->Destroy();

				FbxNode* root = m_Scene->GetRootNode();
				// Modifie les parametres geometriques du Root Node
				// 5 - changement de repere
				FbxAxisSystem SceneAxisSystem = m_Scene->GetGlobalSettings().GetAxisSystem();
				FbxAxisSystem OurAxisSystem( FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd,
				                             FbxAxisSystem::eRightHanded );
				if ( SceneAxisSystem != OurAxisSystem )
				{
					OurAxisSystem.ConvertScene( m_Scene );
				}
				// 6 - Changement d'unites (vers cm) : si (ScaleFactor - 1cm) > 0 -> pas des cm
				FbxSystemUnit SceneSystemUnit = m_Scene->GetGlobalSettings().GetSystemUnit();
				if ( fabs( SceneSystemUnit.GetScaleFactor() - 1.0 ) > 0.00001 )
				{
					FbxSystemUnit OurSystemUnit( 1.0 );
					OurSystemUnit.ConvertScene( m_Scene );
				}
				Graphics::Skeleton* result = new Graphics::Skeleton();

				int childCount = root->GetChildCount();
				for ( int i = 0; i < childCount; i++ )
				{
					const FbxNode* child = root->GetChild( i );
					ProcessNode( child, root, result );
				}
				root->Destroy();

				return result;
			}

			void FbxLoader::ProcessNode( const FbxNode* node,
			                             const FbxNode* parentNode, Graphics::s_Skeleton* skeleton )
			{
				const FbxNodeAttribute* att = node->GetNodeAttribute();
				if ( att )
				{
					FbxNodeAttribute::EType type;
					type = att->GetAttributeType();
					switch ( type )
					{
						case FbxNodeAttribute::eMesh:
							LoadMesh( node, skeleton );
							break;
						case FbxNodeAttribute::eSkeleton:
							break;
						case FbxNodeAttribute::eCamera:
							break;
						case FbxNodeAttribute::eLight:
							break;
						default: break;
					}
				}

				int childCount = node->GetChildCount();
				for ( int i = 0; i < childCount; i++ )
				{
					const FbxNode* child = node->GetChild( i );
					ProcessNode( child, node, skeleton );
				}
			}

			void FbxLoader::LoadMesh( const FbxNode* node, Graphics::s_Skeleton* skeleton )
			{
				TCMINFO("Read Mesh");

				FbxNode* mutable_node = const_cast<FbxNode*>(node);
				FbxMesh* pMesh = mutable_node->GetMesh();

				// Transform placant l'objet dans la scene.
				// Passer par l'Animation Evaluator (global) accumule les transform des parents
				FbxAMatrix globalTransform =
						node->GetScene()->GetAnimationEvaluator()->GetNodeGlobalTransform( mutable_node );

				// Matrice geometrique locale.
				FbxVector4 translation = node->GetGeometricTranslation( FbxNode::eSourcePivot );
				FbxVector4 rotation = node->GetGeometricRotation( FbxNode::eSourcePivot );
				FbxVector4 scale = node->GetGeometricScaling( FbxNode::eSourcePivot );

				FbxAMatrix geometryTransform;
				geometryTransform.SetTRS( translation, rotation, scale );

				FbxAMatrix finalGlobalTransform = globalTransform * geometryTransform;

				const int lPolygonCount = pMesh->GetPolygonCount();
				FbxArray<Graphics::SubMesh*> subMeshes;
				// Count the polygon count of each material
				FbxLayerElementArrayTemplate<int>* lMaterialIndice = nullptr;
				FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
				if ( pMesh->GetElementMaterial() )
				{
					lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
					lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
					if ( lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon )
					{
						FBX_ASSERT( lMaterialIndice->GetCount() == lPolygonCount );
						if ( lMaterialIndice->GetCount() == lPolygonCount )
						{
							// Count the faces of each material
							for ( int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex )
							{
								const int lMaterialIndex = lMaterialIndice->GetAt( lPolygonIndex );
								if ( subMeshes.GetCount() < lMaterialIndex + 1 )
								{
									subMeshes.Resize( lMaterialIndex + 1 );
								}
								if ( subMeshes[lMaterialIndex] == nullptr )
								{
									subMeshes[lMaterialIndex] = new Graphics::SubMesh;
								}
								uint32_t count = subMeshes[lMaterialIndex]->GetCount();
								count += 3;
								subMeshes[lMaterialIndex]->SetCount( count );
							}

							// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
							// if, in the loop above, we resized the mSubMeshes by more than one slot.
							for ( int i = 0; i < subMeshes.GetCount(); i++ )
							{
								if ( subMeshes[i] == nullptr )
									subMeshes[i] = new Graphics::SubMesh;
							}

							// Record the offset (how many vertex)
							const int lMaterialCount = subMeshes.GetCount();
							int lOffset = 0;
							for ( int lIndex = 0; lIndex < lMaterialCount; ++lIndex )
							{
								subMeshes[lIndex]->SetOffset( lOffset );
								lOffset += subMeshes[lIndex]->GetCount();
								// This will be used as counter in the following procedures, reset to zero
								subMeshes[lIndex]->SetCount( 0 );
							}
							FBX_ASSERT( lOffset == lPolygonCount * 3);
						}
					}
				}

				// All faces will use the same material.
				if ( subMeshes.GetCount() == 0 )
				{
					subMeshes.Resize( 1 );
					subMeshes[0] = new Graphics::SubMesh();
				}

				// Bake material and hook as user data.
				const int lMaterialCount = node->GetMaterialCount();
				for ( int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex )
				{
					FbxSurfaceMaterial* lMaterial = node->GetMaterial( lMaterialIndex );
					if ( lMaterial )
					{
						subMeshes[lMaterialIndex]->SetMaterial( CreateMaterial( lMaterial ) );
					}
				}

				bool mAllByControlPoint = true;

				// Congregate all the data of a mesh to be cached in VBOs.
				// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
				bool mHasNormal = pMesh->GetElementNormalCount() > 0;
				bool mHasUV = pMesh->GetElementUVCount() > 0;
				bool mHasTangent = pMesh->GetElementTangentCount() > 0;
				FbxGeometryElement::EMappingMode lNormalMappingMode;
				FbxGeometryElement::EMappingMode lUVMappingMode;
				FbxGeometryElement::EMappingMode lTanMappingMode;
				if ( mHasNormal )
				{
					lNormalMappingMode = pMesh->GetElementNormal( 0 )->GetMappingMode();
					if ( lNormalMappingMode == FbxGeometryElement::eNone )
					{
						mHasNormal = false;
					}
					if ( mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint )
					{
						mAllByControlPoint = false;
					}
				}
				if ( mHasUV )
				{
					lUVMappingMode = pMesh->GetElementUV( 0 )->GetMappingMode();
					if ( lUVMappingMode == FbxGeometryElement::eNone )
					{
						mHasUV = false;
					}
					if ( mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint )
					{
						mAllByControlPoint = false;
					}
				}
				if ( mHasTangent )
				{
					lTanMappingMode = pMesh->GetElementTangent( 0 )->GetMappingMode();
					if ( lTanMappingMode == FbxGeometryElement::eNone )
					{
						mHasTangent = false;
					}
					if ( mHasTangent && lTanMappingMode != FbxGeometryElement::eByControlPoint )
					{
						mAllByControlPoint = false;
					}
				}
				/* determine la presence de tangentes
				FbxLayerElementTangent* meshTangents = pMesh->GetElementTangent( 0 );
				if ( meshTangents == nullptr )
				{
					// sinon on genere des tangentes (pour le tangent space normal mapping)
					bool test = pMesh->GenerateTangentsDataForAllUVSets( true );
					meshTangents = pMesh->GetElementTangent( 0 );
				}

				FbxLayerElement::EReferenceMode tangentRefMode = meshTangents->GetReferenceMode();*/

				// Allocate the array memory, by control point or by polygon vertex.
				int lPolygonVertexCount = pMesh->GetControlPointsCount();
				std::vector<Graphics::s_Fragment *> vecFragment;
				vecFragment.resize( lPolygonCount );

				for ( int i = 0; i < vecFragment.size(); ++i )
				{
					vecFragment[i] = new Graphics::s_Fragment;
				}

				std::vector<Graphics::VertexData *> vertexDataList;

				if ( !mAllByControlPoint )
				{
					lPolygonVertexCount = lPolygonCount * 3;
				}
				vertexDataList.resize( lPolygonVertexCount );

				FbxStringList lUVNames;
				pMesh->GetUVSetNames( lUVNames );
				const char* lUVName = nullptr;
				if ( mHasUV && lUVNames.GetCount() )
				{
					lUVName = lUVNames[0];
				}
				const FbxGeometryElementTangent* lTanElement = nullptr;
				if ( mHasTangent )
				{
					lTanElement = pMesh->GetElementTangent( 0 );
				}

				Math::Vec3 vLowerLeftFront = Math::Vec3( 999999.0f, 999999.0f, -999999.0f );
				Math::Vec3 vUpperRightBack = Math::Vec3( -999999.0f, -999999.0f, 999999.0f );

				// Populate the array with vertex attribute, if by control point.
				const FbxVector4* lControlPoints = pMesh->GetControlPoints();
				FbxVector4 lCurrentVertex;
				FbxVector4 lCurrentNormal;
				FbxVector2 lCurrentUV;
				FbxVector4 lCurrentTangent;
				if ( mAllByControlPoint )
				{
					const FbxGeometryElementNormal* lNormalElement = nullptr;
					const FbxGeometryElementUV* lUVElement = nullptr;

					if ( mHasNormal )
					{
						lNormalElement = pMesh->GetElementNormal( 0 );
					}
					if ( mHasUV )
					{
						lUVElement = pMesh->GetElementUV( 0 );
					}

					for ( int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex )
					{
						// Save the vertex position.
						lCurrentVertex = lControlPoints[lIndex];
						lCurrentVertex = finalGlobalTransform.MultT( lCurrentVertex );

						if ( vertexDataList[lIndex] == nullptr )
							vertexDataList[lIndex] = new Graphics::s_VertexData;

						Graphics::s_VertexData* vertexData = vertexDataList[lIndex];

						Math::Vec3 vertex;
						vertex.x = static_cast<float>(lCurrentVertex.mData[0]);
						vertex.y = static_cast<float>(lCurrentVertex.mData[1]);
						vertex.z = static_cast<float>(lCurrentVertex.mData[2]);

						vertexData->m_vertex = vertex;

						vLowerLeftFront.x = ( vLowerLeftFront.x < vertex.x ) ?
							                    vLowerLeftFront.x : vertex.x;
						vLowerLeftFront.y = ( vLowerLeftFront.y < vertex.y ) ?
							                    vLowerLeftFront.y : vertex.y;
						vLowerLeftFront.z = ( vLowerLeftFront.z > vertex.z ) ?
							                    vLowerLeftFront.z : vertex.z;

						// Update upper-right-back corner of BB
						vUpperRightBack.x = ( vUpperRightBack.x > vertex.x ) ?
							                    vUpperRightBack.x : vertex.x;
						vUpperRightBack.y = ( vUpperRightBack.y > vertex.y ) ?
							                    vUpperRightBack.y : vertex.y;
						vUpperRightBack.z = ( vUpperRightBack.z < vertex.z ) ?
							                    vUpperRightBack.z : vertex.z;

						// Save the normal.
						if ( mHasNormal )
						{
							int lNormalIndex = lIndex;
							if ( lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect )
							{
								lNormalIndex = lNormalElement->GetIndexArray().GetAt( lIndex );
							}
							lCurrentNormal = lNormalElement->GetDirectArray().GetAt( lNormalIndex );
							lCurrentNormal = finalGlobalTransform.MultT( lCurrentNormal );
							Math::Vec3 normal;

							normal.x = static_cast<float>(lCurrentNormal.mData[0]);
							normal.y = static_cast<float>(lCurrentNormal.mData[1]);
							normal.z = static_cast<float>(lCurrentNormal.mData[2]);

							vertexData->m_normal = normal;
						}

						// Save the UV.
						if ( mHasUV )
						{
							int lUVIndex = lIndex;
							if ( lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect )
							{
								lUVIndex = lUVElement->GetIndexArray().GetAt( lIndex );
							}
							lCurrentUV = lUVElement->GetDirectArray().GetAt( lUVIndex );

							Math::Vec2 texture;
							texture.x = static_cast<float>(lCurrentUV.mData[0]);
							texture.y = static_cast<float>(lCurrentUV.mData[1]);

							vertexData->m_texture = texture;
						}

						if ( mHasTangent )
						{
							int lTanIndex = lIndex;
							if ( lTanElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect )
							{
								lTanIndex = lTanElement->GetIndexArray().GetAt( lIndex );
							}
							lCurrentTangent = lTanElement->GetDirectArray().GetAt( lTanIndex );

							Math::Vec3 tangent;
							tangent.x = static_cast<float>(lCurrentTangent.mData[0]);
							tangent.y = static_cast<float>(lCurrentTangent.mData[1]);
							tangent.z = static_cast<float>(lCurrentTangent.mData[2]);

							vertexData->m_tangent = tangent;
						}
					}
				}

				int lVertexCount = 0;
				for ( int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex )
				{
					// The material for current face.
					int lMaterialIndex = 0;
					if ( lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon )
					{
						lMaterialIndex = lMaterialIndice->GetAt( lPolygonIndex );
					}

					// Where should I save the vertex attribute index, according to the material
					const int lIndexOffset = ( subMeshes[lMaterialIndex]->GetOffset() +
						subMeshes[lMaterialIndex]->GetCount() ) / 3;

					Graphics::s_Fragment* fragment = vecFragment[lIndexOffset];

					for ( int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex )
					{
						const int lControlPointIndex = pMesh->GetPolygonVertex( lPolygonIndex, lVerticeIndex );

						if ( vertexDataList[lVertexCount] == nullptr )
							vertexDataList[lVertexCount] = new Graphics::s_VertexData;

						Graphics::s_VertexData* vertexData = vertexDataList[lVertexCount];

						if ( mAllByControlPoint )
						{
							fragment->m_idxFragmentVertex[lVerticeIndex] = static_cast<uint16_t>(lControlPointIndex);
						}
						// Populate the array with vertex attribute, if by polygon vertex.
						else
						{
							fragment->m_idxFragmentVertex[lVerticeIndex] = static_cast<uint16_t>(lVertexCount);

							lCurrentVertex = lControlPoints[lControlPointIndex];
							lCurrentVertex = finalGlobalTransform.MultT( lCurrentVertex );
							Math::Vec3 vertex;
							vertex.x = static_cast<float>(lCurrentVertex.mData[0]);
							vertex.y = static_cast<float>(lCurrentVertex.mData[1]);
							vertex.z = static_cast<float>(lCurrentVertex.mData[2]);

							vertexData->m_vertex = vertex;

							vLowerLeftFront.x = ( vLowerLeftFront.x < vertex.x ) ?
								                    vLowerLeftFront.x : vertex.x;
							vLowerLeftFront.y = ( vLowerLeftFront.y < vertex.y ) ?
								                    vLowerLeftFront.y : vertex.y;
							vLowerLeftFront.z = ( vLowerLeftFront.z > vertex.z ) ?
								                    vLowerLeftFront.z : vertex.z;

							// Update upper-right-back corner of BB
							vUpperRightBack.x = ( vUpperRightBack.x > vertex.x ) ?
								                    vUpperRightBack.x : vertex.x;
							vUpperRightBack.y = ( vUpperRightBack.y > vertex.y ) ?
								                    vUpperRightBack.y : vertex.y;
							vUpperRightBack.z = ( vUpperRightBack.z < vertex.z ) ?
								                    vUpperRightBack.z : vertex.z;

							if ( mHasNormal )
							{
								pMesh->GetPolygonVertexNormal( lPolygonIndex, lVerticeIndex, lCurrentNormal );
								Math::Vec3 normal;
								lCurrentNormal = finalGlobalTransform.MultT( lCurrentNormal );
								normal.x = static_cast<float>(lCurrentNormal.mData[0]);
								normal.y = static_cast<float>(lCurrentNormal.mData[1]);
								normal.z = static_cast<float>(lCurrentNormal.mData[2]);

								vertexData->m_normal = normal;
							}

							if ( mHasUV )
							{
								bool lUnmappedUV;
								pMesh->GetPolygonVertexUV( lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV );

								Math::Vec2 texture;
								texture.x = static_cast<float>(lCurrentUV.mData[0]);
								texture.y = static_cast<float>(lCurrentUV.mData[1]);

								vertexData->m_texture = texture;
							}

							if ( mHasTangent )
							{
								int lTanIndex = lControlPointIndex;
								if ( lTanElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect )
								{
									lTanIndex = lTanElement->GetIndexArray().GetAt( lControlPointIndex );
								}
								lCurrentTangent = lTanElement->GetDirectArray().GetAt( lTanIndex );

								Math::Vec3 tangent;
								tangent.x = static_cast<float>(lCurrentTangent.mData[0]);
								tangent.y = static_cast<float>(lCurrentTangent.mData[1]);
								tangent.z = static_cast<float>(lCurrentTangent.mData[2]);

								vertexData->m_tangent = tangent;
							}
						}
						++lVertexCount;
					}
					uint32_t count = subMeshes[lMaterialIndex]->GetCount();
					count += 3;
					subMeshes[lMaterialIndex]->SetCount( count );
				}

				for ( int i = 0; i < subMeshes.GetCount(); ++i )
				{
					uint32_t indiceOffset = subMeshes[i]->GetOffset();
					uint32_t indiceCount = subMeshes[i]->GetCount();

					Math::Vec3 subLowerLeftFront = Math::Vec3( 999999.0f, 999999.0f, -999999.0f );
					Math::Vec3 subUpperRightBack = Math::Vec3( -999999.0f, -999999.0f, 999999.0f );

					for ( uint32_t index = 0; index < indiceCount; ++index )
					{
						Graphics::Fragment* frag = vecFragment[( index + indiceOffset ) / 3];
						Graphics::VertexData* vertex = vertexDataList[frag->m_idxFragmentVertex[( index + indiceOffset ) % 3]];

						Math::Vec3 v = vertex->m_vertex;
						subLowerLeftFront.x = ( subLowerLeftFront.x < v.x ) ?
							                      subLowerLeftFront.x : v.x;
						subLowerLeftFront.y = ( subLowerLeftFront.y < v.y ) ?
							                      subLowerLeftFront.y : v.y;
						subLowerLeftFront.z = ( subLowerLeftFront.z > v.z ) ?
							                      subLowerLeftFront.z : v.z;

						// Update upper-right-back corner of BB
						subUpperRightBack.x = ( subUpperRightBack.x > v.x ) ?
							                      subUpperRightBack.x : v.x;
						subUpperRightBack.y = ( subUpperRightBack.y > v.y ) ?
							                      subUpperRightBack.y : v.y;
						subUpperRightBack.z = ( subUpperRightBack.z < v.z ) ?
							                      subUpperRightBack.z : v.z;
					}

					Math::Vec3 boxSize;
					Math::Vec3 offset = ( subUpperRightBack + subLowerLeftFront ) / 2;
					boxSize.z = abs( subUpperRightBack.z - subLowerLeftFront.z );
					boxSize.x = abs( subUpperRightBack.x - subLowerLeftFront.x );
					boxSize.y = abs( subUpperRightBack.y - subLowerLeftFront.y );
					Math::Bounds::Box box = Math::Bounds::Box::FromVec3( boxSize );
					box.position = offset;

					subMeshes[i]->SetBox( box );

					skeleton->AddSubMesh( subMeshes[i] );
				}

				Graphics::AbstractMesh* abstractMesh = Graphics::RendererFactory::CreateMesh( vertexDataList, vecFragment );

				Math::Vec3 boxSize;
				Math::Vec3 offset = ( vUpperRightBack + vLowerLeftFront ) / 2;
				boxSize.z = abs( vUpperRightBack.z - vLowerLeftFront.z );
				boxSize.x = abs( vUpperRightBack.x - vLowerLeftFront.x );
				boxSize.y = abs( vUpperRightBack.y - vLowerLeftFront.y );
				Math::Bounds::Box box = Math::Bounds::Box::FromVec3( boxSize );
				box.position = offset;

				abstractMesh->SetBox( box );

				skeleton->SetMesh( abstractMesh );

				TCMINFO("End Read Mesh");
			}

			FbxDouble3 GetMaterialProperty( const FbxSurfaceMaterial* pMaterial,
			                                const char* pPropertyName,
			                                const char* pFactorPropertyName,
			                                std::string& pTextureName, bool& valid )
			{
				valid = false;
				FbxDouble3 lResult( 0, 0, 0 );
				const FbxProperty lProperty = pMaterial->FindProperty( pPropertyName );
				const FbxProperty lFactorProperty = pMaterial->FindProperty( pFactorPropertyName );
				if ( lProperty.IsValid() && lFactorProperty.IsValid() )
				{
					valid = true;
					lResult = lProperty.Get<FbxDouble3>();
					double lFactor = lFactorProperty.Get<FbxDouble>();
					if ( lFactor != 1 && lFactor > 0 )
					{
						lResult[0] *= lFactor;
						lResult[1] *= lFactor;
						lResult[2] *= lFactor;
					}
				}

				if ( lProperty.IsValid() )
				{
					const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
					if ( lTextureCount )
					{
						const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>( 0 );
						if ( lTexture )
						{
							pTextureName = lTexture->GetFileName();
						}
					}
				}

				return lResult;
			}

			Graphics::AbstractMaterial* FbxLoader::CreateMaterial( const FbxSurfaceMaterial* pMaterial ) const
			{
				Graphics::AbstractMaterial* result = Graphics::RendererFactory::CreateMaterial();
				result->SetName( pMaterial->GetName() );

				bool isValid;
				std::string filepath;
				const FbxDouble3 lEmissive = GetMaterialProperty( pMaterial,
				                                                  FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, filepath,
				                                                  isValid );
				//result->m_maps.emissiveMap = LoadTexFromFile(filepath);

				if ( isValid )
				{
					Math::ColorRGB emissive;
					emissive.r = static_cast<float>(lEmissive[0]);
					emissive.g = static_cast<float>(lEmissive[1]);
					emissive.b = static_cast<float>(lEmissive[2]);

					result->SetEmissiveColor( emissive );
				}

				const FbxDouble3 lAmbient = GetMaterialProperty( pMaterial,
				                                                 FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, filepath,
				                                                 isValid );

				result->SetAmbientMap( MatLoader::LoadTexFromFile( filepath ) );

				if ( isValid )
				{
					Math::ColorRGB ambient;

					ambient.r = static_cast<float>(lAmbient[0]);
					ambient.g = static_cast<float>(lAmbient[1]);
					ambient.b = static_cast<float>(lAmbient[2]);

					result->SetAmbientColor( ambient );
				}

				const FbxDouble3 lDiffuse = GetMaterialProperty( pMaterial,
				                                                 FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, filepath,
				                                                 isValid );
				result->SetDiffuseMap( MatLoader::LoadTexFromFile( filepath ) );

				if ( isValid )
				{
					Math::ColorRGB diffuse;

					diffuse.r = static_cast<float>(lDiffuse[0]);
					diffuse.g = static_cast<float>(lDiffuse[1]);
					diffuse.b = static_cast<float>(lDiffuse[2]);

					result->SetDiffuseColor( diffuse );
				}
				const FbxDouble3 lSpecular = GetMaterialProperty( pMaterial,
				                                                  FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, filepath,
				                                                  isValid );
				result->SetSpecularColorMap( MatLoader::LoadTexFromFile( filepath ) );

				if ( isValid )
				{
					Math::ColorRGB specular;

					specular.r = static_cast<float>(lSpecular[0]);
					specular.g = static_cast<float>(lSpecular[1]);
					specular.b = static_cast<float>(lSpecular[2]);

					result->SetSpecularColor( specular );
				}

				const FbxProperty propertyBump = pMaterial->FindProperty( FbxSurfaceMaterial::sBump );
				if ( propertyBump.IsValid() )
				{
					int textureCount = propertyBump.GetSrcObjectCount<FbxFileTexture>();
					if ( textureCount > 0 )
					{
						result->SetBumpMap( MatLoader::LoadTexFromFile( propertyBump.GetSrcObject<FbxFileTexture>( 0 )->GetFileName() ) );
					}
				}

				FbxProperty lShininessProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sShininess );
				if ( lShininessProperty.IsValid() )
				{
					double lShininess = lShininessProperty.Get<FbxDouble>();
					result->SetShininess( static_cast<float>(lShininess) );
				}
				return result;
			}
		}
	}
}

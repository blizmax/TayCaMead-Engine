#pragma once

// Necessaire si linkage dynamique (dll)
#define FBXSDK_SHARED
#include <fbxsdk.h>
#include "TCMRenderer/RHI/Generic/RenderData.h"

#ifdef FBXSDK_SHARED
	#pragma comment(lib, "libfbxsdk.lib")
#else
	#error "FBXSDK cannot be loaded"
#endif

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_Skeleton;
			struct s_SubMesh;
		}

		namespace Resources
		{
			/**
			 * \brief Class used for loading .fbx
			 */
			class FbxLoader final
			{
			public:
				~FbxLoader();

				/**
				 * \brief Loads objects from an .fbx file
				 * \param filename _IN_ Path of the file to be parsed
				 * \return Scene object of every objects loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				static Graphics::s_Skeleton* LoadFromFile( const std::string& filename );

				static FbxString GetCurrentDocumentPath();

			protected:
				FbxLoader();

				/**
				 * \brief Initializes the FBX parser
				 */
				void Initialize();

				/**
				 * \brief Shutdowns the FBX parser
				 */
				void Shutdown() const;

				/**
				 * \brief Loads objects from an .fbx file
				 * \param filename _IN_ Path of the file to be parsed
				 * \return Scene object of every objects loaded using the file\n
				 *		   Returns nullptr if nothing could be loaded
				 */
				Graphics::s_Skeleton* CoreLoadFromFile( const std::string& filename );

				/**
				 * \brief Creates a custom object from FbxNode's
				 * \param node _IN_ Node be converted
				 * \param parentNode _IN_ Parent of the node to be converted
				 * \param skeleton _IN_ s_Skeleton of the object created
				 * \return The converted scene object
				 */
				void ProcessNode( const FbxNode* node,
				                  const FbxNode* parentNode, Graphics::s_Skeleton* skeleton );

				/**
				 * \brief Creates a custom mesh and sets it to be a component of an object
				 * \param node _IN_ Node (mesh) to be set
				 * \param skeleton _IN_ s_Bone which will contain the new mesh
				 */
				void LoadMesh( const FbxNode* node, Graphics::s_Skeleton* skeleton );

				Graphics::AbstractMaterial* CreateMaterial( const FbxSurfaceMaterial* pMaterial ) const;

				//Graphics::s_AbstractMaterial * CreateMaterial( const FbxSurfaceMaterial * pMaterial );
			private:
				/**
				 * \brief Get the unique instance of the TCMFbxLoader
				 * \return A reference to the TCMFbxLoader
				 */
				static FbxLoader& GetInstance();

				/**
				 * \brief The FcbManager used when loading a .fbx
				 */
				FbxManager* m_FbxManager;

				/**
				 * \brief The main FcbScene used when loading a .fbx
				 */
				FbxScene* m_Scene;
			};
		}
	}
}

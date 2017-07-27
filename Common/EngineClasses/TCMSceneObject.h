#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include "TCMTicking.h"
#include "MathLib/Vec3.h"
#include "MathLib/Vec4.h"
#include "MathLib/Mat4.h"
#include "TCMMemory/TCMPoolObject.h"
#include "TCMMemory/TCMDynamicPoolManager.hpp"

#include "TCMLogger/Logger.h"
#include "TCMTranform.h"

namespace TCM
{
	namespace Engine
	{
		namespace Graphics
		{
			struct s_Skeleton;
		}

		namespace Physics
		{
			struct RigidBody;
		}

		class TCMComponent;

		/**
		 * \brief Base class for every object contained in a TCMScene
		 */
		class ENGINECLASSES_DLL_EXPORT TCMSceneObject final : public TCMTicking, public Memory::TCMPoolObject
		{
			TCM_CLASS(TCMSceneObject, TCMTicking);

			TCM_SERIALIZE( m_transform.position,
				m_transform.eulerAngles,
				m_transform.scale );

			static Event<>& GetOnRelationChange();

			TCMSceneObject();
			virtual ~TCMSceneObject();

			/**
			 * \brief Pseudo-constructor used instanciated by pool
			 */
			void TCMPoolCreate() override;

			/**
			 * \brief Pseudo-destructor used when released by pool
			 */
			void TCMPoolRelease() override;

			/**
			 * \brief Construct the TCMSceneObject using the path from a mesh
			 * \param path _IN_ Path of the Skeleton
			 */
			void Construct( const std::string& path );

			/**
			* \brief Construct the TCMSceneObject using the path from a mesh
			* \param path _IN_ Path of the Skeleton
			* \param filename _IN_ Filename of the Skeleton
			*/
			void Construct( const std::string& path, const std::string& filename );
			void Construct( Graphics::s_Skeleton* skeleton );

			void GenSkeleton( Graphics::s_Skeleton* skeleton );

#ifdef _DEBUG
			void* operator new( size_t size )
			{
				return ::operator new( size );
			}

			void operator delete( void* ptr, size_t size )
			{
				return ::operator delete( ptr, size );
			}
#endif

			/**
			 * \brief Initialization method called after creation
			 */
			void Init() override;

			/**
			* \brief TCM function to manage updates when being modified editor
			* \param deltaTime _IN_ Deltatime since last call
			* \warning Should not be manually called nor overriden
			*/
			void TCMEditorUpdate( const float deltaTime ) override final;

			/**
			 * \brief TCM function to manage updates
			 * \param deltaTime _IN_ Deltatime since last call
			 * \return True if can update, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			bool TCMUpdate( const float deltaTime ) override final;

			/**
			* \brief TCM function to manage fixed updates (physics)
			* \param fixedDeltaTime _IN_ fixed Deltatime since last call
			* \return True if can update, false otherwise
			* \warning Should not be manually called nor overriden
			*/
			bool TCMFixedUpdate( const float fixedDeltaTime ) override final;

			/**
			 * \brief TCM function to manage rendering
			 * \return True if can render, false otherwise
			 * \warning Should not be manually called nor overriden
			 */
			bool TCMRender() override final;

			/**
			* \brief Set if the instance should be enabled or not
			* \param bEnabled _IN_ The new state of the instance
			* \param callback _IN_OPT_ Launch the callback or not
			*/
			void SetEnabled( const bool bEnabled, const bool callback = true ) override final;

			void RenderGizmos() override final;

			/**
			 * \brief Adds a TCMComponent to the TCMSceneObject
			 * \param component _IN_ The TCMComponent to be added
			 */
			void AddComponent( TCMComponent* component );

			/**
			 * \brief Removes a TCMComponent from the TCMSceneObject
			 * \param component _IN_ The TCMComponent to be removed
			 * \return True if component was removed, false otherwise
			 */
			bool RemoveComponent( TCMComponent* component ) const;

			/**
			 * \brief Removes a TCMComponent from the TCMSceneObject
			 * \param index _IN_ The index of the TCMComponent to be removed
			 * \return True if component was removed, false otherwise
			 */
			bool RemoveComponent( const int index ) const;

			/**
			 * \brief Get a TCMComponent of a certain type
			 * \tparam T The type of the TCMComponent to get
			 * \return The first component of type T
			 * \note Usage: GetComponent<T>()
			 */
			template<typename T>
			typename std::enable_if<std::is_base_of<TCMComponent, T>::value, T*>::type GetComponent() const
			{
				for ( TCMComponent* oneComponent : *m_vecComponent )
				{
					T* castedComponent = dynamic_cast<T *>(oneComponent);
					if ( castedComponent != nullptr )
						return castedComponent;
				}
				return nullptr;
			}

			TCMComponent* GetComponent( const int index ) const;

			/**
			 * \brief Get all TCMComponent's of a certain type
			 * \tparam T The type of the TCMComponent to get
			 * \return Vector of every TCMComponent of type T
			 * \note Usage: GetAllComponent<T>()
			 */
			template<typename T>
			typename std::enable_if<std::is_base_of<TCMComponent, T>::value, std::vector<T *>>::type GetAllComponents() const
			{
				std::vector<T *> vectorComponents = std::vector<T *>();

				for ( TCMComponent* oneComponent : *m_vecComponent )
				{
					T* castedComponent = dynamic_cast<T *>(oneComponent);
					if ( castedComponent != nullptr )
						vectorComponents.push_back( castedComponent );
				}
				return vectorComponents;
			}

			/**
			 * \brief Get the vector of the TCMSceneObject 's components
			 */
			std::vector<TCMComponent *> GetAllComponents() const;

			/**
			 * \brief Get the count of components attached to the TCMSceneObject
			 */
			size_t GetComponentsCount() const;

			/**
			* \brief Get all TCMSceneObjects with component of certain type
			* \tparam T The type of the TCMComponent's to find
			* \return Vector of every TCMScenObject with T TCMComponent
			*/
			template<typename T>
			typename std::enable_if<std::is_base_of<TCMComponent, T>::value, std::vector<TCMSceneObject *>>::type GetAllChildren() const
			{
				std::vector<TCMSceneObject *> vecChilds = std::vector<TCMSceneObject *>();

				for ( auto iter = m_vecChilds->cbegin(); iter != m_vecChilds->cend(); ++iter )
				{
					if ( ( *iter )->GetComponent<T>() != nullptr )
						vecChilds.push_back( *iter );
				}

				return vecChilds;
			}

			/**
			 * \brief Get all TCMSceneObjects with component of certain type, inside children too
			 * \tparam T The type of the TCMComponent's to find
			 * \return Vector of every TCMScenObject with T TCMComponent
			 */
			template<typename T>
			typename std::enable_if<std::is_base_of<TCMComponent, T>::value, std::vector<TCMSceneObject *>>::type DeepGetAllChildren() const
			{
				std::vector<TCMSceneObject *> vecChilds = std::vector<TCMSceneObject *>();

				for ( auto iter = m_vecChilds->cbegin(); iter != m_vecChilds->cend(); ++iter )
				{
					if ( ( *iter )->GetComponent<T>() != nullptr )
						vecChilds.push_back( *iter );

					std::vector<TCMSceneObject *> childVecSceneObject = ( *iter )->DeepGetAllChildren<T>();
					vecChilds.insert( vecChilds.cend(), childVecSceneObject.cbegin(), childVecSceneObject.cend() );
				}

				return vecChilds;
			}

			/**
			 * \brief Get all children of the TCMSceneObject
			 * \return List of all TCMSceneObject children
			 */
			std::vector<TCMSceneObject *> GetAllChildren() const;

			/**
			 * \brief Get index of the child
			 * \param child _IN_ Child to find 
			 * \return Index of the child, -1 if not found
			 */
			int GetChildIndex( TCMSceneObject* child ) const;

			/**
			* \brief Get children by name of the TCMSceneObject
			* \return List of all TCMSceneObject children
			*/
			std::vector<TCMSceneObject *> GetChildren( const std::string& str ) const;

			/**
			* \brief Get children by name of the TCMSceneObject
			* \return List of all TCMSceneObject children
			*/
			std::vector<TCMSceneObject *> DeepGetChildren( const std::string& str ) const;

			/**
			 * \brief Get all children, include children of children
			 * \return List of all TCMSceneObject children
			 */
			std::vector<TCMSceneObject *> DeepGetAllChildren() const;

			/**
			 * \brief Get the TCMSceneObject at index
			 * \param index _IN_ index of the TCMSceneObject in the children list
			 * \return TCMSceneObject child at index
			 */
			TCMSceneObject* GetChild( const int index ) const;

			/**
			 * \brief Get count of children
			 * \return Count
			 */
			size_t GetChildCount() const;

			/**
			 * \brief Set the position of the scene object
			 * \param position : new position
			 */
			void SetPosition( const Math::Vec3& position );

			/**
			 * \brief Set the rotation of the scene object
			 * \param rotation : new rotation
			 */
			void SetRotation( const Math::Vec3& rotation );

			/**
			 * \brief Set the scale of the scene object
			 * \param scale : new scale
			 */
			void SetScale( const Math::Vec3& scale );

			/**
			 * \brief Get the position of the TCMSceneObject
			 * \return Current position
			 */
			Math::Vec3 GetLocalPosition() const;

			/**
			 * \brief Get the rotation of the TCMSceneObject
			 * \return Current rotation
			 */
			Math::Vec3 GetLocalRotation() const;

			/**
			 * \brief Get the scale of the TCMSceneObject
			 * \return Current scale
			 */
			Math::Vec3 GetLocalScale() const;

			/**
			 * \brief Get World Matrix, already transposed
			 * \return World Matrix of the object
			 */
			Math::Mat4 GetWorldMatrix() const;

			/**
			 * \brief Get Locale Matrix, already transposed
			 * \return Locale Matrix of the object
			 */
			Math::Mat4 GetLocaleMatrix() const;

			/**
			 * \brief Sets the parent of this TCMSceneObject
			 * \param newParent _IN_ The TCMSceneObject set as parent
			 * \note This function will NOT add the child to the parent, and is called by the parent's function AddChild()
			 */
			void SetParent( TCMSceneObject* newParent );

			/**
			 * \brief Gets the parent of this TCMSceneObject
			 * \return The TCMSceneObject set as parent
			 */
			TCMSceneObject* GetParent() const;

			/**
			 * \brief Removes parent of this TCMSceneObject
			 */
			void RemoveParent();

			/**
			 * \brief Adds child to this TCMSceneObject
			 * \param newChild _IN_ TCMSceneObject to be added as child
			 * \param position _IN_ position in the vecctor
			 */
			void AddChild( TCMSceneObject* newChild, int position = -1 );

			/**
			 * \brief Removes child from this TCMSceneObject
			 * \param oldChild _IN_ TCMSceneObject to be removed from childs
			 */
			bool RemoveChild( TCMSceneObject* oldChild ) const;

			/**
			* \brief Removes child from this TCMSceneObject
			* \param index _IN_ index of the TCMSceneObject to be removed from childs
			*/
			void RemoveChild( const int index ) const;

			/**
			* \brief Removes children from this TCMSceneObject
			* \param start _IN_ index of the first TCMSceneObject to be removed from childs
			* \param count _IN_ count of TCMSceneObject to be removed 
			*/
			bool RemoveChildren( const int start, const int count ) const;

			/**
			 * \brief Get the World Position of this TCMSceneObject
			 * \return The World Position of the object
			 */
			Math::Vec3 GetWorldPosition() const;

			/**
			 * \brief Get the World Rotation of this TCMSceneObject
			 * \return The World Rotation of the object
			 */
			Math::Vec3 GetWorldRotation() const;

			/**
			 * \brief Get the World Scale of this TCMSceneObject
			 * \return The World Scale of the object
			 */
			Math::Vec3 GetWorldScale() const;

			/**
			 * \brief Get the up vector from the current transform
			 * \return The up vector
			 */
			inline Math::Vec3 Up() const
			{
				Math::Vec4 col = GetWorldMatrix().GetRow( 1 );
				return Math::Vec3( col.x, col.y, -col.z ).Normalized();
			};

			/**
			 * \brief Get the right vector from the current transform
			 * \return The right vector
			 */
			Math::Vec3 Right() const
			{
				Math::Vec4 col = GetWorldMatrix().GetRow( 0 );
				return Math::Vec3( col.x, col.y, -col.z ).Normalized();
			};

			/**
			 * \brief Get the forward vectosswqqwwqwqwsdwwewsr from the current transform
			 * \return The forward vector
			 */
			Math::Vec3 Forward() const
			{
				Math::Vec4 col = GetWorldMatrix().GetRow( 2 );
				return Math::Vec3( col.x, col.y, -col.z ).Normalized();
			};

			void OnParentTransformChanged();

			/**
			 * \brief Callback when the TCMSceneObject collide
			 * \param body _IN_ RigidBody it has collided with
			 */
			void OnCollisionEnter( Physics::RigidBody* body ) const;

			/**
			* \brief Callback when the TCMSceneObject exit a collision
			*/
			void OnCollisionExit() const;

			bool m_isSelected;

		protected:
			/**
			 * \brief Called when destroying the instance
			 */
			void OnDestroy() override;

			/**
			 * \brief Notify TCMComponents's that transform has changed
			 */
			void NotifyWorldMatChanged() const;
			/**
			 * \brief Transform struct of this TCMSceneObject
			 */
			Transform m_transform;

			/**
			 * \brief World transform struct of this TCMSceneObject
			 */
			Transform m_worldTransform;

		private:

			/**
			* \brief Removes child from this TCMSceneObject
			* \param oldChild _IN_ TCMSceneObject to be removed from childs
			*/
			bool _RemoveChild( TCMSceneObject* oldChild ) const;

			/**
			 * \brief Pointer to the parent of this object
			 */
			TCMSceneObject* m_parent;

			/**
			 * \brief Calculate new World Matrix
			 * \note Used to optimize GetWorldMatrix() calls
			 */
			void CalcWorldMatrix();

			/**
			 * \brief Vector containing every TCMSceneObject having this instance as parent
			 */
			std::vector<TCMSceneObject *>* m_vecChilds;

			/**
			 * \brief Vector containing every TCMComponent held by this TCMSceneObject
			 */
			std::vector<TCMComponent *>* m_vecComponent;

			/**
			 * \brief Current World Matrix
			 * \note Changed only at moments by CalcWorldMatrix()
			 */
			Math::Mat4 m_worldMatrix;

			/**
			* \brief Transform cached used for editor update
			*/
			Transform m_transformCached;

#pragma region Static - Factory / Pool
		public:
			/**
			 * \brief Creates a TCMSceneObject
			 * \tparam TDerived The derived type of the TCMSceneObject to be instanciated
			 * \return A new object of type TDerived
			 * \note Don't always call type constructor. Calls TCMPoolCreate(), but no Construct()
			 */
			template<class TDerived = TCMSceneObject>
			static TDerived* Create()
			{
				return GetInstance().New<TDerived>();
			};

			/**
			 * \brief Creates a TCMSceneObject
			 * \tparam TDerived The derived type of the TCMSceneObject to be instanciated
			 * \tparam Args The types of the arguments taken by the Construct()
			 * \param args _IN_ The arguments sent to Construct()
			 * \return A new object of type TDerived
			 * \note Don't always call type constructor. Calls TCMPoolCreate(), and Construct(args)
			 */
			template<class TDerived = TCMSceneObject, typename... Args>
			static TDerived* Create( Args&&... args )
			{
				return GetInstance().New<TDerived>( args... );
			};

			/**
			 * \brief Releases object
			 * \param object _IN_ The TCMSceneObject to be deleted
			 * \note Will never call type destructor. Calls TCMPoolRelease() only
			 */
			static void Release( TCMSceneObject* object )
			{
				GetInstance().Delete( object );
			};

		private:
			/**
			 * \brief Get the unique instance of the TCMDynamicPoolManager<TCMSceneObject>
			 * \return A reference to the TCMDynamicPoolManager<TCMSceneObject>
			 */
			static Memory::TCMDynamicPoolManager<TCMSceneObject>& GetInstance()
			{
				static std::unique_ptr<Memory::TCMDynamicPoolManager<TCMSceneObject>> m_instance;
				static std::once_flag m_onceFlag;

				std::call_once( m_onceFlag,
				                []()
				                {
					                m_instance.reset( new Memory::TCMDynamicPoolManager<TCMSceneObject>() );
				                }
				);
				return *( m_instance.get() );
			};
#pragma endregion
		};
	}
}

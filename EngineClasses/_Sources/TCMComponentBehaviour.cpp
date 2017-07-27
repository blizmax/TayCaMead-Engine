#include "TCMComponentBehaviour.h"

namespace TCM
{
	namespace Engine
	{
		TCMComponentBehaviour::TCMComponentBehaviour()
		{
			SetName( "TCMComponentBehaviour" );
		}

		TCMComponentBehaviour::~TCMComponentBehaviour()
		{
		}

		void TCMComponentBehaviour::TCMEditorUpdate( const float deltaTime )
		{
			// Do nothing special if you are a user behaviour
		}

		void TCMComponentBehaviour::TCMPoolCreate()
		{
			Super::TCMPoolCreate();

			Awake();
		}

		void TCMComponentBehaviour::TCMPoolRelease()
		{
			// TODO: should probably do something else

			Super::TCMPoolRelease();
		}

		void TCMComponentBehaviour::Init()
		{
			Super::Init();

			Start();
		}

		bool TCMComponentBehaviour::TCMUpdate( const float deltaTime )
		{
			return Super::TCMUpdate( deltaTime );
		}

		bool TCMComponentBehaviour::TCMFixedUpdate( const float fixedDeltaTime )
		{
			return Super::TCMFixedUpdate( fixedDeltaTime );
		}

		void TCMComponentBehaviour::Awake()
		{
			// Do nothing special if not overriden
		}

		void TCMComponentBehaviour::Start()
		{
			// Do nothing special if not overriden
		}

		void TCMComponentBehaviour::Update( const float deltaTime )
		{
			// Do nothing special if not overriden
		}

		void TCMComponentBehaviour::FixedUpdate( const float fixedDeltaTime )
		{
			// Do nothing special if not overriden
		}
	}
}

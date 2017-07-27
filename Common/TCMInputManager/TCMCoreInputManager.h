#pragma once
#include "TCMInputManager/BaseInputManagerKeys.h"
#include "MathLib/Vec2.h"

template<typename T>
class TCMCoreInputManager abstract
{
public:
	virtual ~TCMCoreInputManager() = default;

	virtual bool BaseIsFocused() const = 0;
	virtual void UpdateMousePosition( TCM::Engine::Math::Vec2& pos ) = 0;
	virtual void UpdateKeysState() = 0;

	bool KeyDown( SMouseInput key ) const
	{
		if ( key == SMouseInput::NO_MOUSE_INPUT )
			return true;
		return CoreKeyDown( key );
	};

	bool KeyUp( SMouseInput key ) const
	{
		if ( key == SMouseInput::NO_MOUSE_INPUT )
			return true;
		return CoreKeyUp( key );
	};

	bool KeyToggled( SMouseInput key ) const
	{
		if ( key == SMouseInput::NO_MOUSE_INPUT )
			return true;
		return CoreKeyToggled( key );
	};

	bool KeyDown( SKey key ) const
	{
		if ( key == SKey::NO_KEY_INPUT )
			return true;
		return CoreKeyDown( key );
	};

	bool KeyUp( SKey key ) const
	{
		if ( key == SKey::NO_KEY_INPUT )
			return true;
		return CoreKeyUp( key );
	};

	bool KeyToggled( SKey key ) const
	{
		if ( key == SKey::NO_KEY_INPUT )
			return true;
		return CoreKeyToggled( key );
	};

	bool KeyDown( SSpecialKey key ) const
	{
		if ( key == SSpecialKey::NO_SPECIAL_KEY_INPUT )
			return true;
		return CoreKeyDown( key );
	};

	bool KeyUp( SSpecialKey key ) const
	{
		if ( key == SSpecialKey::NO_SPECIAL_KEY_INPUT )
			return true;
		return CoreKeyUp( key );
	};

	bool KeyToggled( SSpecialKey key ) const
	{
		if ( key == SSpecialKey::NO_SPECIAL_KEY_INPUT )
			return true;
		return CoreKeyToggled( key );
	};

protected:
	TCMCoreInputManager() = default;

	virtual bool CoreKeyDown( SMouseInput key ) const = 0;
	virtual bool CoreKeyUp( SMouseInput key ) const = 0;
	virtual bool CoreKeyToggled( SMouseInput key ) const = 0;

	virtual bool CoreKeyDown( SKey key ) const = 0;
	virtual bool CoreKeyUp( SKey key ) const = 0;
	virtual bool CoreKeyToggled( SKey key ) const = 0;

	virtual bool CoreKeyDown( SSpecialKey key ) const = 0;
	virtual bool CoreKeyUp( SSpecialKey key ) const = 0;
	virtual bool CoreKeyToggled( SSpecialKey key ) const = 0;

	T mouseInputs[INT(SMouseInput::COUNT)];
	T specialKeys[INT(SSpecialKey::COUNT)];
	T keys[INT(SKey::COUNT)];

	inline T GetMouseInput( SMouseInput mouseInput ) const
	{
		return mouseInputs[INT(mouseInput)];
	}

	inline T GetSpecialKey( SSpecialKey specialKey ) const
	{
		return specialKeys[INT(specialKey)];
	}

	inline T GetKey( SKey key ) const
	{
		return keys[INT(key)];
	}
};

#ifndef VOLT3D_EASE_EXPO_H
#define VOLT3D_EASE_EXPO_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseExpoIn
	*	@brief Updates action with expo in ease function.
	*/
	class VOLT3D_DLL EaseExpoIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseExpoIn();

	public:
		// Destructor
		~EaseExpoIn();

		// Create
		static v3d::EaseExpoIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseExpoIn
	*	@brief Updates action with expo out ease function.
	*/
	class VOLT3D_DLL EaseExpoOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseExpoOut();

	public:
		// Destructor
		~EaseExpoOut();

		// Create
		static v3d::EaseExpoOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseExpoIn
	*	@brief Updates action with expo in out ease function.
	*/
	class VOLT3D_DLL EaseExpoInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseExpoInOut();

	public:
		// Destructor
		~EaseExpoInOut();

		// Create
		static v3d::EaseExpoInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif
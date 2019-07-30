#ifndef VOLT3D_EASE_BOUNCE_H
#define VOLT3D_EASE_BOUNCE_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseBounceBase
	*	@brief Base class of bounce ease action.
	*/
	class VOLT3D_DLL EaseBounceBase : public v3d::Ease
	{
	protected:
		// Constructor
		EaseBounceBase();

		// bounce formula
		float getBounce(float t, const float d) const;

	public:
		// Destructor
		~EaseBounceBase();
	};


	/**
	*	@class EaseBounceIn
	*	@brief Updates action with bounce in ease function.
	*/
	class VOLT3D_DLL EaseBounceIn : public v3d::EaseBounceBase
	{
	private:
		// Constructor
		EaseBounceIn();

	public:
		// Destructor
		~EaseBounceIn();

		// Create
		static v3d::EaseBounceIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseBounceIn
	*	@brief Updates action with bounce out ease function.
	*/
	class VOLT3D_DLL EaseBounceOut : public v3d::EaseBounceBase
	{
	private:
		// Constructor
		EaseBounceOut();

	public:
		// Destructor
		~EaseBounceOut();

		// Create
		static v3d::EaseBounceOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseBounceIn
	*	@brief Updates action with bounce in out ease function.
	*/
	class VOLT3D_DLL EaseBounceInOut : public v3d::EaseBounceBase
	{
	private:
		// Constructor
		EaseBounceInOut();

	public:
		// Destructor
		~EaseBounceInOut();

		// Create
		static v3d::EaseBounceInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif
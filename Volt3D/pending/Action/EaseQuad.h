#ifndef VOLT3D_EASE_QUAD_H
#define VOLT3D_EASE_QUAD_H

#include "Volt3D/Macro/Define.h"

#include "Ease.h"

namespace v3d
{
	/**
	*	@class EaseQuadIn
	*	@brief Updates action with quad in ease function.
	*/
	class VOLT3D_DLL EaseQuadIn : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuadIn();

	public:
		// Destructor
		~EaseQuadIn();

		// Create
		static v3d::EaseQuadIn* create(v3d::Action* action);
		
		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseQuadIn
	*	@brief Updates action with quad out ease function.
	*/
	class VOLT3D_DLL EaseQuadOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuadOut();

	public:
		// Destructor
		~EaseQuadOut();

		// Create
		static v3d::EaseQuadOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};



	/**
	*	@class EaseQuadIn
	*	@brief Updates action with quad out ease function.
	*/
	class VOLT3D_DLL EaseQuadInOut : public v3d::Ease
	{
	private:
		// Constructor
		EaseQuadInOut();

	public:
		// Destructor
		~EaseQuadInOut();

		// Create
		static v3d::EaseQuadInOut* create(v3d::Action* action);

		// override. Get ease time.
		float getT() const override;
	};
}

#endif
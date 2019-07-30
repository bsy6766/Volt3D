#ifndef VOLT3D_TINT_TO_H
#define VOLT3D_TINT_TO_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

#include "Volt3D/Color/Color3F.h"

namespace v3d
{
	// forward declaration
	class RenderNode;
	class TransformNode;

	/**
	*	@class TintTo
	*	@brief Changes color of object by changing color to given color in duration.
	*/
	class VOLT3D_DLL TintTo : public v3d::Action
	{
	private:
		// Constructor
		TintTo();

		// target. Need TransformNode at least.
		v3d::RenderNode* target;

		// color
		v3d::Color3F color;
		v3d::Color3F colorDelta;

		// init
		bool init(const float duration, const v3d::Color3F& color);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~TintTo();

		/**
		*	Creates RotateTo action
		*	@param duration Duration of action
		*	@param angle Angle to rotate
		*/
		static v3d::TintTo* create(const float duration, const v3d::Color3F& color);

		// override. Set target.
		bool setTarget(v3d::TransformNode* renderNodeTarget) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
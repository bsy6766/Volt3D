#ifndef VOLT3D_TINT_BY_H
#define VOLT3D_TINT_BY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

#include "Volt3D/Color/Color3F.h"

namespace v3d
{
	// forward declaration
	class RenderNode;
	class TransformNode;

	/**
	*	@class TintBy
	*	@brief Adds color of object with amount of color given in duration.
	*/
	class VOLT3D_DLL TintBy : public v3d::Action
	{
	private:
		// Constructor
		TintBy();

		// target. Need TransformNode at least.
		v3d::RenderNode* target;

		// color
		v3d::Color3F color;

		// init
		bool init(const float duration, const v3d::Color3F& color);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~TintBy();

		/**
		*	Creates RotateTo action
		*	@param duration Duration of action
		*	@param angle Angle to rotate
		*/
		static v3d::TintBy* create(const float duration, const v3d::Color3F& color);

		// override. Set target.
		bool setTarget(v3d::TransformNode* renderNodeTarget) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
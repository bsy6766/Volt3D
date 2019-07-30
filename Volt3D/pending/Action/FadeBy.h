#ifndef VOLT3D_FADE_BY_H
#define VOLT3D_FADE_BY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;
	class RenderNode;

	/**
	*	@class FadeBy
	*	@brief Fades object by changing opacity with amount of opacity given in duration.
	*/
	class VOLT3D_DLL FadeBy : public v3d::Action
	{
	private:
		// Constructor
		FadeBy();

		// opacity
		float opacityDelta;

		// target. Need TransformNode at least.
		v3d::RenderNode* target;

		// init
		bool init(const float duration, const float opacity);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~FadeBy();

		/**
		*	Creates FadeBy action
		*	@param duration Duration of action. [0.0f,]
		*	@param opacity Opacity to change. [0.0f, 1.0f]
		*	@return A FadeBy instance if success. Else, nullptr.
		*/
		static FadeBy* create(const float duration, const float opacity);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
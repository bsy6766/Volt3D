#ifndef VOLT3D_FADE_TO_H
#define VOLT3D_FADE_TO_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;
	class RenderNode;

	/**
	*	@class FadeTo
	*	@brief Fades object by changing opacity to given opacity in duration.
	*/
	class VOLT3D_DLL FadeTo : public v3d::Action
	{
	private:
		// Constructor
		FadeTo();

		// opacity
		float opacity;
		float opacityDelta;

		// target. Need TransformNode at least.
		v3d::RenderNode* target;

		// init
		bool init(const float duration, const float opacity);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~FadeTo();

		/**
		*	Creates FadeTo action
		*	@param duration Duration of action
		*	@param opacity Opacity to change
		*/
		static FadeTo* create(const float duration, const float opacity);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
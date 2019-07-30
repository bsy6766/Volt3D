#ifndef VOLT3D_SCALE_TO_H
#define VOLT3D_SCALE_TO_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class ScaleTo
	*	@brief Scale object by changing scale to given scale in duration.
	*/
	class VOLT3D_DLL ScaleTo : public v3d::Action
	{
	private:
		// Constructor
		ScaleTo();

		// scale in x and y axis
		glm::vec3 scale;
		glm::vec3 scaleDelta;

		// target. Need TransformNode at least.
		v3d::TransformNode* target;

		// init
		bool init(const float duration, const glm::vec3& scale);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~ScaleTo();

		/**
		*	Creates RotateTo action
		*	@param duration Duration of action
		*	@param scale Scale in x y axis.
		*/
		static ScaleTo* create(const float duration, const glm::vec3& scale);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
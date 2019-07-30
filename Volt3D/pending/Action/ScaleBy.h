#ifndef VOLT3D_SCALE_BY_H
#define VOLT3D_SCALE_BY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class ScaleBy
	*	@brief Scales object by changing scale with amount of scale given in duration.
	*/
	class VOLT3D_DLL ScaleBy : public v3d::Action
	{
	private:
		// Constructor
		ScaleBy();

		// scale in x and y axis
		glm::vec3 scaleDelta;

		// target. Need TransformNode at least.
		v3d::TransformNode* target;

		// init
		bool init(const float duration, const glm::vec3& scale);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~ScaleBy();

		/**
		*	Creates RotateTo action
		*	@param duration Duration of action
		*	@param scale Scale in x y axis.
		*/
		static ScaleBy* create(const float duration, const glm::vec3& scale);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
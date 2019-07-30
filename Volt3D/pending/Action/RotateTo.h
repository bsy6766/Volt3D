#ifndef VOLT3D_ROTATE_TO_H
#define VOLT3D_ROTATE_TO_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

#include <glm/glm.hpp>

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class RotateTo
	*	@brief Rotates object by changing rotation angle to given angle in duration.
	*/
	class VOLT3D_DLL RotateTo : public v3d::Action
	{
	private:
		// Constructor
		RotateTo();

		// angle
		glm::vec3 angle;
		glm::vec3 angleDelta;

		// target. Need TransformNode at least.
		v3d::TransformNode* target;

		// init
		bool init(const float duration, const glm::vec3 angle);

		// override
		void updateEase(const float delta) override;
		
	public:
		// Destructor
		~RotateTo();

		/**
		*	Creates RotateTo action
		*	@param duration Duration of action
		*	@param angle Final angle of node to rotate.
		*/
		static v3d::RotateTo* create(const float duration, const glm::vec3 angle);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
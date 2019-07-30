#ifndef VOLT3D_ROTATE_BY_H
#define VOLT3D_ROTATE_BY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

#include <glm/glm.hpp>

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class RotateBy
	*	@brief Rotates object by changing rotation angle with amount of angle given in duration.
	*/
	class VOLT3D_DLL RotateBy : public v3d::Action
	{
	private:
		// Constructor
		RotateBy();

		// angle
		glm::vec3 angleDelta;

		// target. Need TransformNode at least.
		v3d::TransformNode* target;

		// init
		bool init(const float duration, const glm::vec3 angle);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~RotateBy();

		/**
		*	Creates RotateBy action
		*	@param duration Duration of action
		*	@param angle Final angle of node to rotate.
		*/
		static v3d::RotateBy* create(const float duration, const glm::vec3 angle);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
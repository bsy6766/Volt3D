#ifndef VOLT3D_MOVE_TO_H
#define VOLT3D_MOVE_TO_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class MoveTo
	*	@brief Translates object by changing position to given position in duration.
	*/
	class VOLT3D_DLL MoveTo : public v3d::Action
	{
	private:
		// Constructor
		MoveTo();

		// destination
		glm::vec3 distance;
		glm::vec3 destination;

		// target. Need TransformNode at least.
		v3d::TransformNode* target;
		
		// init
		bool init(const float duration, const glm::vec3& destination);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~MoveTo();

		/**
		*	Creates MoveTo action
		*	@param duration Duration of action
		*	@param destination Position to move in xyz axis
		*/
		static MoveTo* create(const float duration, const glm::vec3& destination);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
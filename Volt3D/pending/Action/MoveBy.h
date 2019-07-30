#ifndef VOLT3D_MOVE_BY_H
#define VOLT3D_MOVE_BY_H

#include "Action.h"

#include "Volt3D/Macro/Define.h"

namespace v3d
{
	// forward declaration
	class TransformNode;

	/**
	*	@class MoveBy
	*	@brief Translates object by changing position with amount of distance given in duration.
	*/
	class VOLT3D_DLL MoveBy : public v3d::Action
	{
	private:
		// Constructor
		MoveBy();

		// distance
		glm::vec3 distance;

		// target. Need TransformNode at least.
		v3d::TransformNode* target;

		// init
		bool init(const float duration, const glm::vec3& distance);

		// override
		void updateEase(const float delta) override;

	public:
		// Destructor
		~MoveBy();

		/**
		*	Creates MoveBy action
		*	@param duration Duration of action
		*	@param distance Distance to move
		*/
		static MoveBy* create(const float duration, const glm::vec3& distance);

		// override. Set target.
		bool setTarget(v3d::TransformNode* target) override;

		// ovrrides. Update action
		void update(const float delta) override;
	};
}

#endif
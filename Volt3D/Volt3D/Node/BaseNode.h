#ifndef V3D_BASE_NODE_H
#define V3D_BASE_NODE_H

#include <string>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class BaseNode
	*	@brief Base class of all objects in the engine including all 3D, 2D and UI.
	*
	*	BaseNode class is a simple metadata class that contains integer id and string name.
	*/
	class VOLT3D_DLL BaseNode
	{
	private:
		static unsigned int idCounter;

	protected:
		BaseNode(const std::string& name);

		std::string name;
		unsigned int id;
		bool active;

	public:
		virtual ~BaseNode();

		/**
		*	Get ID.
		*	@return Unique ID of this node.
		*/
		unsigned int getID() const;

		/**
		*	Get name.
		*	@return Name of this node.
		*/
		std::string getName() const;

		/**
		*	Set active.
		*	Enabled node will be updated and rendered.
		*	@param active true or false to set.
		*/
		virtual void setActive(const bool active);

		/**
		*	Check if node is active.
		*	@return true if node is active. Else, false.
		*/
		bool isActive() const;

		/**
		*	Log BaseNode information.
		*/
		virtual void print(const bool printBase) const;
	};
}

#endif
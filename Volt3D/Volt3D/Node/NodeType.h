/**
*	@file NodeType.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_NODE_TYPE_H
#define V3D_NODE_TYPE_H

namespace v3d
{
	/**
	*	@enum NodeType
	*	Type of nodes in Volt3D engine.
	*
	*	0 means undefined node type or uninitialized state.
	*/
	enum class NodeType
	{
		// Node lives in 3D space.
		e3D,
		// Node lives in 2D space.
		e2D,
		// Node lives in UI (Screen) space.
		// @note _UI is defined in "tchar.h" on windows 10 sdk...why...
		eGUI,
		// Scene work as root node.
		eScene,
	};

	inline std::string nodeTypeToString(v3d::NodeType value)
	{
		switch (value)
		{
		case v3d::NodeType::e3D: return "3D";
		case v3d::NodeType::e2D: return "2D";
		case v3d::NodeType::eGUI: return "GUI";
		case v3d::NodeType::eScene: return "Scene";
		default: return "Invalid node type";
		}
	}
}

#endif
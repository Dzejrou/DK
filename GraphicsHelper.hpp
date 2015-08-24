#pragma once

#include <cstdlib>
class EntitySystem;

namespace GraphicsHelper
{
	/**
	 * Brief: Sets the model of a given entity.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 * Param: Name of the new mesh.
	 * Note: Requires a call to init_graphics_component to take effect.
	 */
	void set_mesh(EntitySystem&, std::size_t, const std::string&);

	/**
	 * Brief: Returns the name of the mesh of a given entity.
	 * Param: Reference to the entity system that contains components.
	 */
	const std::string& get_mesh(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the name of the material a given entity is using.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 * Param: Name of the entity.
	 * Note: Requires a call to init_graphcis_component to take effect.
	 */
	void set_material(EntitySystem&, std::size_t, const std::string&);

	/**
	 * Brief: Returns the name of the material a given entity is using.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 */
	const std::string& get_material(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the visibility status of a given entity.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 * Param: True for visible, false for invisible.
	 */
	void set_visible(EntitySystem&, std::size_t, bool);

	/**
	 * Brief: Returns true if a given entity is visible, false otherwise.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 */
	bool is_visible(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the manual scaling status of a given entity.
	 *        (i.e. if the model should use dimensions stored in the mesh file
	 *         or has them explicitly set.)
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 * Param: True for manual scaling, false for using the scale of the mesh.
	 * Note: Requires a call to init_graphics_component to take effect.
	 */
	void set_manual_scaling(EntitySystem&, std::size_t, bool);

	/**
	 * Brief: Returns true if a given entity's model has explicit dimensions
	 *        or false if the dimensions of it's mesh are used.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 */
	bool get_manual_scaling(EntitySystem&, std::size_t);

	/**
	 * Brief: Changes the dimensions of a given entity (requires
	 *        manual_scaling to be true).
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 * Param: The new scale value.
	 * Note: Requires a call to init_graphics_component to take effect.
	 */
	void set_scale(EntitySystem&, std::size_t, const Ogre::Vector3&);

	/**
	 * Brief: Returns the scale (the dimensions) of a given entity (requires
	 *        manual_scaling to be true).
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 */
	const Ogre::Vector3& get_scale(EntitySystem&, std::size_t);

	/**
	 * Brief: Rotates a given entity so that it faces another one.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the first entity.
	 * Param: ID of the second entity.
	 */
	void look_at(EntitySystem&, std::size_t, std::size_t);

	/**
	 * Brief: Rotates a given entity by a given amount of radians.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the entity.
	 * Param: Rotation angle in radians.
	 * Note: Ogre3D has conversion functions.
	 */
	void rotate(EntitySystem&, std::size_t, Ogre::Real);

	/**
	 * Brief: Returns a given entity's bounding box.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of th entity.
	 * Note: The entity has to have a GraphicsComponent, because collision detection is
	 *       done using Ogre's bounding boxes.
	 */
	const Ogre::AxisAlignedBox& get_bounds(EntitySystem&, std::size_t);

	/**
	 * Brief: Returns true if two given entities collide, false otherwise.
	 * Param: Reference to the entity system that contains components.
	 * Param: ID of the first entity.
	 * Param: ID of the second entity.
	 */
	bool collide(EntitySystem&, std::size_t, std::size_t);
}
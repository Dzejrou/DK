#pragma once

#include <Ogre.h>
#include <vector>
#include <limits>
#include <string>

/**
 * Manages polygon precise raycasting used with half walls that have empty spaces
 * in their bounding boxes.
 * Note: Strongly inspired by http://www.ogre3d.org/tikiwiki/Raycasting+to+the+polygon+level from
 *       the official Ogre3D wiki, big thanks to all contributors.
 */
class RayCaster
{
	public:
		/**
		 * Constructor.
		 * Param: Scene manager that is used to create the ray query.
		 */
		RayCaster(Ogre::SceneManager&);

		/**
		 * Brief: Casts a ray that checks for polygon level collisions on it's way.
		 *        Returns a pair of a bool, signaling whether a hit was made and
		 *        a distance to the place of the collision, if any occured.
		 * Param: Starting position of the ray.
		 * Param: Direction of the ray.
		 * Param: Name of the entity to be ignored (the target) if it's a wall.
		 */
		std::pair<bool, Ogre::Real> cast(const Ogre::Vector3&, const Ogre::Vector3&,
											const std::string& = "") const;
	private:
		/**
		 * Query used for the collision ray cast.
		 */
		Ogre::RaySceneQuery* query_;

		/**
		 * Brief: Returns information about vertices of an entity by modifying it's size_t and
		 *        vector parameters.
		 * Param: Entity to be checked.
		 * Param: Number of vertices, will be set inside the function.
		 * Param: Number of indices, will be set inside the function.
		 * Param: Vector of vertex point positions, will be filled inside the function.
		 * Param: Vector of indices of the vertex points inside the vertex vector above.
		 * Param: Position of the entity.
		 * Param: Orientation of the entity.
		 * Param: Scale of the entity.
		 */
		void get_info(const Ogre::Entity&, std::size_t&, std::size_t&, std::vector<Ogre::Vector3>&,
					  std::vector<std::size_t>&, const Ogre::Vector3&, const Ogre::Quaternion&,
					  const Ogre::Vector3&) const;
};
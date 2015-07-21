#pragma once

#include <Ogre.h>
#include <OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <memory>
#include <vector>

#include "EntitySystem.hpp"
#include "HealthSystem.hpp"
#include "MovementSystem.hpp"
#include "AISystem.hpp"
#include "InputSystem.hpp"
#include "lppscript/LppScript.hpp"
#include "Console.hpp"

enum class GAME_STATE
{
	RUNNING, ENDED
};

class Game : public Ogre::FrameListener, public OIS::KeyListener,
			 public OIS::MouseListener, public Ogre::WindowEventListener
{
	public:
		/**
		 * Constructor.
		 */
		Game();

		/**
		 * Destructor.
		 */
		~Game();

		/**
		 * Brief: Starts the game.
		 */
		void run();

		/**
		 * Brief: Updates the game in one frame.
		 * Param: Time since the last frame.
		 */
		void update(Ogre::Real);

		/**
		 * Brief: Changes the game's state.
		 * Param: The new state.
		 */
		void set_state(GAME_STATE);
	protected:
		/**
		 * Inherited methods (callbacks).
		 * Handle Ogre3D and OIS related stuff.
		 */
		bool frameRenderingQueued(const Ogre::FrameEvent&) override;
		bool keyPressed(const OIS::KeyEvent&) override;
		bool keyReleased(const OIS::KeyEvent&) override;
		bool mouseMoved(const OIS::MouseEvent&) override;
		bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID) override;
		bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID) override;
		void windowResized(Ogre::RenderWindow* rw) override;
		void windowClosed(Ogre::RenderWindow* rw) override;
	private:
		/**
		 * Init methods.
		 */
		void ogre_init();
		void ois_init();
		void level_init();
		void lua_init();
		void cegui_init();

		/**
		 * Current game state.
		 */
		GAME_STATE state_;

		/**
		 * Pointers to Ogre3D objects.
		 */
		std::unique_ptr<Ogre::Root> root_;
		Ogre::SceneManager* scene_mgr_;
		Ogre::RenderWindow* window_;
		Ogre::Camera* main_cam_;
		Ogre::Viewport* main_view_;
		Ogre::Light* main_light_;

		OIS::InputManager* input_;
		OIS::Keyboard* keyboard_;
		OIS::Mouse* mouse_;
		Ogre::Vector3 camera_dir_;

		/**
		 * Unique pointers to systems (sadly all systems require the EntitySystem, which
		 * requires Ogre::SceneManager, so the all have to be instantiated after the ogre_init method
		 * call and thus cannot be references).
		 */
		std::unique_ptr<EntitySystem> entity_system_;
		std::unique_ptr<HealthSystem> health_system_;
		std::unique_ptr<MovementSystem> movement_system_;
		std::unique_ptr<AISystem> ai_system_;
		std::unique_ptr<InputSystem> input_system_;

		/**
		 * Vector of all systems used for updating the game's logic.
		 */
		std::vector<System*> systems_;

		/**
		 * CEGUI related stuff.
		 */
		CEGUI::OgreRenderer* renderer_;
		CEGUI::MouseButton ois_to_cegui(OIS::MouseButtonID);

		/**
		 * Ingame console.
		 */
		Console console_;

		/**
		 * These methods are to be used from Lua, note that Lua can register only
		 * static functions, so a static pointer to the instance of this game is created in
		 * the game's constructor and used in these static functions.
		 */
		static Game* lua_this;
		static int lua_create_entity(lpp::Script::state);
		static int lua_destroy_entity(lpp::Script::state);
		static int lua_add_component(lpp::Script::state);
		static int lua_delete_component(lpp::Script::state);

		// Movement system.
		static int lua_move_to(lpp::Script::state);
		static int lua_move(lpp::Script::state);
		static int lua_rotate(lpp::Script::state);
		static int lua_is_moving(lpp::Script::state);
		static int lua_is_solid(lpp::Script::state);
		static int lua_can_move_to(lpp::Script::state);
		static int lua_collide(lpp::Script::state);
		static int lua_get_distance(lpp::Script::state);
		static int lua_get_position(lpp::Script::state);
		static int lua_get_speed_modifier(lpp::Script::state);
		static int lua_set_speed_modifier(lpp::Script::state);
		static int lua_enemy_in_radius(lpp::Script::state);
		static int lua_closest_enemy(lpp::Script::state);
		static int lua_dir_to_closest_enemy(lpp::Script::state);
		static int lua_dir_to_closest_enemy_in_radius(lpp::Script::state);
		static int lua_dir_to_enemy(lpp::Script::state);
		static int lua_get_dir(lpp::Script::state);
		static int lua_get_dir_back(lpp::Script::state);
		static int lua_get_dir_left(lpp::Script::state);
		static int lua_get_dir_right(lpp::Script::state);
		static int lua_get_angle(lpp::Script::state);
		static int lua_get_angle_between(lpp::Script::state);

		// Health system.
		static int lua_get_health(lpp::Script::state);
		static int lua_add_health(lpp::Script::state);
		static int lua_sub_health(lpp::Script::state);
		static int lua_heal(lpp::Script::state);
		static int lua_buff(lpp::Script::state);
		static int lua_get_defense(lpp::Script::state);
		static int lua_add_defense(lpp::Script::state);
		static int lua_sub_defense(lpp::Script::state);

		// AI system.
		static int lua_is_friendly(lpp::Script::state);
		static int lua_is_neutral(lpp::Script::state);
		static int lua_is_inanimate(lpp::Script::state);
		static int lua_get_blueprint(lpp::Script::state);
		static int lua_get_state(lpp::Script::state);
		static int lua_get_faction(lpp::Script::state);

		// Input system.
		static int lua_set_input_handler(lpp::Script::state);
};
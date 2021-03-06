#pragma once

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <memory>
#include <vector>
#include <Enums.hpp>
#include <Typedefs.hpp>
#include <gui/OptionsWindow.hpp>
namespace level_generators
{
	class LevelGenerator;
}
class System;
class EntitySystem;
class HealthSystem;
class MovementSystem;
class AISystem;
class InputSystem;
class GridSystem;
class TaskSystem;
class CombatSystem;
class ProductionSystem;
class TimeSystem;
class EventSystem;
class GraphicsSystem;
class TriggerSystem;
class ManaSpellSystem;
class WaveSystem;
class AnimationSystem;
class Camera;
class Spellcaster;
class SelectionBox;
class GameSerializer;
class EntityPlacer;
class EntityCreator;
class LevelGenerator;
class DeferredShadingSystem;

class Game : public Ogre::FrameListener, public OIS::KeyListener,
			 public OIS::MouseListener, public Ogre::WindowEventListener
{
	friend class GameSerializer;
	friend class LuaInterface;
	friend class GUI;
	friend class NewGameDialog;
	friend void action::QUICK_LOAD();
	friend void action::QUICK_SAVE();
	friend void action::RESET_CAMERA();
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
		 * \brief Starts the game.
		 */
		void run();

		/**
		 * \brief Updates the game in one frame.
		 * \param Time since the last frame.
		 */
		void update(tdt::real);

		/**
		 * \brief Changes the game's state.
		 * \param The new state.
		 */
		void set_state(GAME_STATE);

		/**
		 * \brief Creates a new game with the given dimensions.
		 * \param Width of the level.
		 * \param Height of the level.
		 */
		void new_game(tdt::uint, tdt::uint);

		/**
		 * \brief Creates an empty level with the given dimensions.
		 * \param Width of the level.
		 * \param Height of the level.
		 */
		void create_empty_level(tdt::uint, tdt::uint);

		/**
		 * \brief Resets the main camera's position and orientation to it's original state.
		 */
		void reset_camera();

		/**
		 * \brief Restores the unlocks to their initial state.
		 */
		void reset_unlocks();

		/**
		 * \brief Sets the ID of the entity that represents the
		 *        Dungeon Throne.
		 * \param The new ID.
		 */
		void set_throne_id(tdt::uint);

		/**
		 * \brief Returns the ID of the entity that represents
		 *        the Dungeon Throne
		 */
		tdt::uint get_throne_id() const;

	protected:
		/*
		 * Inherited methods (callbacks).
		 * Handle Ogre3D and OIS related stuff.
		 * Those redundant comments above the function definitions
		 * are there to avoid Doxygen problems.
		 */

		/**
		 * Called when the previous frame is queued for rendering,
		 * used for game updates.
		 */
		bool frameRenderingQueued(const Ogre::FrameEvent&) override;

		/**
		 * Called when a key is pressed.
		 */
		bool keyPressed(const OIS::KeyEvent&) override;

		/**
		 * Called when a key is released.
		 */
		bool keyReleased(const OIS::KeyEvent&) override;

		/**
		 * Called when the mouse is moved.
		 */
		bool mouseMoved(const OIS::MouseEvent&) override;

		/**
		 * Called when a mouse button is pressed.
		 */
		bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID) override;

		/**
		 * Called when a mouse button is released.
		 */
		bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID) override;

		/**
		 * Called when the window is resized.
		 */
		void windowResized(Ogre::RenderWindow* rw) override;

		/**
		 * Called when the window is closed.
		 */
		void windowClosed(Ogre::RenderWindow* rw) override;

	private:
		/**
		 * Init methods.
		 */
		void ogre_init();
		void ois_init();
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
		Ogre::Viewport* main_view_;
		Ogre::Light* main_light_;

		OIS::InputManager* input_;
		OIS::Keyboard* keyboard_;
		OIS::Mouse* mouse_;

		/**
		 * Camera rendering to the window.
		 */
		std::unique_ptr<Camera> main_cam_;

		/**
		 * Unique pointers to systems (sadly all systems require the EntitySystem, which
		 * requires Ogre::SceneManager, so the all have to be instantiated after the ogre_init method
		 * call and thus cannot be references).
		 */
		std::unique_ptr<EntitySystem> entity_system_{nullptr};
		std::unique_ptr<HealthSystem> health_system_{nullptr};
		std::unique_ptr<MovementSystem> movement_system_{nullptr};
		std::unique_ptr<AISystem> ai_system_{nullptr};
		std::unique_ptr<InputSystem> input_system_{nullptr};
		std::unique_ptr<GridSystem> grid_system_{nullptr};
		std::unique_ptr<TaskSystem> task_system_{nullptr};
		std::unique_ptr<CombatSystem> combat_system_{nullptr};
		std::unique_ptr<ProductionSystem> production_system_{nullptr};
		std::unique_ptr<TimeSystem> time_system_{nullptr};
		std::unique_ptr<EventSystem> event_system_{nullptr};
		std::unique_ptr<GraphicsSystem> graphics_system_{nullptr};
		std::unique_ptr<TriggerSystem> trigger_system_{nullptr};
		std::unique_ptr<ManaSpellSystem> mana_spell_system_{nullptr};
		std::unique_ptr<WaveSystem> wave_system_{nullptr};
		std::unique_ptr<AnimationSystem> animation_system_{nullptr};

		/**
		 * Used to save the game.
		 */
		std::unique_ptr<GameSerializer> game_serializer_{nullptr};

		/**
		 * Vector of all systems used for updating the game's logic.
		 */
		std::vector<System*> systems_{};

		/**
		 * CEGUI renderer.
		 */
		CEGUI::OgreRenderer* renderer_;

		/**
		 * \brief Converts an OIS key code to CEGUI key code.
		 * \param OIS key code.
		 */
		CEGUI::MouseButton ois_to_cegui(OIS::MouseButtonID);

		/**
		 * Allows to spawn entities with the mouse ingame.
		 */
		std::unique_ptr<EntityPlacer> placer_;

		/**
		 * The game's ground represented by a plane, used for ray intersections.
		 */
		std::unique_ptr<Ogre::Plane> ground_;

		/**
		 * Entity holding the ground plane. Used for easier plane switching.
		 */
		Ogre::Entity* ground_entity_;

		/**
		 * \brief Toggles the free camera movement mode.
		 */
		void toggle_camera_free_mode();

		/**
		 * \brief Returns a pair consisting of the location of the point where the player
		 *        clicked on the ground plane and a boolean indicating if the click was indeed
		 *        on the ground plane.
		 */
		std::pair<bool, Ogre::Vector3> get_mouse_click_position(const OIS::MouseEvent&) const;

		/**
		 * Selection box used to select multiple entities at once.
		 */
		std::unique_ptr<SelectionBox> selection_box_;

		/**
		 * A gui window that allows to place entities selected from a menu.
		 * (TODO: Allow to create/modify entities.)
		 */
		std::unique_ptr<EntityCreator> entity_creator_;

		/**
		 * Saved position of the mouse cursor (2D).
		 */
		Ogre::Vector2 mouse_position_;

		/**
		 * Used to create new levels (that is, to distribute walls and
		 * gold deposits).
		 */
		std::unique_ptr<level_generators::LevelGenerator> level_generator_;

		/**
		 * Allows the player to cast spells of different types (positional, targeted etc.).
		 */
		std::unique_ptr<Spellcaster> spell_caster_;

		/**
		 * ID of the entity representing the Dungeon Throne, losing which ends
		 * the game.
		 */
		tdt::uint throne_id_;

		/**
		 * Manages deferred shading.
		 */
		std::unique_ptr<DeferredShadingSystem> ds_system_;
};
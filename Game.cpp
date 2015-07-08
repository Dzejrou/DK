#include "Game.hpp"

Game::Game()
    : state_{GAME_STATE::RUNNING}, root_{nullptr}, window_{nullptr},
      scene_mgr_{nullptr}, main_cam_{nullptr}
{
    ogre_init();
}



void Game::run()
{
    point last_time = clock::now();
    point curr_time{};
    frame frame_time{};

    while(state_ != GAME_STATE::ENDED)
    {
        curr_time = clock::now();
        auto delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - last_time);

        while(delta_time >= frame_time)
        {
            // do the game stuff

            root_->renderOneFrame();
            //delta_time -= frame_time;
        }

        last_time = curr_time;
    }
}



void Game::ogre_init()
{
    Ogre::String plugs, res;
#ifdef _DEBUG
    plugs = "plugins_d.cfg";
    res = "resources_d.cfg";
#else
    plugs = "plugins.cfg";
    res = "resources.cfg";
#endif
    root_.reset(new Ogre::Root{plugs});

    Ogre::ConfigFile cf;
    cf.load(res);

    // Load resource settings.
    Ogre::String name, loc_type;
    auto selit = cf.getSectionIterator();
    while(selit.hasMoreElements())
    {
        Ogre::ConfigFile::SettingsMultiMap* settings = selit.getNext();
        for(auto& x : *settings)
        {
            loc_type = x.first;
            name = x.second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, loc_type);
        }
    }
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    if(!(root_->restoreConfig() || root_->showConfigDialog()))
    {   // Configuration read failed, end the game.
        state_ = GAME_STATE::ENDED;
        return;
    }
    else
        root_->initialise(false);

    // Window init.
    window_ = root_->createRenderWindow("Dungeon Keeper", 800, 600, false);
    window_->setVisible(true);

    // Scene init.
    // TODO: Research different types of scene managers!
    scene_mgr_ = root_->createSceneManager(Ogre::ST_GENERIC);
    main_cam_ = scene_mgr_->createCamera("MainCam");
    main_cam_->lookAt(0, 0, -300);
    main_cam_->setPosition(0, 0, 80);
    main_cam_->setNearClipDistance(5);
    main_view_ = window_->addViewport(main_cam_);
    main_cam_->setAspectRatio(Ogre::Real{main_view_->getActualWidth()} /
                              Ogre::Real{main_view_->getActualHeight()});
    main_light_ = scene_mgr_->createLight("MainLight");
    main_light_->setPosition(20, 80, 50);
}
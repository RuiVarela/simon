#pragma once

#include "Project.h"

namespace re
{
    class Menu
    {
    public:
        Menu();
        ~Menu();

        void clear();

        void addButton(std::string const& resource, std::function<void(void)> callback);
        void update();
        void render();

    private:
        struct MenuItem {
            std::shared_ptr<Button> button;
            std::function<void(void)> callback;
        };

        std::vector<MenuItem> m_items;
    };

}
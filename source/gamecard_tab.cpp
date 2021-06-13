/*
 * gamecard_tab.cpp
 *
 * Copyright (c) 2020-2021, DarkMatterCore <pabloacurielz@gmail.com>.
 *
 * This file is part of nxdumptool (https://github.com/DarkMatterCore/nxdumptool).
 *
 * nxdumptool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nxdumptool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <nxdt_includes.h>
#include <gamecard_tab.hpp>

namespace i18n = brls::i18n;    /* For getStr(). */
using namespace i18n::literals; /* For _i18n. */

namespace nxdt::views
{
    GameCardTab::GameCardTab(nxdt::tasks::GameCardTask *gc_status_task) : brls::LayerView(), gc_status_task(gc_status_task)
    {
        /* Add error frame. */
        this->error_frame = new ErrorFrame("gamecard_tab/error_frame/not_inserted"_i18n);
        this->addLayerWrapper(this->error_frame);
        
        /* Add list. */
        this->list = new brls::List();
        
        this->dump_card_image = new brls::ListItem("gamecard_tab/list/dump_card_image/label"_i18n, "gamecard_tab/list/dump_card_image/description"_i18n);
        this->list->addView(this->dump_card_image);
        
        this->dump_certificate = new brls::ListItem("gamecard_tab/list/dump_certificate/label"_i18n, "gamecard_tab/list/dump_certificate/description"_i18n);
        this->list->addView(this->dump_certificate);
        
        this->dump_header = new brls::ListItem("gamecard_tab/list/dump_header/label"_i18n, "gamecard_tab/list/dump_header/description"_i18n);
        this->list->addView(this->dump_header);
        
        this->dump_decrypted_cardinfo = new brls::ListItem("gamecard_tab/list/dump_decrypted_cardinfo/label"_i18n, "gamecard_tab/list/dump_decrypted_cardinfo/description"_i18n);
        this->list->addView(this->dump_decrypted_cardinfo);
        
        this->dump_initial_data = new brls::ListItem("gamecard_tab/list/dump_initial_data/label"_i18n, "gamecard_tab/list/dump_initial_data/description"_i18n);
        this->list->addView(this->dump_initial_data);
        
        this->dump_hfs_partitions = new brls::ListItem("gamecard_tab/list/dump_hfs_partitions/label"_i18n, "gamecard_tab/list/dump_hfs_partitions/description"_i18n);
        this->list->addView(this->dump_hfs_partitions);
        
        this->addLayerWrapper(this->list);
        
        /* Setup gamecard status task. */
        this->gc_status_task_sub = this->gc_status_task->RegisterListener([this](GameCardStatus gc_status) {
            if (gc_status < GameCardStatus_InsertedAndInfoLoaded) this->changeLayerWrapper(this->error_frame);
            
            switch(gc_status)
            {
                case GameCardStatus_NotInserted:
                    this->error_frame->SetMessage("gamecard_tab/error_frame/not_inserted"_i18n);
                    break;
                case GameCardStatus_Processing:
                    this->error_frame->SetMessage("gamecard_tab/error_frame/processing"_i18n);
                    break;
                case GameCardStatus_NoGameCardPatchEnabled:
                    this->error_frame->SetMessage("gamecard_tab/error_frame/nogc_enabled"_i18n);
                    break;
                case GameCardStatus_LotusAsicFirmwareUpdateRequired:
                    this->error_frame->SetMessage("gamecard_tab/error_frame/lafw_update_required"_i18n);
                    break;
                case GameCardStatus_InsertedAndInfoNotLoaded:
                    this->error_frame->SetMessage(i18n::getStr("gamecard_tab/error_frame/info_not_loaded"_i18n, GITHUB_NEW_ISSUE_URL));
                    break;
                case GameCardStatus_InsertedAndInfoLoaded:
                    this->changeLayerWrapper(this->list);
                    break;
                default:
                    break;
            }
            
            this->gc_status = gc_status;
        });
    }
    
    GameCardTab::~GameCardTab(void)
    {
        /* Unregister gamecard task listener. */
        this->gc_status_task->UnregisterListener(this->gc_status_task_sub);
        
        /* Clear views vector. */
        if (this->views.size()) this->views.clear();
    }
    
    void GameCardTab::addLayerWrapper(brls::View* view)
    {
        this->views.push_back(view);
        this->addLayer(view);
        if (this->view_index == -1) this->view_index = 0;
    }
    
    void GameCardTab::changeLayerWrapper(brls::View* view)
    {
        int index = -1;
        brls::View *current_focus = brls::Application::getCurrentFocus();
        
        for(size_t i = 0; i < this->views.size(); i++)
        {
            if (this->views[i] == view)
            {
                index = (int)i;
                break;
            }
        }
        
        if (index == -1) return;
        
        /* Focus the sidebar if we're currently focusing a ListItem element. */
        if (current_focus && (current_focus == this->dump_card_image || current_focus == this->dump_certificate || current_focus == this->dump_header || \
            current_focus == this->dump_decrypted_cardinfo || current_focus == this->dump_initial_data || current_focus == this->dump_hfs_partitions))
        {
            brls::Application::onGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, false);
        }
        
        this->changeLayer(index);
        this->invalidate(true);
        this->view_index = index;
    }
}

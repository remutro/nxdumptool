/*
 * gamecard_tab.hpp
 *
 * Copyright (c) 2020-2024, DarkMatterCore <pabloacurielz@gmail.com>.
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

#pragma once

#ifndef __GAMECARD_TAB_HPP__
#define __GAMECARD_TAB_HPP__

#include "root_view.hpp"
#include "layered_error_frame.hpp"
#include "focusable_item.hpp"

namespace nxdt::views
{
    class GameCardTab: public LayeredErrorFrame
    {
        private:
            typedef bool (*GameCardSizeFunc)(u64 *out_size);

            RootView *root_view = nullptr;

            nxdt::tasks::GameCardStatusEvent::Subscription gc_status_task_sub;
            GameCardStatus gc_status = GameCardStatus_NotInserted;

            std::string raw_filename_full = "";
            std::string raw_filename_id_only = "";

            void ProcessGameCardStatus(const GameCardStatus& gc_status);

            void PopulateList(void);
            void AddApplicationMetadataItems(void);
            void AddPropertiesTable(void);

            void GenerateRawFilenames(void);
            std::string GetFormattedSizeString(GameCardSizeFunc func);
            std::string GetCardIdSetString(const FsGameCardIdSet& card_id_set);

        public:
            GameCardTab(RootView *root_view);
            ~GameCardTab();
    };
}

#endif  /* __GAMECARD_TAB_HPP__ */

/**
* Copyright (c) 2011-2014 - Ashita Development Team
*
* Ashita is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Ashita is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Ashita.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ASHITA_Greed_H_INCLUDED__
#define __ASHITA_Greed_H_INCLUDED__

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/**
 * @brief Required includes for an extension.
 */
#include "..\..\ADK\Ashita.h"
#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_utils.hpp"
#include <map>
#include <stdint.h>

#define SPACING 3
#define PADDING 3
#define GreedSIZE 36
#define FONTSIZE 10
#define FADERANGE 400

// buffer I/O macros
#define RBUFP(p,pos) (((uint8_t*)(p)) + (pos))
#define RBUFB(p,pos) (*(uint8_t*)RBUFP((p),(pos)))
#define RBUFW(p,pos) (*(uint16_t*)RBUFP((p),(pos)))
#define RBUFL(p,pos) (*(uint32_t*)RBUFP((p),(pos)))
#define RBUFF(p,pos) (*(float*)RBUFP((p),(pos)))

#define WBUFP(p,pos) (((uint8_t*)(p)) + (pos))
#define WBUFB(p,pos) (*(uint8_t*)WBUFP((p),(pos)))
#define WBUFW(p,pos) (*(uint16_t*)WBUFP((p),(pos)))
#define WBUFL(p,pos) (*(uint32_t*)WBUFP((p),(pos)))
#define WBUFU(p,pos) (*(uint64_t*)WBUFP((p),(pos)))
#define WBUFF(p,pos) (*(float*)WBUFP((p),(pos)))

/**
 * @brief Global copy of our plugin data.
 */
PluginData* g_PluginData = NULL;

static std::vector<std::string> jobs = { "", "WAR", "MNK", "WHM", "BLM", "RDM", "THF", "PLD", "DRK",
    "BST", "BRD", "RNG", "SAM", "NIN", "DRG", "SMN", "BLU", "COR", "PUP", "DNC", "SCH", "GEO", "RUN" };

static const std::vector<D3DCOLOR> jobColors = { D3DCOLOR_ARGB(255, 120, 120, 120), D3DCOLOR_ARGB(255, 140, 0, 0), D3DCOLOR_ARGB(255, 255, 177, 32), 
                                              D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 68, 68, 68), D3DCOLOR_ARGB(255, 255, 0, 0), D3DCOLOR_ARGB(255, 0, 164, 49), 
                                              D3DCOLOR_ARGB(255, 198, 198, 0), D3DCOLOR_ARGB(255, 116, 0, 145), D3DCOLOR_ARGB(255, 165, 153, 10), D3DCOLOR_ARGB(255, 184, 128, 10),
                                              D3DCOLOR_ARGB(255, 224, 0, 230), D3DCOLOR_ARGB(255, 234, 100, 0), D3DCOLOR_ARGB(255, 119, 0, 0), D3DCOLOR_ARGB(255, 130, 17, 255),
                                              D3DCOLOR_ARGB(255, 79, 196, 0), D3DCOLOR_ARGB(255, 50, 66, 217), D3DCOLOR_ARGB(255, 136, 68, 0), D3DCOLOR_ARGB(255, 244, 98, 98),
                                              D3DCOLOR_ARGB(255, 15, 190, 220), D3DCOLOR_ARGB(255, 0, 123, 145), D3DCOLOR_ARGB(255, 255, 70, 255), D3DCOLOR_ARGB(255, 255, 255, 255) };

struct config_t
{
    uint8_t spacing;
    uint8_t padding;
    uint8_t Greedsize;
    uint8_t fontsize;
    float faderange;
    Ashita::Enums::FrameAnchor anchor;
    float x;
    float y;
    uint8_t rf;
    uint8_t gf;
    uint8_t bf;
    uint8_t rb;
    uint8_t gb;
    uint8_t bb;
    bool jobColored;
};

/**
 * @brief Our Main Plugin Class
 *
 * @note    The main class of your plugin MUST use PluginBase as a base class. This is the
 *          internal base class that Ashita uses to communicate with your plugin!
 */
class Greed : PluginBase
{
    /**
     * @brief Internal class variables.
     */
    IAshitaCore*        m_AshitaCore;
    DWORD               m_PluginId;
    IDirect3DDevice8*   m_Direct3DDevice;

private:
    rapidxml::xml_document<> settings;
    rapidxml::file<>* xmlFile;
    std::map<IFontObject*, uint32_t> targIdMap;
    void LoadSettings();
    config_t config;

public:
    /**
     * @brief Constructor and deconstructor.
     */
	Greed(void);
	virtual ~Greed(void);

    /**
     * @brief GetPluginData implementation.
     */
    PluginData GetPluginData(void);

    /**
     * @brief PluginBase virtual overrides.
     */
    bool Initialize(IAshitaCore* ashitaCore, DWORD dwPluginId);
    void Release(void);
    bool HandleCommand(const char* pszCommand, int nCommandType);
    bool HandleNewChatLine(short sMode, char* pszChatLine);
    bool HandleIncomingPacket(unsigned int uiPacketId, unsigned int uiPacketSize, void* lpRawData);
    bool HandleOutgoingPacket(unsigned int uiPacketId, unsigned int uiPacketSize, void* lpRawData);
    bool Direct3DInitialize(IDirect3DDevice8* lpDevice);
    void Direct3DRelease(void);
    void Direct3DPreRender(void);
    void Direct3DRender(void);
    void onClick(int, IFontObject*, float, float);
};

Greed* g_Greed = NULL;

/**
 * @brief Required Plugin Exports
 */
__declspec(dllexport) double     __stdcall GetInterfaceVersion(void);
__declspec(dllexport) void       __stdcall CreatePluginData(PluginData* lpBuffer);
__declspec(dllexport) IPlugin*   __stdcall CreatePlugin(char* pszUnused);

#endif // __ASHITA_Greed_H_INCLUDED__

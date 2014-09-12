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

#include "Greed.h"
#include <algorithm>

#undef max
#undef min

D3DCOLOR darken(D3DCOLOR color)
{
    return D3DCOLOR_ARGB(((color & 0xFF000000) >> 24), ((color & 0xFF0000) >> 16) / 3, ((color & 0xFF00) >> 8) / 3, (color & 0xFF) / 3);
}

D3DCOLOR brighten(D3DCOLOR color)
{
    uint8_t alpha = (color & 0xFF000000) >> 24;
    uint8_t r = (color & 0xFF0000) >> 16;
    uint8_t g = (color & 0xFF00) >> 8;
    uint8_t b = (color & 0xFF);
    return D3DCOLOR_ARGB(alpha, r > 225 ? 255 : r + 30, g > 225 ? 255 : g + 30, b > 225 ? 255 : b + 30);
}

/**
 * @brief Constructor and Deconstructor
 */
Greed::Greed(void)
: m_AshitaCore(NULL)
, m_PluginId(0)
, m_Direct3DDevice(NULL)
{ }
Greed::~Greed(void)
{ }

/**
 * @brief Obtains the plugin data for this plugin.
 *
 * @return The PluginData structure for this plugin.
 */
PluginData Greed::GetPluginData(void)
{
    return (*g_PluginData);
}

/**
 * @brief Initializes our plugin. This is the main call that happens when your plugin is loaded.
 *
 * @param ashitaCore        The main Ashita Core object interface to interact with Ashita.
 * @param scriptEngine      The main script engine object interface to interact with the script engine.
 * @param dwPluginId        The base address of your plugin. This is used as the ID.
 *
 * @return True on success, false otherwise.
 *
 * @note If your plugin returns false here, it will be unloaded immediately!
 */
bool Greed::Initialize(IAshitaCore* ashitaCore, DWORD dwPluginId)
{
    // Store the variables we are passed..
    this->m_AshitaCore = ashitaCore;
    this->m_PluginId = dwPluginId;

    g_Greed = this;

    LoadSettings();

    return true;
}

/**
 * @brief Releases this plugin. This is called when your plugin is unloaded.
 *
 * @note Your plugin should cleanup all its data here before it unloads. Anything such as:
 *          - Font objects.
 *          - Gui objects.
 *          - Bindings to the script engine (if you extended it any).
 */
void Greed::Release(void)
{

}

/**
 * @brief Allows a plugin to attempt to handle a game command.
 *
 * @param pszCommand            The command being processed.
 * @param nCommandType          The type of command being processed.
 *
 * @return True on handled, false otherwise.
 */
bool Greed::HandleCommand(const char* pszCommand, int nCommandType)
{
    return false;
}

/**
 * @brief Allows a plugin to attempt to handle a new chat line.
 *
 * @param sMode                 The chat type being added.
 * @param pszChatLine           The chat line being added.
 *
 * @return True on handled, false otherwise.
 */
bool Greed::HandleNewChatLine(short sMode, char* pszChatLine)
{
    return false;
}

/**
 * @brief Allows a plugin to attempt to handle an incoming packet.
 *
 * @param uiPacketId            The id of the packet.
 * @param uiPacketSize          The size of the packet.
 * @param lpRawData             The raw packet data.
 *
 * @return True on handled, false otherwise.
 *
 * @note    Returning true on this will block the packet from being handled! This can
 *          have undesired effects! Use with caution as this can get you banned!
 */
bool Greed::HandleIncomingPacket(unsigned int uiPacketId, unsigned int uiPacketSize, void* lpRawData)
{
    return false;
}

/**
 * @brief Allows a plugin to attempt to handle an outgoing packet.
 *
 * @param uiPacketId            The id of the packet.
 * @param uiPacketSize          The size of the packet.
 * @param lpRawData             The raw packet data.
 *
 * @return True on handled, false otherwise.
 *
 * @note    Returning true on this will block the packet from being handled! This can
 *          have undesired effects! Use with caution as this can get you banned!
 */
bool Greed::HandleOutgoingPacket(unsigned int uiPacketId, unsigned int uiPacketSize, void* lpRawData)
{
    return false;
}

/**
 * @brief Direct3D initialize call to prepare this plugin for Direct3D calls.
 *
 * @param lpDevice              The Direct3D device currently wrapped by Ashita.
 *
 * @return True on success, false otherwise.
 *
 * @note    Plugins that do not return true on this call will not receive any other
 *          Direct3D calls listed below!
 */
bool Greed::Direct3DInitialize(IDirect3DDevice8* lpDevice)
{
    this->m_Direct3DDevice = lpDevice;

    IFontObject* base = m_AshitaCore->GetFontManager()->CreateFontObject("GreedBase");
    base->SetFont("Consolas", 10);
    base->SetAutoResize(false);
    base->GetBackground()->SetColor(D3DCOLOR_ARGB(0x60, 0x00, 0x00, 0x00));
    base->GetBackground()->SetVisibility(true);
    base->GetBackground()->SetWidth(308);
    base->GetBackground()->SetHeight(config.Greedsize + config.spacing * 2);
    base->SetAnchor(config.anchor);
    base->SetAnchorParent(config.anchor);
    base->SetText("");
    base->SetPosition(config.x, config.y);
    base->SetVisibility(true);

    IFontObject* prev = base;
    IFontObject* rowBase = NULL;

    for (int i = 0; i < 18; i++)
    {
        std::string texPath = m_AshitaCore->GetAshitaInstallPathA();
        texPath.append("\\Resources\\Greed\\gradient.tga");

        char alias[32];
        sprintf_s(alias, sizeof alias, "GreedBG%d", i);
        IFontObject* bg = m_AshitaCore->GetFontManager()->CreateFontObject(alias);

        bg->SetAutoResize(false);
        bg->GetBackground()->SetColor(D3DCOLOR_ARGB(0xFF, config.rb, config.gb, config.bb));
        bg->GetBackground()->SetWidth(config.Greedsize);
        bg->GetBackground()->SetHeight(config.Greedsize);
        bg->GetBackground()->SetVisibility(true);
        bg->SetParent(prev);
        bg->SetAnchor(config.anchor);

        auto right = config.anchor & Ashita::Enums::Right;
        auto bottom = config.anchor & Ashita::Enums::Bottom;

        if (prev == base)
        {
            bg->SetAnchorParent(config.anchor);
            bg->SetPosition(right ? -config.spacing : config.spacing, bottom ? 
                -config.spacing - (config.Greedsize + config.padding) * (i / 6) :
                config.spacing + (config.Greedsize + config.padding) * (i / 6));
            rowBase = bg;
        }
        else if (i == 6 || i == 12)
        {
            bg->SetParent(rowBase);
            bg->SetAnchorParent(static_cast<Ashita::Enums::FrameAnchor>(config.anchor ^ Ashita::Enums::Bottom));
            bg->SetPosition(0, bottom ? -config.padding : config.padding);
            rowBase = bg;
        }
        else
        {
            bg->SetAnchorParent(static_cast<Ashita::Enums::FrameAnchor>(config.anchor ^ Ashita::Enums::Right));
            bg->SetPosition(right ? -config.padding : config.padding, 0);
        }
        bg->SetVisibility(true);
        bg->SetClickFunction(g_onClick);
        bg->GetBackground()->SetTextureFromFile(texPath.c_str());

        sprintf_s(alias, sizeof alias, "GreedFG%d", i);
        IFontObject* fg = m_AshitaCore->GetFontManager()->CreateFontObject(alias);

        fg->SetAutoResize(false);
        fg->GetBackground()->SetColor(D3DCOLOR_ARGB(0xFF, config.rf, config.gf, config.bf));
        fg->GetBackground()->SetWidth(config.Greedsize);
        fg->GetBackground()->SetHeight(config.Greedsize);
        fg->GetBackground()->SetVisibility(true);
        fg->SetParent(bg);
        fg->SetAnchor(Ashita::Enums::BottomLeft);
        fg->SetAnchorParent(Ashita::Enums::BottomLeft);
        fg->SetPosition(0, 0);
        fg->SetVisibility(true);
        fg->GetBackground()->SetTextureFromFile(texPath.c_str());

        sprintf_s(alias, sizeof alias, "GreedTX%d", i);
        IFontObject* tx = m_AshitaCore->GetFontManager()->CreateFontObject(alias);

        tx->SetAutoResize(false);
        tx->SetFont("Consolas", config.fontsize);
        tx->SetText("");
        tx->SetColor(D3DCOLOR_ARGB(0xFF, config.rb, config.gb, config.bb));
        tx->GetBackground()->SetVisibility(false);
        tx->SetParent(bg);
        tx->SetAnchor(Ashita::Enums::BottomLeft);
        tx->SetAnchorParent(Ashita::Enums::BottomLeft);
        tx->SetPosition(config.padding, -config.Greedsize / 2 - (5 * config.fontsize) / 6);
        tx->SetVisibility(true);

        sprintf_s(alias, sizeof alias, "GreedIndicatorTopLeft%d", i);
        IFontObject* br = m_AshitaCore->GetFontManager()->CreateFontObject(alias);

        br->SetAutoResize(false);
        br->GetBackground()->SetColor(D3DCOLOR_ARGB(0xFF, 0x80, 0x80, 0x00));
        br->GetBackground()->SetWidth(config.Greedsize / 6);
        br->GetBackground()->SetHeight(config.Greedsize / 6);
        br->GetBackground()->SetVisibility(true);
        br->SetParent(bg);
        br->SetAnchor(Ashita::Enums::TopLeft);
        br->SetAnchorParent(Ashita::Enums::TopLeft);
        br->SetPosition(0,0);
        br->SetVisibility(false);

        prev = bg;
    }

    return true;
}

/**
 * @brief Direct3D release call to allow this plugin to cleanup any Direct3D objects.
 */
void Greed::Direct3DRelease(void)
{
    m_AshitaCore->GetFontManager()->DeleteFontObject("GreedBase");

    for (int i = 0; i < 18; i++)
    {
        char alias[32];
        sprintf_s(alias, sizeof alias, "GreedBG%d", i);
        m_AshitaCore->GetFontManager()->DeleteFontObject(alias);

        sprintf_s(alias, sizeof alias, "GreedFG%d", i);
        m_AshitaCore->GetFontManager()->DeleteFontObject(alias);

        sprintf_s(alias, sizeof alias, "GreedTX%d", i);
        m_AshitaCore->GetFontManager()->DeleteFontObject(alias);

        sprintf_s(alias, sizeof alias, "GreedIndicatorTopLeft%d", i);
        m_AshitaCore->GetFontManager()->DeleteFontObject(alias);
    }
}

/**
 * @brief Direct3D prerender call to allow this plugin to prepare for rendering.
 *
 * @note This will only be called if you returned true in Direct3DInitialize!
 */
void Greed::Direct3DPreRender(void)
{
}

/**
 * @brief Direct3D render call to allow this plugin to render any custom things.
 *
 * @note This will only be called if you returned true in Direct3DInitialize!
 */
void Greed::Direct3DRender(void)
{
    int party = 0;
    for (int i = 0; i < 18; i++)
    {
        char alias[32];
        sprintf_s(alias, sizeof alias, "GreedBG%d", i);
        IFontObject* bg = m_AshitaCore->GetFontManager()->GetFontObject(alias);
        sprintf_s(alias, sizeof alias, "GreedFG%d", i);
        IFontObject* fg = m_AshitaCore->GetFontManager()->GetFontObject(alias);
        sprintf_s(alias, sizeof alias, "GreedTX%d", i);
        IFontObject* tx = m_AshitaCore->GetFontManager()->GetFontObject(alias);
        sprintf_s(alias, sizeof alias, "GreedIndicatorTopLeft%d", i);
        IFontObject* br = m_AshitaCore->GetFontManager()->GetFontObject(alias);

        if (!m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberActive(i))
        {
            bg->SetVisibility(false);
            fg->SetVisibility(false);
            tx->SetVisibility(false);
            br->SetVisibility(false);
            targIdMap[bg] = 0;
        }
        else
        {
            bg->SetVisibility(true);
            fg->SetVisibility(true);
            tx->SetVisibility(true);
            uint8_t hpp = m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberHPP(i);
            uint8_t job = m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberMainJob(i);
            uint16_t index = m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberTargetIndex(i);
            if (config.jobColored && job <= Ashita::Enums::RuneFencer)
            {
                bg->GetBackground()->SetColor(jobColors[job]);
                fg->GetBackground()->SetColor(darken(jobColors[job]));
                tx->SetColor(brighten(jobColors[job]));
            }
            fg->GetBackground()->SetHeight(bg->GetBackground()->GetHeight() * ((float)hpp / 100));
            fg->SetAnchor(Ashita::Enums::BottomLeft);
            std::string txString;
            char rawString[16];
            if (hpp > 90 || m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberZone(i) != m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberZone(0))
            {
                sprintf_s(rawString, sizeof rawString, "%.*s", (config.Greedsize / config.fontsize) + 1, m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberName(i));
                txString.append(rawString);
            }
            else if (hpp == 0)
            {
                txString.append("DEAD");
            }
            else
            {
                uint32_t hp = m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberHP(i);
                float hpguess = (int)(hp / ((float)hpp / 100)) - hp;
                sprintf_s(rawString, sizeof rawString, "-%.*f", hpguess > 1000 ? 1 : 0, hpguess > 1000 ? hpguess / 1000 : hpguess);
                txString.append(rawString);
            }
            tx->SetText(txString.c_str());
            if (m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberZone(i) != m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberZone(0) ||
                m_AshitaCore->GetDataManager()->GetEntity()->GetDistance(index) > config.faderange)
            {
                bg->GetBackground()->SetColor((bg->GetBackground()->GetColor() & 0x00FFFFFF) | (0x10 << 24));
                fg->GetBackground()->SetColor((fg->GetBackground()->GetColor() & 0x00FFFFFF) | (0x40 << 24));
                tx->SetColor((tx->GetColor() & 0x00FFFFFF) | (0x40 << 24));
            }
            else
            {
                bg->GetBackground()->SetColor((bg->GetBackground()->GetColor() & 0x00FFFFFF) | (0xFF << 24));
                fg->GetBackground()->SetColor((fg->GetBackground()->GetColor() & 0x00FFFFFF) | (0xFF << 24));
                tx->SetColor((tx->GetColor() & 0x00FFFFFF) | (0xFF << 24));
            }
            if (m_AshitaCore->GetDataManager()->GetTarget()->GetTargetIndex() == index && 
                m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberZone(i) == m_AshitaCore->GetDataManager()->GetParty()->GetPartyMemberZone(0))
            {
                br->SetVisibility(true);
            }
            else
            {
                br->SetVisibility(false);
            }
            targIdMap[bg] = m_AshitaCore->GetDataManager()->GetEntity()->GetServerID(index);
            party |= (i / 6);
        }
    }
    IFontObject* base = m_AshitaCore->GetFontManager()->GetFontObject("GreedBase");
    int maxmem = std::max(m_AshitaCore->GetDataManager()->GetParty()->GetAllianceParty0Count(), std::max(m_AshitaCore->GetDataManager()->GetParty()->GetAllianceParty1Count(),
        m_AshitaCore->GetDataManager()->GetParty()->GetAllianceParty2Count()));
    base->GetBackground()->SetWidth(config.spacing * 2 + maxmem * config.Greedsize + (maxmem - 1) * config.padding);
    base->GetBackground()->SetHeight(config.Greedsize + config.spacing * 2 + (party & 1 ? config.Greedsize + config.padding : 0) +
        (party & 2 ? config.Greedsize + config.padding : 0));
    base->SetAnchor(config.anchor);
}

void Greed::LoadSettings()
{
    config.faderange = FADERANGE;
    config.fontsize = FONTSIZE;
    config.Greedsize = GreedSIZE;
    config.padding = PADDING;
    config.spacing = SPACING;
    config.anchor = Ashita::Enums::BottomRight;
    config.x = -132;
    config.y = -16;
    config.rf = 0x50;
    config.gf = 0x28;
    config.bf = 0x00;
    config.rb = 0xFF;
    config.gb = 0x80;
    config.bb = 0x00;
    config.jobColored = true;

    char szConfigPath[MAX_PATH] = { 0 };
    sprintf_s(szConfigPath, MAX_PATH, "%sConfig\\%s.xml", m_AshitaCore->GetAshitaInstallPathA(), g_PluginData->Name);
    try
    {
        xmlFile = new rapidxml::file<>(szConfigPath);
        settings.parse<0>(xmlFile->data());
    }
    catch (std::runtime_error& e)
    {
        m_AshitaCore->GetChatManager()->Write("Greed: Could not load settings: %s", e.what());
        return;
    }

    auto node = settings.first_node("settings");

    if (node)
    {
        auto setNode = node->first_node("faderange");
        if (setNode)
            config.faderange = pow(atoi(setNode->value()),2);

        setNode = node->first_node("fontsize");
        if (setNode)
            config.fontsize = atoi(setNode->value());

        setNode = node->first_node("Greedsize");
        if (setNode)
            config.Greedsize = atoi(setNode->value());

        setNode = node->first_node("padding");
        if (setNode)
            config.padding = atoi(setNode->value());

        setNode = node->first_node("spacing");
        if (setNode)
            config.spacing = atoi(setNode->value());

        setNode = node->first_node("anchor");
        if (setNode)
            config.anchor = static_cast<Ashita::Enums::FrameAnchor>(atoi(setNode->value()));

        setNode = node->first_node("xpos");
        if (setNode)
            config.x = atoi(setNode->value());

        setNode = node->first_node("ypos");
        if (setNode)
            config.y = atoi(setNode->value());

        setNode = node->first_node("fgred");
        if (setNode)
            config.rf = atoi(setNode->value());

        setNode = node->first_node("fggreen");
        if (setNode)
            config.gf = atoi(setNode->value());

        setNode = node->first_node("fgblue");
        if (setNode)
            config.bf = atoi(setNode->value());

        setNode = node->first_node("bgred");
        if (setNode)
            config.rb = atoi(setNode->value());

        setNode = node->first_node("bggreen");
        if (setNode)
            config.gb = atoi(setNode->value());

        setNode = node->first_node("bgblue");
        if (setNode)
            config.bb = atoi(setNode->value());

        setNode = node->first_node("jobcolored");
        if (setNode)
            config.jobColored = strncmp(setNode->value(),"true",setNode->value_size()) ? false : true;
    }
}

/**
 * @brief Gets the interface version this plugin was compiled with.
 *
 * @note This is a required export, your plugin must implement this!
 */
__declspec(dllexport) double __stdcall GetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}

/**
 * @brief Gets the plugin data for this plugin.
 *
 * @note This is a required export, your plugin must implement this!
 */
__declspec(dllexport) void __stdcall CreatePluginData(PluginData* lpBuffer)
{
    g_PluginData = lpBuffer;

    strcpy_s(g_PluginData->Name, sizeof(g_PluginData->Name), "Greed");
    strcpy_s(g_PluginData->Author, sizeof(g_PluginData->Author), "kjLotus");

    g_PluginData->InterfaceVersion = ASHITA_INTERFACE_VERSION;
    g_PluginData->PluginVersion = 1.00f;
    g_PluginData->Priority = 0;
}

/**
 * @brief Creates an instance of this plugin object.
 *
 * @note This is a required export, your plugin must implement this!
 */
__declspec(dllexport) IPlugin* __stdcall CreatePlugin(char* pszReserved)
{
    UNREFERENCED_PARAMETER(pszReserved);
	return (IPlugin*)new Greed();
}

void Greed::onClick(int type, IFontObject* font, float xPos, float yPos)
{
    if (xmlFile)
    {
        std::string job = jobs[m_AshitaCore->GetDataManager()->GetPlayer()->GetMainJob()];
        std::transform(job.begin(), job.end(), job.begin(), ::tolower);
        auto node = settings.first_node(job.c_str());
        uint32_t targid;
        try
        {
            targid = targIdMap.at(font);
        }
        catch (const std::out_of_range& e)
        {
            return;
        }

        if (node)
        {
            std::string click;
            if (type == Ashita::Enums::LeftClick)
                click.append("lclick");
            else if (type == Ashita::Enums::RightClick)
                click.append("rclick");
            else if (type == Ashita::Enums::MiddleClick)
                click.append("mclick");
            else if (type == Ashita::Enums::X1Click)
                click.append("button4");
            else if (type == Ashita::Enums::X2Click)
                click.append("button5");
            else
                return;

            for (auto clickNode = node->first_node(click.c_str()); clickNode; clickNode = clickNode->next_sibling(click.c_str()))
            {
                uint8_t modifiers = (GetAsyncKeyState(VK_CONTROL) & 0x8000 ? 1 : 0);
                modifiers |= (GetAsyncKeyState(VK_MENU) & 0x8000 ? 2 : 0);

                uint8_t foundModifiers = 0;
                auto attr = clickNode->first_attribute("modifier");
                for (auto attr = clickNode->first_attribute("modifier"); attr; attr = attr->next_attribute("modifier"))
                {
                    if (strcmp(attr->value(), "ctrl") == 0)
                        foundModifiers |= 1;
                    else if (strcmp(attr->value(), "alt") == 0)
                        foundModifiers |= 2;
                }
                if (modifiers == foundModifiers)
                {
                    std::string action = clickNode->value();

                    char cmd[64];
                    sprintf_s(cmd, sizeof cmd, "%s %d", action.c_str(), targid);
                    m_AshitaCore->GetChatManager()->QueueCommand(cmd, Ashita::Enums::Menu);
                    return;
                }
            }
        }
        else
        {
            //no config for this job, default left click: target
            if (type == 0 && !(GetKeyState(VK_CONTROL) & 0x8000) && !(GetKeyState(VK_MENU) & 0x8000))
            {
                char cmd[64];
                sprintf_s(cmd, sizeof cmd, "/target %d", targid);
                m_AshitaCore->GetChatManager()->QueueCommand(cmd, Ashita::Enums::Menu);
            }
        }
    }
}

void g_onClick(int type, void* font, float xPos, float yPos)
{
    g_Greed->onClick(type, (IFontObject*)font, xPos, yPos);
}
/*
    LightBaker3000 Frontend project,
    (c) 2022-2023 CrazyRussian
*/

#include "application.h"
#include "../tiny-process-library/process.hpp"
#include "properties_editor.h"
#include <SDL2/SDL_thread.h>
#include <functional>
#include <thread>

#include "lb3k_wrapper.h"
#include <nlohmann/json.hpp>
#include <regex>

#ifndef LINUX
#include <corecrt_malloc.h>
#endif

#include "hammer_fgd.h"
#include "r_editor_grid.h"
#include "secondary_window.h"
#include "viewports_orchestrator.h"

Application::Application()
{
    m_pCommandsRegistry      = new CCommandsRegistry;
    m_pFileSystem            = new FileSystem;
    m_pPersistentStorage     = nullptr;
    m_pLightBakerApplication = new LightBaker3000("lb3k/LightBaker3000.exe");
}

int Application::SuggestMonitorForNewWindow()
{
    int nMonitors = SDL_GetNumVideoDisplays();

    if (nMonitors > 1)
    {
        int *monitorsUsage = new int[nMonitors];
        memset(monitorsUsage, 0, sizeof(int) * nMonitors);

        for (auto &it : m_lstWindows)
        {
            int index = SDL_GetWindowDisplayIndex(it->SDLHandle());
            monitorsUsage[index]++;
        }

        int leastCluttered = 0;
        int leastWindows   = 99999;

        for (int i = 0; i < nMonitors; i++)
        {
            if (monitorsUsage[i] < leastWindows)
            {
                leastCluttered = i;
                leastWindows   = monitorsUsage[i];
            }
        }

        delete[] monitorsUsage;

        return leastCluttered;
    }

    return 0;
}

IPlatformWindow *Application::FindWindowBySDLId(size_t sdlid)
{
    for (auto &it : m_lstWindows)
        if (it->GetId() == sdlid)
            return it;

    return nullptr;
}

//#include <windows.h>

void Application::SetupEventsRedirection(bool enabled, IPlatformWindow *targetWindow)
{
    m_bEventsRedirectionEnabled = enabled;
    m_pEventsRedirectionTarget  = targetWindow;

    if (!enabled && targetWindow)
    {
        // TODO: save pointer position before enabling redirection
        // and returning it back there?

        SDL_Window *pWindow     = targetWindow->SDLHandle();
        glm::ivec2  centerPoint = targetWindow->CenterPointGlobal();

        SDL_WarpMouseGlobal((int)centerPoint.x, (int)centerPoint.y);
    }
}

Application::~Application()
{
    // if (m_pMainWindow) delete m_pMainWindow;

    for (auto &it : m_lstWindows)
        delete it;

    m_lstWindows.clear();

    delete m_pFileSystem;

    delete m_pCommandsRegistry;
    delete m_pPersistentStorage;
    delete m_pLightBakerApplication;
    delete GameConfigurationsManager::Instance();
}

#include <sdl-event-to-string\sdl_event_to_string.h>

void Application::Run()
{    
    CProfileManager::Reset();

    bool loop = false;

    do
    {
        m_lstWindows.remove_if([](IPlatformWindow *wind) {
            if (wind->IsTerminated())
            {
                // TODO: review when will be optimizing FBO usage
                ViewportsOrchestrator::Instance()->DestroyWindowViewports(wind);
                delete wind;
                return true;
            }
            return false;
        });

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            IPlatformWindow *pTarget = nullptr;

            if (m_bEventsRedirectionEnabled && m_pEventsRedirectionTarget != nullptr)
            {
                pTarget = m_pEventsRedirectionTarget;
            }
            else
            {
                // Input events are sent to window hovered by mouse
                switch (event.type)
                {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                    for (auto &it : m_lstWindows)
                    {
                        if (it->HasMouseInside())
                        {
                            pTarget = it;
                            break;
                        }
                    }
                    break;
                case SDL_DROPFILE:
                    pTarget = m_pMainWindow;
                    break;

                default:
                    // TODO: windowId not present in all event types, some events should be handled by application class
                    pTarget = FindWindowBySDLId(event.window.windowID);
                    break;
                }
            }

            if (pTarget && !pTarget->IsTerminated())
            {
                std::string str = sdlEventToString(event);

                //Con_Printf("%s -> %s\n", str.c_str(), pTarget->GetDescription());

                bool bResult = pTarget->HandleEvent(event);

                if (!bResult)
                    pTarget->SetTerminated(true);
                else
                    loop = true;
            }
        }

        if (!loop)
            break;

        for (auto &it : m_lstWindows)
        {
            it->UpdateCursorVisibility(m_bMouseCursorVisible);

            if (!it->IsTerminated())
                it->IterateUpdate();
        }

        CProfileManager::Increment_Frame_Counter();

    } while (loop && !m_bTerminated);
}

void Application::InitMainWindow()
{
    m_pPersistentStorage = PersistentStorage::Instance();
    m_pMainWindow        = new MainWindow("LightBaker3000 FrontEnd", glm::vec2(1280, 720));
    m_pMainWindow->InitStuff();

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::OpenNewWindow, "Open new window", nullptr, nullptr, 0, [&]() 
            {
            int displayIndex = SuggestMonitorForNewWindow();

            static size_t    m_counter  = 1;
            SecondaryWindow *pNewWindow = new SecondaryWindow(std::format("New window {0}", m_counter++), displayIndex);

            m_lstWindows.push_back(pNewWindow);
        }));

    m_lstWindows.push_back(m_pMainWindow);
}

void Application::Init(std::string cmdLine)
{
    m_strFileToLoad = cmdLine;
}

MainWindow *Application::GetMainWindow()
{
    return Instance()->m_pMainWindow;
}

FileSystem *Application::GetFileSystem()
{
    return Instance()->m_pFileSystem;
}

void Application::EPICFAIL(const char *format, ...)
{
    char tmp[4096];

    va_list argp;
    va_start(argp, format);
    vsprintf_s(tmp, sizeof(tmp), format, argp);
    va_end(argp);

    SDL_Window *window = nullptr;

    if (Application::GetMainWindow())
        window = Application::GetMainWindow()->Handle();

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "EPIC FAIL", tmp, window);
    exit(1);
}

DebugConsole *Application::Console()
{
    return Instance()->GetMainWindow()->Console();
}

void Application::ExecuteBaking()
{
    auto sceneRenderer = m_pMainWindow->GetSceneRenderer();
    auto scene         = sceneRenderer->GetScene();

    auto inst     = Instance();
    auto settings = inst->GetLightBakerApplication()->Settings();

    std::string lmMeshFile = scene->ExportForCompiling(nullptr, settings);

    if (!lmMeshFile.empty())
    {
        m_flBakingPercentage        = 0;
        m_bBakingFinished           = false;
        m_bWaitingForBakingToFinish = true;
        m_pLightBakerApplication->ExecuteBaking(lmMeshFile.c_str());
    }
}

void Application::CheckIfBakngFinished()
{
    if (!m_bWaitingForBakingToFinish && m_bBakingFinished)
        return;

    if (!m_bBakingFinished)
        return;

    m_bWaitingForBakingToFinish = false;

    if (m_bDoBakingAgain)
    {
        m_bDoBakingAgain = false;
        ExecuteBaking();
        Con_Printf("[FRONTEND] Need to rebake scene");
    }
    else
    {
        Con_Printf("[FRONTEND] Baking finished");
        m_pMainWindow->GetSceneRenderer()->ReloadScene(LRF_RELOAD_LIGHTMAPS);
    }

    // m_pMainWindow->GetSceneRenderer()->ReloadModel();
}

void Application::NotifyBakingFinished(int code)
{
    m_bBakingFinished = true;
}

PersistentStorage *Application::GetPersistentStorage()
{
    return Instance()->m_pPersistentStorage;
}

bool Application::DelayedInitDone()
{
    return Instance()->m_bDelayedInitDone;
}

void Application::FlagDelayedInitDone()
{
    m_bDelayedInitDone = true;

    if (!m_strFileToLoad.empty())
        Application::GetMainWindow()->GetSceneRenderer()->LoadModel(m_strFileToLoad.c_str(), LRF_LOAD_ALL);
}

LightBaker3000 *Application::GetLightBakerApplication()
{
    return m_pLightBakerApplication;
}

void Application::ScheduleCompilationIfNecceseary()
{
    auto inst = Instance();

    if (!inst->GetPersistentStorage()->GetSettingBool(ApplicationSettings::RebakeSceneAfterChanges))
        return;

    if (!Application::IsWaitingForBakerToFinish())
    {
        auto settings   = inst->GetLightBakerApplication()->Settings();
        int  oldSize[2] = {settings->m_lmSettings.size[0], settings->m_lmSettings.size[1]};
        int  oldSamples = settings->m_iSamples;

        settings->m_iSamples           = 64;
        settings->m_lmSettings.size[0] = 64;
        settings->m_lmSettings.size[1] = 64;

        inst->ExecuteBaking();

        settings->m_iSamples           = oldSamples;
        settings->m_lmSettings.size[0] = oldSize[0];
        settings->m_lmSettings.size[1] = oldSize[1];
    }
    else
        FlagToDoBakingAgain();
}

bool Application::IsWaitingForBakerToFinish()
{
    return Instance()->m_bWaitingForBakingToFinish;
}

void Application::FlagToDoBakingAgain()
{
    Instance()->m_bDoBakingAgain = true;
}

void Application::ParseLightBakerProgressMessage(std::string &captured)
{
    std::regex word_regex("(.+), Elapsed:(.+), Remaining:(.+)");
    auto       words_begin = std::sregex_iterator(captured.begin(), captured.end(), word_regex);
    auto       words_end   = std::sregex_iterator();

    auto dist = std::distance(words_begin, words_end);

    if (dist != 1)
        return;

    auto iterator = words_begin;

    std::smatch match    = *iterator;
    m_flBakingPercentage = std::stof(match[1].str());
    iterator++;

    std::string elapsed  = match[2].str();
    std::string remained = match[3].str();
}

void Application::ShowMouseCursor()
{    
    m_bMouseCursorVisible = true;
}

void Application::HideMouseCursor()
{
    m_bMouseCursorVisible = false;    
}

const char *date     = __DATE__;
const char *mon[12]  = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char  mond[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Greetings to:
//  Xash3D-FWGS team,
//  Uncle Mike,
//  and Id Software
// returns days since Sep 6 2022
int Application::Q_buildnum(void)
{
    int        m = 0, d = 0, y = 0;
    static int b = 0;

    if (b != 0)
        return b;

    for (m = 0; m < 11; m++)
    {
        if (!strncasecmp(&date[0], mon[m], 3))
            break;
        d += mond[m];
    }

    d += atoi(&date[4]) - 1;
    y = atoi(&date[7]) - 1900;
    b = d + (int)((y - 1) * 365.25f);

    if (((y % 4) == 0) && m > 1)
    {
        b += 1;
    }

    b -= 44443; // Sep 06 2022

    return b;
}

IPlatformWindow *Application::FindPlatformWindow(size_t id)
{
    // TODO: implement
    return Instance()->m_pMainWindow;
}

void Application::Terminate()
{
    m_bTerminated = true;
}

bool Application::IsMouseCursorVisible()
{
    return m_bMouseCursorVisible;
}

float Application::GetBakingProgress()
{
    return m_flBakingPercentage;
}

std::string &Application::BakingStatusMessage()
{
    return m_strBakingStatus;
}

Application *Application::Instance()
{
    static Application *sInstance = new Application;
    return sInstance;
}

CCommandsRegistry *Application::CommandsRegistry()
{
    return Instance()->m_pCommandsRegistry;
}

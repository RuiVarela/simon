#include "Project.h"

#define MESSAGES_TIME 1000

using namespace re;

#define MINT_CREAM \
    CLITERAL(Color) { 240, 247, 238, 255 }
#define BEAU_BLUE \
    CLITERAL(Color) { 196, 215, 242, 255 }
#define POWDER_BLUE \
    CLITERAL(Color) { 175, 222, 220, 255 }
#define MORNING_BLUE \
    CLITERAL(Color) { 145, 168, 164, 255 }
#define OLD_LAVANDER \
    CLITERAL(Color) { 119, 104, 113, 255 }

enum GameState
{
    PlayBackCells,
    PlayBackCell,
    WaitPlayBackCellEnd,

    InformPlayerShouldStart,
    WaitForInput,

    WaitPlayBackStart,

    ShowHome,
    ShowPause,
};

struct Simon::Implementation
{
    Implementation()

    {
        base_pass.id = -1;
        screen_w = -1;
        screen_h = -1;
        audio_started = false;
    }

    RenderTexture2D base_pass;
    int screen_w;
    int screen_h;

    std::vector<Cell> cells;
    std::vector<int> levels;
    size_t current_level;
    size_t current_level_step;

    GameState state;
    ElapsedTimer state_timer;

    std::string center_message;
    std::set<size_t> step_input;

    Menu start_menu;
    Menu game_menu;
    std::shared_ptr<Button> game_button;
    bool audio_started;
};

Simon::Simon()
{
    m = std::make_shared<Implementation>();
}

Simon::~Simon()
{
    m.reset();
}

bool Simon::processGameLogic()
{
    if (m->state == GameState::PlayBackCells)
    {
        m->current_level_step = 0;
        m->state_timer.invalidate();
        m->state = GameState::PlayBackCell;
        m->center_message = "";
    }
    else if (m->state == GameState::PlayBackCell)
    {

        if (!m->state_timer.isValid())
        {
            int cell = m->levels[m->current_level_step];
            m->cells[cell].setPressed(true);

            //logDbg("Simon", sfmt("Playback cell level=%d cell=%d", m->current_level_step, cell));
            m->state_timer.restart();
        }
        else if (m->state_timer.hasExpired(500))
        {
            m->state = GameState::WaitPlayBackCellEnd;
            for (auto &cell : m->cells)
                cell.setPressed(false);
            m->state_timer.restart();
        }
    }
    else if (m->state == GameState::WaitPlayBackCellEnd)
    {
        if (m->state_timer.hasExpired(200))
        {
            if (m->current_level_step == m->current_level)
            {
                m->current_level_step = 0;
                m->state = GameState::InformPlayerShouldStart;
                m->state_timer.restart();
                m->center_message = "Go!";
            }
            else
            {
                m->current_level_step++;
                m->state = GameState::PlayBackCell;
                m->state_timer.invalidate();
            }
        }
    }
    else if (m->state == GameState::InformPlayerShouldStart)
    {
        if (m->state_timer.hasExpired(MESSAGES_TIME))
        {
            m->state = GameState::WaitForInput;
            m->center_message = "";
            for (auto &cell : m->cells)
                cell.setPressed(false);
            m->current_level_step = 0;
        }
    }
    else if (m->state == GameState::WaitForInput)
    {
        bool all_released = true;
        for (size_t i = 0; i != m->cells.size(); ++i)
        {
            if (m->cells[i].isPressed())
            {
                all_released = false;
                m->step_input.insert(i);
            }
        }

        if (all_released && !m->step_input.empty())
        {
            int cell = m->levels[m->current_level_step];
            bool correct = (m->step_input.size() == 1) && (m->step_input.count(cell) == 1);
            m->step_input.clear();

            if (correct)
            {
                if (m->current_level_step == m->current_level)
                {
                    m->state = GameState::WaitPlayBackStart;
                    m->state_timer.restart();
                    m->center_message = "Very Well!";

                    m->current_level = (m->current_level + 1) % m->levels.size();
                }
                else
                {
                    m->current_level_step++;
                }
            }
            else
            {
                m->state = GameState::WaitPlayBackStart;
                m->state_timer.restart();
                m->center_message = "Try Again!";
            }
        }
        else
        {
            return true;
        }
    }
    else if (m->state == GameState::WaitPlayBackStart)
    {
        if (m->state_timer.hasExpired(MESSAGES_TIME))
        {
            m->state = GameState::PlayBackCells;
        }
    }
    else if (m->state == GameState::ShowPause)
    {
        for (auto &cell : m->cells)
            cell.setPressed(false);
    }

    return false;
}

void Simon::restartGame()
{
    m->levels.clear();

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> distribution(0, 3);

    for (int i = 0; i != 2500; ++i)
    {
        m->levels.push_back(distribution(engine));
    }

    m->current_level = 0;
    m->current_level_step = 0;
    m->state = GameState::PlayBackCells;
}

void Simon::update()
{

    bool resized = false;
    #ifdef PLATFORM_WEB
        resized = (GetScreenWidth() != m->screen_w) || (GetScreenHeight() != m->screen_h);
    #else
        resized = IsWindowResized();
    #endif

    if (m->base_pass.id == -1 || resized)
    {
        //
        // Setup Base Pass
        //
        if (m->base_pass.id != -1)
        {
            UnloadRenderTexture(m->base_pass);
            m->base_pass.id = -1;
        }
        m->base_pass = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        SetTextureFilter(m->base_pass.texture, TEXTURE_FILTER_BILINEAR);

        //
        // setup cell sizes
        //
        for (auto &cell : m->cells)
            cell.computeSizeParameters(GetScreenWidth(), GetScreenHeight());
    }

    m->screen_w = GetScreenWidth();
    m->screen_h = GetScreenHeight();

    //
    // Game Logic
    //
    if (processGameLogic())
    {
        //
        // button pressed
        //
        bool pressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        Vector2 mouse_position = GetMousePosition();
        for (auto &cell : m->cells)
            cell.setPressed(pressed && cell.inside(mouse_position));
    }

    //
    // cell update
    //
    for (auto &cell : m->cells)
        cell.update();

    //
    // GUI
    //
    m->start_menu.setActive(m->state == GameState::ShowHome);
    m->game_menu.setActive(m->state == GameState::ShowPause);
    m->game_button->setActive(m->state != GameState::ShowHome);

    m->start_menu.update();
    m->game_menu.update();
    m->game_button->update(Button::Profile::ScreenRightCorner);
}

void Simon::renderBackground()
{
    ClearBackground(RAYWHITE);

    Rectangle screen;
    screen.x = 0;
    screen.y = 0;
    screen.width = GetScreenWidth();
    screen.height = GetScreenHeight();

    Color c_start = RAYWHITE;
    Color c_mid = MINT_CREAM;
    Color c_end = BEAU_BLUE;

    DrawRectangleGradientEx(screen, c_mid, c_start, c_mid, c_end);
}

void Simon::renderCell(Cell &cell)
{
    float thickness = 0.01f;
    const Color border_color = OLD_LAVANDER;

    Vector2 v_line_start;
    v_line_start.x = cell.center().x;
    v_line_start.y = float(cell.center().y) - cell.radius();
    Vector2 v_line_end;
    v_line_end.x = cell.center().x;
    v_line_end.y = float(cell.center().y) + cell.radius();

    Vector2 h_line_start;
    h_line_start.x = float(cell.center().x) - cell.radius();
    h_line_start.y = cell.center().y;
    Vector2 h_line_end;
    h_line_end.x = float(cell.center().x) + cell.radius();
    h_line_end.y = cell.center().y;

    BeginScissorMode(cell.scissor().x, cell.scissor().y, cell.scissor().width, cell.scissor().height);
    {
        DrawCircle(cell.center().x, cell.center().y, cell.radius() * (1.0 + thickness), border_color);
        DrawCircleGradient(cell.center().x, cell.center().y, cell.radius(), cell.centerColor(), cell.color());
        DrawLineEx(h_line_start, h_line_end, cell.radius() * thickness, border_color);
        DrawLineEx(v_line_start, v_line_end, cell.radius() * thickness, border_color);
    }
    EndScissorMode();
}

void Simon::render()
{
    //
    // Render Base Pass
    //
    BeginTextureMode(m->base_pass);
    {
        renderBackground();
        for (auto &cell : m->cells)
        {
            renderCell(cell);
        }

        if (!m->center_message.empty())
        {
            float font_size = 34.0f;

            int size = MeasureText(m->center_message.c_str(), font_size);
            int x = (GetScreenWidth() - size) / 2;
            int y = (GetScreenHeight() - font_size) / 2;

            int margin = 10;

            Rectangle region;
            region.x = x - margin;
            region.y = y - margin;
            region.width = size + 2 * margin;
            region.height = font_size + 2 * margin;

            Color c_start = RAYWHITE;
            Color c_mid = MINT_CREAM;
            Color c_end = BEAU_BLUE;

            DrawRectangleGradientEx(region, c_mid, c_start, c_mid, c_end);
            DrawRectangleLinesEx(region, font_size * 0.1f, OLD_LAVANDER);

            DrawText(m->center_message.c_str(), x, y, font_size, OLD_LAVANDER);
        }

        DrawText(re::sfmt("Level %d [%d]", m->current_level + 1, m->current_level_step + 1).c_str(),
                 20, 20, 20, LIGHTGRAY);

        if (m->state == GameState::ShowHome)
            m->start_menu.render();
        else if (m->state == GameState::ShowPause)
            m->game_menu.render();
        else
        {
            m->game_button->render();
        }
    }
    EndTextureMode();

    //
    // Render the texture to screen
    //
    BeginDrawing();
    {
        Rectangle src{0.0f, 0.0f, (float)m->base_pass.texture.width, -(float)m->base_pass.texture.height};
        Rectangle dst{0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
        Vector2 origin{0, 0};

        DrawTexturePro(m->base_pass.texture, src, dst, origin, 0.0f, WHITE);
    }
    EndDrawing();
}

void Simon::step()
{
    update();
    render();
}

void Simon::setup()
{
    SetTraceLogLevel(LOG_INFO);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE /*| FLAG_MSAA_4X_HINT*/);


    InitWindow(800, 450, "Baby Simon");

#ifndef PLATFORM_WEB
    SetTargetFPS(30);
#endif 

    m->cells.push_back(Cell::CellTL);
    m->cells.push_back(Cell::CellTR);
    m->cells.push_back(Cell::CellBL);
    m->cells.push_back(Cell::CellBR);

    m->start_menu.addButton("button_play.png", [this]() { 
        restartGame(); 

        InitAudioDevice(); // Initialize audio device
        
        // audio can only start in response to a click
        for (auto &cell : m->cells)
            cell.enableAudio();
    });

    m->game_menu.addButton("button_restart.png", [this]()
                           { restartGame(); });

    m->game_menu.addButton("button_resume.png", [this]()
                           { m->state = GameState::PlayBackCells; });

    m->game_button = std::make_shared<Button>("button_menu.png");
    m->game_button->setCallback([this]()
                                {
                                    m->center_message = "";
                                    m->state = GameState::ShowPause;
                                });

    restartGame();
    m->state = GameState::ShowHome;
}

void Simon::shutdown()
{
    if (m->base_pass.id != -1)
    {
        UnloadRenderTexture(m->base_pass);
        m->base_pass.id = -1;
    }

    m->game_button.reset();
    m->start_menu.clear();
    m->game_menu.clear();
    m->cells.clear();

    if (m->audio_started) {
        CloseAudioDevice();
    }

    CloseWindow();
}

void Simon::run()
{

    while (!WindowShouldClose())
    {
        step();
    }
}
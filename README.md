# OrbitInvaders

Space Invaders–inspired game built in **Unreal Engine 5.7** (C++ + Blueprints).  
The player orbits Earth and must stop invaders and asteroids from colliding with it.

## Features

- Main Menu (Start / Endless / Options / Quit)
- Pause menu (Escape)
- End screen (win / lose + score)
- Invader formation around Earth, bunkers, special spiral invader
- Asteroids (split on hit; touching Earth ends the game)
- Earth HP, player lives (hearts), score, Options menu for gameplay tuning
- Endless mode with rising difficulty per wave

## Requirements

- Unreal Engine **5.7.x**
- Visual Studio 2022 with C++ game development workload
- [Git LFS](https://git-lfs.com/) (required to pull `.uasset` / `.umap` assets)

## How to build & run

1. Clone the repository and make sure Git LFS pulled assets:
   ```bash
   git clone https://github.com/BagsikMr/OrbitInvaders.git
   cd OrbitInvaders
   git lfs pull
   ```
2. Right-click `OrbitInvaders.uproject` → **Generate Visual Studio project files**.
3. Open the generated `.sln` in Visual Studio, set configuration to **Development Editor**, build the solution.
4. Open `OrbitInvaders.uproject` in Unreal Editor (or launch from Visual Studio).
5. Open the game map under `Content/Maps` and press **Play**.

If the editor asks about a missing **VisualStudioTools** plugin, disable/remove it — it is optional and not required to compile or play.

## Controls

- Move on orbit: **A** (left) / **D** (right) (can be changed in IMC_Default)
- Fire: **Space**
- Pause: **Escape** (test in Standalone Game if PIE swallows Escape)

## Project layout

| Path | Contents |
|------|----------|
| `Source/OrbitInvaders/` | C++ gameplay (Core, Actors, UI) |
| `Content/Blueprints/` | BP subclasses (GameMode, pawn, etc.) |
| `Content/UI/` | Widget Blueprints (menus, HUD, options) |
| `Content/Maps/` | Game level |
| `Config/` | Project defaults |

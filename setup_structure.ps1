$directories = @(
    "src/entities",
    "src/systems",
    "src/states",
    "src/ui",
    "src/managers",
    "src/core"
)

$files = @(
    "src/entities/Player.cpp",
    "src/entities/PlayerHealth.cpp",
    "src/entities/PlayerEvolution.cpp",
    "src/entities/Enemy.cpp",
    "src/entities/EnemySpawner.cpp",
    "src/entities/CollisionHandler.cpp",
    "src/systems/InputSystem.cpp",
    "src/systems/BackgroundScroller.cpp",
    "src/systems/EffectsManager.cpp",
    "src/systems/SoundManager.cpp",
    "src/systems/Timer.cpp",
    "src/systems/AudioSystem.cpp",
    "src/systems/ConfigManager.cpp",
    "src/states/GameStateManager.cpp",
    "src/ui/MainMenu.cpp",
    "src/ui/SettingsMenu.cpp",
    "src/ui/UIManager.cpp",
    "src/managers/AssetManager.cpp",
    "src/core/Game.cpp",
    "src/core/Renderer.cpp",
    "src/core/Window.cpp",
    "src/core/SoundManager.cpp"
)

# Tạo thư mục
foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path $dir -Force | Out-Null
}

# Tạo file rỗng
foreach ($file in $files) {
    New-Item -ItemType File -Path $file -Force | Out-Null
}

Write-Output "Directory structure and files created successfully!"

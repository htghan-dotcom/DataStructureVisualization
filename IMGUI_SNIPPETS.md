# ImGui Integration Snippets (for SFML + ImGui-SFML)

## 1) Initialization with modern font

```cpp
#include <imgui.h>
#include <imgui-SFML.h>

// After creating sf::RenderWindow window...
ImGui::SFML::Init(window);

ImGuiIO& io = ImGui::GetIO();
io.Fonts->Clear();
// Example font paths (adjust to your machine/project):
// io.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMono-Regular.ttf", 19.0f);
// io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 19.0f);
io.Fonts->AddFontDefault();
ImGui::SFML::UpdateFontTexture();
```

## 2) Suggested panel layout

```cpp
// Left panel: controls
ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
ImGui::SetNextWindowSize(ImVec2(260, 560), ImGuiCond_Always);
ImGui::Begin("Control Panel", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
ImGui::SeparatorText("Graph Setup");
// New Graph, Add Node, Delete Node, Add/Modify Edges, Random, Build, View

ImGui::SeparatorText("Algorithms");
// Kruskal, Prim
ImGui::End();

// Right panel: information
ImGui::SetNextWindowPos(ImVec2(950, 10), ImGuiCond_Always);
ImGui::SetNextWindowSize(ImVec2(320, 560), ImGuiCond_Always);
ImGui::Begin("Information", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
// Step counter, workflow status, pseudocode
ImGui::End();

// Bottom panel: playback
ImGui::SetNextWindowPos(ImVec2(10, 585), ImGuiCond_Always);
ImGui::SetNextWindowSize(ImVec2(1260, 125), ImGuiCond_Always);
ImGui::Begin("Playback", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
// Play/Pause/Step/Skip + speed slider
ImGui::End();
```

## 3) Button color styling

```cpp
// Play (green)
ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.55f, 0.30f, 1.0f));
ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.65f, 0.36f, 1.0f));
ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.16f, 0.48f, 0.26f, 1.0f));
if (ImGui::Button("Play")) { /* ... */ }
ImGui::PopStyleColor(3);

// Delete Node (red)
ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.63f, 0.23f, 0.23f, 1.0f));
ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.74f, 0.30f, 0.30f, 1.0f));
ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.55f, 0.19f, 0.19f, 1.0f));
if (ImGui::Button("Delete Node")) { /* ... */ }
ImGui::PopStyleColor(3);

// Active algorithm (yellow/orange)
const bool isActiveAlgo = true;
if (isActiveAlgo) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.95f, 0.65f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 0.72f, 0.32f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.90f, 0.58f, 0.20f, 1.0f));
}
if (ImGui::Button("Kruskal")) { /* ... */ }
if (isActiveAlgo) ImGui::PopStyleColor(3);
```

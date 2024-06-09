#include <Geode/Geode.hpp>
#include "utils.hpp"

#if defined(GEODE_IS_WINDOWS)
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

using namespace geode::prelude;

#include "../api/stringPatch.hpp"

void patchStrings() {
    // uint16_t test = 0x01; // РРрррррррррррррррр
    // Mod::get()->patch((void*)(base::get() + 0x511FD4), ByteVector((uint8_t*)&test, (uint8_t*)&test + 2));
    Mod::get()->patch((void*)(base::get() + 0x511FD4), {0x01, 0x00});  // Ррррррррррррррр
    
    // bool res;
    // res = gdl::patchCString(base::get() + 0x3151D5, "Привет, мир!");
    // log::debug("{}", res);
    // res = gdl::patchCString(base::get() + 0x350598, "Привет, мир 2!");
    // log::debug("{}", res);
    // res = gdl::patchCString(base::get() + 0x3505F1, "Привет, мир 3!");
    // log::debug("{}", res);
    // res = gdl::patchStdString(base::get() + 0x315641, "Hello world!");
    // log::debug("{}", res);

//     static std::vector<std::string> strings;

//     for (auto p : Mod::get()->getPatches()) {
//         if (p->isEnabled()) {
//             auto res = p->disable().err();
//             if (res != std::nullopt) {
//                 log::warn("Failed to disable patch at {}, error: {}", p->getAddress() - base::get(), res);
//             }
//         }
//     }

//     auto locationsFile = gdlutils::loadJson((Mod::get()->getResourcesDir() / "ru_locations.json").string());
//     auto langFile = gdlutils::loadJson((Mod::get()->getResourcesDir() / "ru_lang.json").string());

//     strings.clear();
//     strings.reserve(langFile.size());
    
// #if defined(GEODE_IS_WINDOWS)
//     auto patchFile = gdlutils::loadJson((Mod::get()->getResourcesDir() / "win-2.204.json").string());

//     for (const auto& pair : langFile.items()) {
//         if (!patchFile.contains(pair.key()))
//             continue;

//         strings.push_back(pair.value());

//         for (const auto addr : patchFile[pair.key()]) {
//             const char* str = strings[strings.size() - 1].c_str();
//             auto res = Mod::get()->patch((void*)(base::get() + (uintptr_t)addr), ByteVector((uint8_t*)&str, (uint8_t*)&str + 4)).err();
//             if (res != std::nullopt) {
//                 log::warn("Failed to patch string at 0x{:X}, error: {}", (uintptr_t)addr, res);
//             }
//         }
//     }
// #elif defined(GEODE_IS_ANDROID32)
//     auto patchFile = gdlutils::loadJson((Mod::get()->getResourcesDir() / "android32-2.205.json").string());

//     for (const auto& pair : langFile.items()) {
//         if (!patchFile.contains(pair.key()))
//             continue;

//         strings.push_back(pair.value());

//         const char* str = strings[strings.size() - 1].c_str();
//         auto array = patchFile[pair.key()].get<nlohmann::json::array_t>();

//         for(const auto& addr : array[0].get<nlohmann::json::array_t>()) {
//             Mod::get()->patch((void*)(base::get() + addr.get<uintptr_t>()), ByteVector((uint8_t*)&str, (uint8_t*)&str + 4));
//         }

//         for(const auto& addr : array[1].get<nlohmann::json::array_t>()) {
//             Mod::get()->patch((void*)(base::get() + addr.get<uintptr_t>()), ByteVector({0x00, 0xBF}));
//         }
//     }
// #elif defined(GEODE_IS_ANDROID64)
//     auto patchFile = gdlutils::loadJson((Mod::get()->getResourcesDir() / "android64-2.205.json").string());

//     // coming soon
// #endif
}

$execute {
#if defined(GEODE_IS_WINDOWS)
    SetConsoleOutputCP(65001); // utf8

    BindManager::get()->registerBindable({
        "reload"_spr,
        "Reload lang file",
        "",
        { Keybind::create(KEY_P, Modifier::None) },
        "GDL/Debug"
    });

    new EventListener([=](InvokeBindEvent* event) {
        static bool wasPressed = false;

        if (event->isDown()) {
            if (!wasPressed) {
                wasPressed = true;
                patchStrings();
                Notification::create("GDL: Updated strings", NotificationIcon::Success)->show();
            }
        } else {
            wasPressed = false;
        }
        
	    return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "reload"_spr));
#endif

    patchStrings();
}
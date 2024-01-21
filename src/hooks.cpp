#include "Geode/Geode.hpp"
#include "Geode/modify/MultilineBitmapFont.hpp"
#include "Geode/modify/CCLabelBMFont.hpp"
#include "Geode/modify/CCTextureCache.hpp"

#include "utils.hpp"
#include <utf8.h>

using namespace geode::prelude;

class $modify(MultilineBitmapFont) {
    gd::string stringWithMaxWidth(gd::string p0, float p1, float p2) {
        log::debug("{};{};{}", p0, p1, p2);

        std::string str = p0;

        std::string s;
        auto it = str.begin();
        for (auto i = 0u; i < 10 && it != str.end(); i++) {
            // utf8::append()
            auto cp = utf8::next(it, str.end());
            utf8::append(cp, s);
        }
        
        // return MultilineBitmapFont::stringWithMaxWidth(p0, p1, p2);
        return s;
    }
};

cocos2d::CCBMFontConfiguration* FNTConfigLoadFile_hk(char const* name) {
    log::debug("font {}", name);
    auto newName = ghc::filesystem::relative(Mod::get()->getResourcesDir() / name);
    // std::string newPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(newName.string().c_str(), false);
    log::debug("!!{}", newName.string());
    if (ghc::filesystem::exists(newName)) {
        log::debug("overriding font {}", newName.string());
        return cocos2d::FNTConfigLoadFile(newName.string().c_str());
    }
    return cocos2d::FNTConfigLoadFile(name);
}

class $modify(CCTextureCache) {
    cocos2d::CCTexture2D* addImage(const char* name, bool idk) {
        // log::debug("tex {}", name);
        auto newName = ghc::filesystem::relative(Mod::get()->getResourcesDir() / name);
        if (ghc::filesystem::exists(newName)) {
            log::debug("overriding texture {}", newName.string());
            return CCTextureCache::addImage(newName.string().c_str(), idk);
        }
        return CCTextureCache::addImage(name, idk);
    }
};

void (__thiscall* std_string_assign_o)(void* self, char* src, size_t len);
void std_string_assign_hk(void* self, char* src, size_t len) {
/*     
        Зачем нужны эти хуки:
        В некоторых случаях в гд, функция GDString::assign вызывается в заданной длинной,
        которая ограничивает длинну строки, но переведенная строка может быть длиннее чем оригинальная.
        Было бы намного сложнее пропатчить все длины строк, учитывая то, что не везде указывается длина строки.
        В общем намного легче хукнуть эту функция и установить новую длину строки.

        Why this is required:
        In some places of gd you can see GDString::assign called with fixed length
        which limits strings to the length it was in the game but the translated strings may be longer than the original ones.
        It is also pretty difficult to patch the length as it is not used in all calls and i would somehow need to find places to patch
        So it is easier to hook it and just set it to the string length instead of the fixed length
*/
	std_string_assign_o(self, src, strlen(src));
}

$execute {
    auto addr = GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?FNTConfigLoadFile@cocos2d@@YAPAVCCBMFontConfiguration@1@PBD@Z");
    Mod::get()->hook((void*)addr, FNTConfigLoadFile_hk, "cocos2d::FNTConfigLoadFile", tulip::hook::TulipConvention::Cdecl);
    
    constexpr auto STD_STR_ASSIGN_ADDR = 0x1BB10;
    std_string_assign_o = reinterpret_cast<void (__thiscall*)(void* self, char* src, size_t len)>(base::get() + STD_STR_ASSIGN_ADDR);
    Mod::get()->hook((void*)(base::get() + STD_STR_ASSIGN_ADDR), std_string_assign_hk, "std::string::assign", tulip::hook::TulipConvention::Thiscall);
}
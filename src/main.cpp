#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <fmt/format.h>
#include "FrameMath.hpp"

using namespace geode::prelude;

class $modify(FPEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto spr = ButtonSprite::create("FP");
        spr->setScale(0.7f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(FPEditorUI::onCreateFP));
        btn->setID("fp-creator-button"_spr);

        if (auto menu = this->getChildByID("undo-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        } else {
            auto menu = CCMenu::create();
            menu->setPosition({40.f, CCDirector::get()->getWinSize().height - 40.f});
            menu->addChild(btn);
            this->addChild(menu, 10);
        }
        return true;
    }

    void onCreateFP(CCObject*) {
        auto selected = this->getSelectedObjects();
        if (!selected || selected->count() == 0) {
            Notification::create("Selecciona el bloque de salida", NotificationIcon::Warning)->show();
            return;
        }
        auto start = static_cast<GameObject*>(selected->objectAtIndex(0));

        auto mod = Mod::get();
        auto res = fp::compute(
            static_cast<int>(mod->getSettingValue<int64_t>("play-fps")),
            static_cast<int>(mod->getSettingValue<int64_t>("design-fps")),
            static_cast<int>(mod->getSettingValue<int64_t>("tps")),
            mod->getSettingValue<std::string>("speed"),
            static_cast<int>(mod->getSettingValue<int64_t>("margin-override")),
            mod->getSettingValue<double>("air-time"),
            mod->getSettingValue<double>("hitbox-width")
        );

        // Bloque de aterrizaje (ID 1 = bloque basico, 30x30) a "hueco" unidades del borde derecho
        auto rect = start->getObjectRect();
        float x = rect.getMaxX() + static_cast<float>(res.gapUnits) + 15.f;
        float y = start->getPositionY();
        m_editorLayer->createObject(1, ccp(x, y), false);

        Notification::create(
            fmt::format("Margen {} | hueco {:.2f}u | ventana {:.2f}u", res.margin, res.gapUnits, res.windowUnits),
            NotificationIcon::Success
        )->show();
    }
};

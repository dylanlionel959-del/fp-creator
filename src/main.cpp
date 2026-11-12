#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

// Modificamos la interfaz del editor para inyectar nuestras herramientas
class $modify(Maxr3EditorUI, EditorUI) {
    
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        // Notificación de prueba para verificar que tu mod carga correctamente en el editor
        // Aquí es donde posteriormente añadiremos los botones de TPS y guías de frames.
        Notification::create(
            "Frame Perfect Creator cargado", 
            NotificationIcon::Success
        )->show();

        return true;
    }
};

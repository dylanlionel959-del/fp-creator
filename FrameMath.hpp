#pragma once
#include <algorithm>
#include <cmath>
#include <string>

namespace fp {

struct Result {
    int margin;          // margenes libres
    double stepUnits;    // unidades que avanza el jugador en 1 tick
    double windowUnits;  // ventana total de salto = (1 + margen) ticks
    double jumpLength;   // alcance horizontal de un salto
    double gapUnits;     // hueco entre bordes de los bloques
};

// Unidades por segundo de cada velocidad del juego
inline double speedUnitsPerSec(std::string const& s) {
    if (s == "0.7x") return 251.16;
    if (s == "2x")   return 387.42;
    if (s == "3x")   return 468.0;
    if (s == "4x")   return 576.0;
    return 311.58; // 1x
}

// Regla de margenes (jugar -> poner):
//   120 -> 60 = 1   |   120 -> 30 = 2   |   60 -> 30 = 2
// Si no esta en la tabla se usa (jugar / poner) - 1.
inline int marginFor(int playFps, int designFps, int manual) {
    if (manual >= 0) return manual;
    if (playFps <= designFps) return 0;
    if (playFps == 120 && designFps == 60) return 1;
    if (playFps == 120 && designFps == 30) return 2;
    if (playFps == 60  && designFps == 30) return 2;
    return std::max(0, static_cast<int>(std::lround(double(playFps) / designFps)) - 1);
}

// Ventana de salto: el jugador puede saltar mientras su hitbox siga tocando el
// borde del bloque de salida y todavia llegue al bloque de aterrizaje:
//   ventana = alcance + ancho_hitbox - hueco
// => hueco = alcance + ancho_hitbox - (1 + margen) * paso_de_tick
inline Result compute(int playFps, int designFps, int tps, std::string const& speed,
                      int manualMargin, double airTime, double hitboxWidth) {
    Result r{};
    double v = speedUnitsPerSec(speed);
    r.margin = marginFor(playFps, designFps, manualMargin);
    r.stepUnits = v / std::max(1, tps);
    r.windowUnits = (1 + r.margin) * r.stepUnits;
    r.jumpLength = v * airTime;
    r.gapUnits = std::max(0.0, r.jumpLength + hitboxWidth - r.windowUnits);
    return r;
}

} // namespace fp

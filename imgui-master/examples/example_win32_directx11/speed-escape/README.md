# Neon Heist: Escape

Juego de carreras y extraccion para Roblox, escrito en Luau y organizado con **Rojo**.

El place listo para abrir es `NeonHeist.rbxlx`; tambien se genera `SpeedEscape.rbxlx` por compatibilidad. Ambos se construyen desde `src/`.

## Que incluye

- Pista de 6.000 studs generada por codigo (suelo, obstaculos, boosts, 11 checkpoints y meta).
- +1 de velocidad por cada tecla pulsada (con limite anti-spam en el servidor).
- Contrato de robo: recoge 3 nucleos en orden y extraelos en la meta para cobrar la recompensa grande.
- La velocidad del personaje escala con la stat, con tope para que siga siendo jugable.
- Monedas por checkpoints y por completar la pista.
- Tienda con 3 mejoras: velocidad, monedas y salto.
- Renacimiento (prestige): resetea velocidad y da bonus permanente.
- Guardado automatico con DataStore.
- HUD, barra de progreso, avisos, tienda y efecto de FOV al correr.
- Misiones, leaderboard de sesion, combo de monedas y controles tactiles para movil.
- Bono diario con racha de 7 dias, recompensa creciente y retorno diario.
- Direccion visual cyberpunk con sala de operaciones, skyline lateral, pasos elevados, set pieces por sector, portales circulares, luminarias, franjas de velocidad, nucleos luminosos y HUD responsive.
- Hooks para 2 gamepasses (doble velocidad y doble monedas).
- Si al menos un `PassId` esta configurado, el HUD muestra `PASE DE OPERADOR` y abre la compra directamente.

## Estructura

```
speed-escape/
├── default.project.json
├── README.md
└── src/
    ├── ReplicatedStorage/
    │   └── Shared/
    │       └── Config.lua
    ├── ServerScriptService/
    │   ├── Main.server.lua
    │   ├── DataService.lua
    │   └── MapBuilder.lua
    └── StarterPlayer/
        └── StarterPlayerScripts/
            ├── InputController.client.lua
            └── UIController.client.lua
```

## Opcion A: con Rojo (recomendado)

### 1. Instalar Rojo

1. Descarga el `.zip` de Windows desde <https://github.com/rojo-rbx/rojo/releases>.
2. Extrae `rojo.exe` (puedes dejarlo dentro de esta carpeta).
3. Opcional: agrega la carpeta al `PATH` para escribir `rojo` en cualquier lado.

### 2. Instalar el plugin de Rojo en Studio

1. Abre Roblox Studio y crea un place nuevo con la plantilla **Baseplate**.
2. Ve a **Toolbox > Creator Store** y busca **Rojo** (autor: rojo-rbx).
3. Instala el plugin.
   - Si no aparece en la tienda, descarga `Rojo.rbxm` desde la misma pagina de releases y arrastralo a Studio.

### 3. Conectar

En PowerShell, dentro de esta carpeta:

```powershell
.\rojo.exe serve
```

Luego, en Studio:

1. Abre el plugin **Rojo**.
2. Pulsa **Connect** (por defecto usa `localhost:34872`).
3. Espera a que sincronice. El mapa se genera solo al iniciar el servidor.

### 4. Activar DataStore en Studio

**Home > Game Settings > Security > Enable Studio Access to API Services**.

Sin esto, el juego funciona, pero el progreso no se guarda y veras advertencias en el Output.

### 5. Probar

Pulsa **F5** (Play). Deberias aparecer en la pista, ver el HUD y ganar +1 por tecla.

## Opcion B: sin Rojo (copiar y pegar)

1. En Studio, crea esta jerarquia:

```
ReplicatedStorage
└── Shared (Folder)
    └── Config (ModuleScript)  <- pega Config.lua

ServerScriptService
├── Main (Script)              <- pega Main.server.lua
├── DataService (ModuleScript) <- pega DataService.lua
└── MapBuilder (ModuleScript)  <- pega MapBuilder.lua

StarterPlayer
└── StarterPlayerScripts
    ├── InputController (LocalScript) <- pega InputController.client.lua
    └── UIController (LocalScript)    <- pega UIController.client.lua
```

2. Los remotes (`Remotes` y sus RemoteEvents/RemoteFunction) los crea el servidor automaticamente si no existen. Si prefieres crearlos a mano, deben llamarse exactamente: `AddSpeed`, `Rebirth`, `BuyUpgrade`, `SyncState`, `Notify`, `FX`, `Leaderboard` y `ClaimDaily` (RemoteEvents), y `GetState` (RemoteFunction), dentro de una carpeta `Remotes` en `ReplicatedStorage`.

3. Activa API Services y pulsa **F5**.

## Configuracion

Todo el balance esta en `src/ReplicatedStorage/Shared/Config.lua`:

| Campo | Que hace |
|-------|----------|
| `RebirthRequirement` | Velocidad necesaria para renacer (por defecto 2.500). |
| `CheckpointCoinBase` | Monedas base por checkpoint. |
| `FinishReward` | Monedas por completar la pista. |
| `CheckpointSpeedBonus` | Velocidad extra al pasar un checkpoint. |
| `BoostAmount` | Velocidad que da una plataforma amarilla. |
| `ObstaclePenalty` | Porcentaje de velocidad que pierdes al chocar. |
| `SpeedCurve`, `MaxWalkSpeed` | Como se convierte la stat en velocidad real. |
| `Upgrades` | Coste, crecimiento y efecto de cada mejora. |
| `DailyRewards` | Recompensas de monedas por cada dia consecutivo reclamado. |

Los cambios de `Config.lua` se sincronizan en caliente con Rojo, pero **reinicia el playtest** para que el servidor los aplique.

## Gamepasses (monetizacion)

1. Publica la experiencia primero (aunque sea privada).
2. En el **Creator Hub**, entra a tu experiencia > **Monetization > Passes**.
3. Crea:
   - **Doble Velocidad** (sugerido: 99-199 Robux).
   - **Doble Monedas** (sugerido: 99-199 Robux).
4. Copia cada **Pass ID** y pegalo en `Config.PassIds`.
5. Reinicia el servidor. El juego detecta la compra al entrar.

Nota: los gamepasses no se pueden probar comprando en Studio; publica y prueba desde la pagina del juego.

## Publicar

1. **File > Publish to Roblox**.
2. Nombre, descripcion, icono y thumbnail. En este genero el thumbnail es el 80% del click.
3. Genero sugerido: **Simulator**.
4. Max players: 12-20 para el MVP.
5. Comparte el link y pide feedback de las primeras 24 horas.

## Limitaciones conocidas (MVP)

- El control tactil esta incluido; aun conviene probar el escalado en varios telefonos y tablets.
- DataStore sin session locking: si dos servidores cargan al mismo jugador a la vez, puede haber duplicacion. Para produccion, migra a ProfileService.
- Anticheat basico (limite de teclas y validacion de compras). No hay proteccion contra exploits de movimiento.
- Sin sonidos, cosmeticos, mascotas ni pase de batalla.

## Siguientes pasos sugeridos

1. Probar el loop y ajustar numeros (cuantas monedas por run, cuanto tarda el primer renacimiento).
2. Agregar sonido de tecla, boost y meta.
3. Agregar leaderboard global (Top Speed) con `OrderedDataStore`.
4. Agregar recompensa diaria y pase de batalla.
5. Probar 3 thumbnails distintos y mirar el CTR en Analytics.

## Nota honesta

Esto es un MVP funcional, no una maquina de dinero. El genero esta saturado; lo que decide el resultado es la retencion, el thumbnail y las actualizaciones constantes. Usa este proyecto para aprender el pipeline completo y luego invierte el tiempo en el giro que lo haga distinto.

-- Configuracion compartida entre servidor y cliente.
-- Todo el balance del juego esta aqui.

local Config = {}

-- Mapa
Config.TrackLength = 6000
Config.TrackWidth = 44
Config.CheckpointSpacing = 500
Config.CheckpointCount = 11
Config.ZoneLength = 1000
Config.ZoneCount = 6

-- Contrato principal: recoger nucleos en orden y extraer en la meta.
Config.HeistCoreCount = 3
Config.HeistCoreReward = 500
Config.HeistCompletionReward = 2500

-- Zonas visuales (cada 1000 studs)
Config.Zones = {
	{ Name = "CIUDAD NEON", FloorA = Color3.fromRGB(22, 26, 48), FloorB = Color3.fromRGB(30, 36, 66), Accent = Color3.fromRGB(80, 220, 255) },
	{ Name = "TUNEL MAGENTA", FloorA = Color3.fromRGB(34, 20, 46), FloorB = Color3.fromRGB(46, 26, 60), Accent = Color3.fromRGB(255, 90, 220) },
	{ Name = "ZONA TOXICA", FloorA = Color3.fromRGB(18, 38, 32), FloorB = Color3.fromRGB(24, 50, 40), Accent = Color3.fromRGB(80, 255, 140) },
	{ Name = "SECTOR SOLAR", FloorA = Color3.fromRGB(48, 32, 16), FloorB = Color3.fromRGB(60, 40, 20), Accent = Color3.fromRGB(255, 170, 60) },
	{ Name = "ABISMO VIOLETA", FloorA = Color3.fromRGB(32, 20, 52), FloorB = Color3.fromRGB(42, 26, 66), Accent = Color3.fromRGB(170, 110, 255) },
	{ Name = "NUCLEO ROJO", FloorA = Color3.fromRGB(48, 20, 24), FloorB = Color3.fromRGB(60, 26, 30), Accent = Color3.fromRGB(255, 80, 90) },
}

-- Velocidad
Config.BaseWalkSpeed = 20
Config.SpeedCurve = 13
Config.MaxWalkSpeed = 120
Config.KeyRateLimit = 35

-- Combo por tecleo
Config.ComboWindow = 0.85
Config.ComboMax = 100
Config.ComboCoinBonus = 0.01 -- +1% monedas por punto de combo (max +100%)

-- Recompensas
Config.CheckpointSpeedBonus = 50
Config.CheckpointCoinBase = 40
Config.FinishReward = 1500
Config.DailyRewards = { 250, 400, 650, 950, 1400, 2000, 3000 }
Config.DailySpeedReward = 25
Config.BoostAmount = 100
Config.BoostCooldown = 2
Config.ObstaclePenalty = 0.25
Config.ObstacleCooldown = 1.5

-- Monedas del mapa
Config.CoinValue = 10
Config.CoinRespawn = 18
Config.CoinPerZone = 8
Config.MagnetBase = 8

-- Renacimiento (prestigio)
Config.RebirthRequirement = 2500
Config.RebirthSpeedBonus = 0.25
Config.RebirthCoinBonus = 0.25

-- Mejoras compradas con monedas
Config.Upgrades = {
	Speed = { MaxLevel = 30, BaseCost = 500, Growth = 1.50, Effect = 0.05, Label = "Velocidad" },
	Coins = { MaxLevel = 30, BaseCost = 750, Growth = 1.50, Effect = 0.10, Label = "Monedas" },
	Auto = { MaxLevel = 20, BaseCost = 1200, Growth = 1.60, Effect = 1, Label = "Auto +1/s" },
	Magnet = { MaxLevel = 15, BaseCost = 900, Growth = 1.55, Effect = 4, Label = "Iman" },
	Jump = { MaxLevel = 10, BaseCost = 1000, Growth = 1.70, Effect = 5, Label = "Salto" },
}

-- Misiones: objetivo = BaseTarget * Growth^nivel + Step * nivel
Config.Missions = {
	Keys = { Label = "Presiona teclas", BaseTarget = 300, Growth = 1.45, Step = 0, BaseReward = 200, RewardGrowth = 1.55 },
	Coins = { Label = "Recoge monedas", BaseTarget = 400, Growth = 1.50, Step = 0, BaseReward = 250, RewardGrowth = 1.55 },
	Checkpoint = { Label = "Pasa checkpoints", BaseTarget = 3, Growth = 1.00, Step = 2, BaseReward = 350, RewardGrowth = 1.60 },
}

-- Rangos (segun velocidad)
Config.RankTiers = {
	{ Speed = 0, Name = "NOVATO", Color = Color3.fromRGB(170, 180, 210) },
	{ Speed = 250, Name = "VELOCISTA", Color = Color3.fromRGB(80, 255, 150) },
	{ Speed = 1000, Name = "SUPERSONICO", Color = Color3.fromRGB(80, 220, 255) },
	{ Speed = 2500, Name = "HIPERSONICO", Color = Color3.fromRGB(255, 215, 60) },
	{ Speed = 10000, Name = "LIGHTSPEED", Color = Color3.fromRGB(255, 120, 60) },
	{ Speed = 50000, Name = "LEYENDA", Color = Color3.fromRGB(255, 80, 220) },
}

-- Pega aqui los IDs de tus gamepasses (0 = desactivado).
Config.PassIds = {
	DoubleSpeed = 0,
	DoubleCoins = 0,
}

function Config.GetUpgradeCost(key, level)
	local upgrade = Config.Upgrades[key]
	if not upgrade then
		return math.huge
	end
	return math.floor(upgrade.BaseCost * (upgrade.Growth ^ level))
end

function Config.GetMissionTarget(key, level)
	local mission = Config.Missions[key]
	if not mission then
		return math.huge
	end
	local target = mission.BaseTarget * (mission.Growth ^ level) + mission.Step * level
	return math.max(1, math.min(1e9, math.floor(target)))
end

function Config.GetMissionReward(key, level)
	local mission = Config.Missions[key]
	if not mission then
		return 0
	end
	return math.max(1, math.min(1e9, math.floor(mission.BaseReward * (mission.RewardGrowth ^ level))))
end

function Config.GetWalkSpeed(speed, rebirths, speedLevel, hasDoubleSpeed)
	local base = Config.BaseWalkSpeed + Config.SpeedCurve * math.log10(1 + speed / 10)
	local multiplier = (1 + rebirths * Config.RebirthSpeedBonus)
		* (1 + speedLevel * Config.Upgrades.Speed.Effect)
	if hasDoubleSpeed then
		multiplier *= 2
	end
	return math.clamp(base * multiplier, Config.BaseWalkSpeed, Config.MaxWalkSpeed)
end

function Config.GetComboMultiplier(combo)
	return 1 + math.min(combo or 0, Config.ComboMax) * Config.ComboCoinBonus
end

return Config

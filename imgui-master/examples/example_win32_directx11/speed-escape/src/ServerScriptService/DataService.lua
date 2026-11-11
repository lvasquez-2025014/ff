-- Guardado y carga de progreso con DataStore.
-- Si el place no esta publicado, usa memoria para poder probar en Studio.
local DataStoreService = game:GetService("DataStoreService")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local Config = require(ReplicatedStorage:WaitForChild("Shared"):WaitForChild("Config"))

local STORE_NAME = "SpeedEscape_v2"
local cache = {}
local memory = {}
local store = nil

do
	local ok, result = pcall(function()
		return DataStoreService:GetDataStore(STORE_NAME)
	end)
	if ok then
		store = result
	else
		warn("[DataService] DataStore no disponible: " .. tostring(result))
		warn("[DataService] El progreso se guardara solo en memoria durante esta sesion.")
	end
end

local function newUpgrades()
	local upgrades = {}
	for key in pairs(Config.Upgrades) do
		upgrades[key] = 0
	end
	return upgrades
end

local function newMissions()
	local missions = {}
	for key in pairs(Config.Missions) do
		missions[key] = { Level = 0, Progress = 0 }
	end
	return missions
end

local function newData()
	return {
		Speed = 0,
		Coins = 0,
		Rebirths = 0,
		Checkpoint = 0,
		Best = 0,
		DailyStreak = 0,
		LastDailyClaim = "",
		HeistCores = 0,
		Upgrades = newUpgrades(),
		Missions = newMissions(),
	}
end

local function toNumber(value, fallback)
	if type(value) == "number" and value == value and value >= 0 then
		return value
	end
	return fallback
end

local function sanitize(raw)
	local data = newData()
	if type(raw) ~= "table" then
		return data
	end
	for _, key in ipairs({ "Speed", "Coins", "Rebirths", "Checkpoint", "Best" }) do
		data[key] = math.floor(toNumber(raw[key], 0))
	end
	if type(raw.DailyStreak) == "number" then
		data.DailyStreak = math.clamp(math.floor(raw.DailyStreak), 0, 7)
	end
	if type(raw.LastDailyClaim) == "string" then
		data.LastDailyClaim = raw.LastDailyClaim
	end
	if type(raw.HeistCores) == "number" then
		data.HeistCores = math.clamp(math.floor(raw.HeistCores), 0, Config.HeistCoreCount)
	end
	if type(raw.Upgrades) == "table" then
		for key, upgrade in pairs(Config.Upgrades) do
			local level = toNumber(raw.Upgrades[key], 0)
			data.Upgrades[key] = math.clamp(math.floor(level), 0, upgrade.MaxLevel)
		end
	end
	if type(raw.Missions) == "table" then
		for key in pairs(Config.Missions) do
			local saved = raw.Missions[key]
			if type(saved) == "table" then
				data.Missions[key].Level = math.clamp(math.floor(toNumber(saved.Level, 0)), 0, 100000)
				data.Missions[key].Progress = math.max(0, math.floor(toNumber(saved.Progress, 0)))
			end
		end
	end
	data.Best = math.max(data.Best, data.Speed)
	return data
end

local DataService = {}

function DataService.Load(player)
	local key = "player_" .. player.UserId
	local raw
	if store then
		local ok, result = pcall(function()
			return store:GetAsync(key)
		end)
		if ok then
			raw = result
		else
			warn("[DataService] No se pudo cargar a " .. player.Name .. ": " .. tostring(result))
		end
	else
		raw = memory[key]
	end
	local data = sanitize(raw)
	cache[player] = data
	return data
end

function DataService.Get(player)
	return cache[player]
end

function DataService.Save(player)
	local data = cache[player]
	if not data then
		return false
	end
	local key = "player_" .. player.UserId
	if not store then
		memory[key] = sanitize(data)
		return true
	end
	for attempt = 1, 3 do
		local ok, err = pcall(function()
			store:SetAsync(key, data)
		end)
		if ok then
			return true
		end
		warn("[DataService] Guardado fallido (" .. attempt .. "/3) para " .. player.Name .. ": " .. tostring(err))
		task.wait(1)
	end
	return false
end

function DataService.Release(player)
	cache[player] = nil
end

return DataService
